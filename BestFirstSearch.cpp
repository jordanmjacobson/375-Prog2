#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include<string>
using namespace std;
int num_items; //number of items from input
int capacity; //max capacity for the "knapsack", from input
struct  Node{ //represents algorithm state
  long level; //indicates which level on the "tree" we are
  long profit; //this profit represents the total profit so far. Based on the sum of all items at this state
  long weight; // total weight from all items in this state
  long bound; // the bound for branch and bound algorithm
  vector<int> items_used;
  bool taken;
};
struct CompareBound{ //implementation of comparator overload
  bool operator()(const Node& n1, const Node& n2){
    return n1.bound < n2.bound;
  }
};
int calculate_bound(Node);
//priority queue implementation based on https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
priority_queue<Node,vector<Node>,CompareBound> my_queue;
void sort_items();

struct item{
  long profit; //profit of the single item
  long weight; //weight of the single item
  double ratio; //profit/weight ratio
};
vector<item> items;

void sort_items(){//bubble sort this array based on ratio
  for(int i = 1; i<items.size();i++){
    for(int j = 1;j<items.size();j++){
      if (items[j].ratio< items[j+1].ratio){
        item temp = items[j];
        items[j] = items[j+1];
        items[j+1] = temp;
      }
    }
  }
}
int calculate_bound(Node my_node){ //to be used for priority queue comparison
  if (my_node.weight>=capacity){ //no need to do anything if we can't add anything else into the knapsack!
    return 0;
  }
  int weight_used = my_node.weight; //weight already in the bag
  long retVal = my_node.profit;
  for (long i = my_node.level+1; i<items.size();i++){
    if (weight_used < capacity){
      if(weight_used+items[i].weight<=capacity){
        retVal+= items[i].profit;
        weight_used+=items[i].weight;
      }
      else if(weight_used+items[i].weight> capacity){ //this would be the last item you could insert
         int available = capacity-weight_used;
         int fraction = available * (items[i].profit/items[i].weight) ;
         retVal+= fraction;
         //weight_used+= fraction * items[i].weight;
         //my_node.bound = retVal;
         return retVal;
      }
    }
  }
  //my_node.bound = retVal;
  return retVal;
}
int main(int argc, char * argv[]){
  int max_profit = 0;
  Node best_node;
  int leaves = 0;
  //constructing blank item for root node:
  item blank;
  blank.profit = 0;
  blank.weight = 0;
  blank.ratio = 0;
  items.push_back(blank);
  //constructing root node:
  Node root;
  root.level = -1;
  root.profit = 0;
  root.weight = 0;
  root.bound = calculate_bound(root);
  root.items_used.push_back(0);
  best_node = root;
  ifstream infile(argv[1]); //input file
  string first; //holds the number of items and the capacity
  infile >> first; //read in first line from input file
  size_t comma = first.find(","); //the comma delimiter. Will be recycled later...
  num_items = stoi(first.substr(0,comma));
  capacity =  stoi(first.substr(comma+1));
  string temp; //contains the profit and weight of ith item*/
  item my_item; //item to be added to vector of items

  for (int i = 0;i<num_items; i++){
    infile >> temp;
    comma = temp.find(","); //re-using from when it was first declared above
    my_item.weight = stoi(temp.substr(0,comma)); //sets the weight based on input
    my_item.profit = stoi(temp.substr(comma+1)); //sets the profit based on input
    my_item.ratio = my_item.profit/my_item.weight;
    items.push_back(my_item);

  }


  sort_items();//performs the bubble sort
  my_queue.push(root); //enqueue dummy node
  int nodes_visited = 0; //starts at one becaue of the root? (assuming we count the root)
  while (!my_queue.empty()){
    Node current = my_queue.top();
    my_queue.pop();
    nodes_visited++;
    //nodes_visited++;
    Node next;
    if(current.bound >max_profit){ //item added to knapsack
      next.level = current.level+1;
      next.profit = current.profit+items[next.level].profit;
      next.weight = current.weight + items[next.level].weight;
      next.bound = calculate_bound(next);
      next.taken = true;
      //next.items_used = current.items_used;


      if (next.weight <= capacity&& next.profit > max_profit){ //if the node is promising
        best_node = next;
        max_profit = best_node.profit;
        best_node.items_used = current.items_used;
        best_node.items_used.push_back(next.level);
      }
      else{
        //best_node.items_used.pop_back();
        next.taken = false;
        leaves++;
      }
      if (next.bound >max_profit){
        my_queue.push(next);
      }
    }
    else{
      leaves++;
    }
    next.level = current.level+1;
    next.profit = current.profit;
    next.weight = current.weight;
    next.bound = calculate_bound(next);
    next.items_used.push_back(next.level);
    if (next.bound>next.profit){
      my_queue.push(next);

    }


  }


  //for debug:
 std::cout<<"-------ITEMS-------"<<endl;
  for(int i=0;i<items.size();i++){
    cout<<"("<<items[i].weight<<","<<items[i].profit<<")"<<endl;
  }
  sort_items();
  std::cout<<"-------SORTED-------"<<endl;
  for(int i = 0; i<items.size();i++){
    cout<<"("<<items[i].weight<<","<<items[i].profit<<")"<<endl;
  }
  std::cout<<"max profit: "<<max_profit<<endl;
  std::cout<<"nodes visited: "<<nodes_visited<<endl;
  std::cout<<"items used: "<<endl;
  cout<<"Leaves: "<<leaves<<endl;
  for(int i = 0; i<best_node.items_used.size();i++)
  cout<<"("<<items[best_node.items_used[i]].weight<<","<<items[best_node.items_used[i]].profit<<")"<<endl;
}
