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
};
struct CompareBound{ //implementation of comparator overload
  bool operator()(const Node& n1, const Node& n2){
    return n1.bound < n2.bound;
  }
};
void calculate_bound(Node);
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
  for(int i = 0; i<items.size();i++){
    for(int j = 0;j<items.size();j++){
      if (items[j].ratio< items[j+1].ratio){
        item temp = items[j];
        items[j] = items[j+1];
        items[j+1] = temp;
      }
    }
  }
}
void calculate_bound(Node my_node){ //to be used for priority queue comparison
  if (my_node.weight>=capacity){ //no need to do anything if we can't add anything else into the knapsack!
    return;
  }
  int weight_used = my_node.weight; //weight already in the bag
  long retVal = my_node.profit;
  for (long i = my_node.level; i<items.size();i++){
    if (weight_used < capacity){
      if(weight_used+items[i].weight<=capacity){
        retVal+= items[i].profit;
        weight_used+=items[i].weight;
      }
      else if(weight_used+items[i].weight> capacity){ //this would be the last item you could insert
         long available = capacity-weight_used;
         float fraction = available/items[i].weight;
         retVal+= fraction * items[i].profit;
         weight_used+= fraction * items[i].weight;
         my_node.bound = retVal;
         return;
      }
    }
  }
  my_node.bound = retVal;
  return;
}
int main(int argc, char * argv[]){


  ifstream infile(argv[1]); //input file
  string first; //holds the number of items and the capacity
  infile >> first; //read in first line from input file
  size_t comma = first.find(","); //the comma delimiter. Will be recycled later...
  num_items = stoi(first.substr(0,comma));
  capacity =  stoi(first.substr(comma+1));

  //for debug:
  std::cout<<"# of items: "<<num_items<<", capacity: "<<capacity<<endl;
  string temp; //contains the profit and weight of ith item
  item my_item; //item to be added to vector of items
  for (int i = 0;i<num_items; i++){
    infile >> temp;
    comma = temp.find(","); //re-using from when it was first declared above
    my_item.weight = stoi(temp.substr(0,comma)); //sets the weight based on input
    my_item.profit = stoi(temp.substr(comma+1)); //sets the profit based on input
    my_item.ratio = my_item.profit/my_item.weight;
    items.push_back(my_item);
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
}