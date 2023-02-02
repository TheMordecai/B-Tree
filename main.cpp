#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct node{
  int *keys;
  node *parentptr;
  node **childptr;
  bool leaf;
  int size;

  node(int degree)
  {
    keys = new int[degree];
    childptr = new node*[degree + 1];
    leaf = true;
    size = 0;

    for(int i = 0; i < degree; i++)
    {
      keys[i] = -1;
    }

    for(int i = 0; i < degree + 1; i++)
    {
      childptr[i] = nullptr;  
    }

    parentptr = nullptr;
  }

};

class bTree{
  private:
    node *root;
    int degree;

  public:
    bTree()
    {
      root = nullptr;
      degree = 0;
    }
    node* getRoot(){
      return root;
    }
    void changeDegree(int _degree) 
    {
      root = nullptr;
      degree = _degree;
    }

    void insert(int data)
    {
      if(root == nullptr){
        root = new node(degree);
        root -> keys[0] = data;
        root -> size = 1;
      }
      
      else{
        addAtLeaf(nullptr, root, data);
      }
        
    }

    void addAtLeaf(node *parent, node *n, int data)
    {
      if(n -> leaf){
        int i = n -> size;
        while(i != 0 && n -> keys[i - 1] > data){
          n -> keys[i] = n -> keys[i - 1];
          i--;
        }

        n -> keys[i] = data;
        n -> size += 1;
      }
      else{
        int i = 0;
        while(i < n -> size && data > n -> keys[i]){
          i++;
        }
        
        addAtLeaf(n, n -> childptr[i], data);
      }

      if(n -> size == degree){
        if(n == root){
          node* temp = new node(degree);
          temp -> leaf = false;
          temp -> childptr[0] = n;
          // childptr[0] -> parentptr = temp;
          splitChild(temp, n);
          root = temp;
        }
        else{
          splitChild(parent, n);
        }
      }
      int i = 1;
      int height = getHeight(root, 0) + 1;
      while(i < height){
        printLevel(root, i);
        cout << endl;
        i++;
      }
    }

    void splitChild(node* parent, node* leftNode)
    {
      node* rightNode = new node(degree);
      rightNode -> parentptr = parent;
      int mid = (degree - 1) / 2;

      // copies half of the left node to right node
      // copies half of the pointer connected to the left node to the right node
      for(int i = 0; i < mid; i++){
        rightNode -> keys[i] = leftNode -> keys[i + mid + 1];
        rightNode -> size += 1;
      }


      // ****************************************************************
      // copies half the pointers of the left node to the right node
      if(leftNode -> leaf == false){
        for(int i = 0; i < mid + 1; i++){
          rightNode -> childptr[i] = leftNode -> childptr[i + mid + 1];
        }
        rightNode -> leaf == false;
      }

      // ****************************************************************

      // finds the correct position to add the new array
      int pos;
      for(int i = 0; i < parent -> size + 1; i++){
        if(parent->childptr[i] == leftNode){
          pos = i;
          break;
        }
      }
      
      if(parent->keys[0] == -1){
        parent -> childptr[1] = rightNode;
        parent -> keys[0] = leftNode -> keys[mid];
        parent -> size += 1;
      }
      else{
        parent -> keys[pos] = leftNode -> keys[mid];
        parent -> childptr[pos + 1] = rightNode;
        parent -> size += 1;
      }
      
      
      // adds the middle keys to the parent
    
      
      node* temp = new node(degree);

      for(int i = 0; i < mid; i++){
        temp -> keys[i] = leftNode -> keys[i];
        temp -> size += 1;
      }
      for(int i = 0; i < mid + 1; i++){
        temp -> childptr[i] = leftNode -> childptr[i];
      }
      leftNode = temp;
      parent -> childptr[pos] = leftNode;
      leftNode -> parentptr = parent;


      if(parent -> size == degree){
        if(parent == root){
          node* temp = new node(degree);
          temp -> leaf = false;
          temp -> childptr[0] = parent;
          splitChild(temp, parent);
          root = temp;
        }
        else{
          // traverseTree(root, parent);
          splitChild(parent -> parentptr, parent);
        }
      }
    }

// void printLevel(node *n, int level, ostream &out) 
    void printLevel(node *n, int level) 
    {
      if (n == nullptr) return;
      if (level == 1) {
        int i = 0;
        while (i < n -> size) {
          // out << n -> keys[i] << " ";
          cout << n -> keys[i] << " ";
          i++;
        }
      }

      else if (level > 1) {
        for (int i = 0; i < degree; i++) {
          if (n -> childptr[i] != nullptr){
            // printLevel(n -> childptr[i], level - 1, out);
            printLevel(n -> childptr[i], level - 1);
          }
        }
      }
    }

    int getHeight(node *n, int level) 
    {
      if (n == nullptr) return 0;

      if (n->leaf)
        return level;
      else {
        int max = 0;
        for (int i = 0; i < degree; i++) {
          if (n->childptr[i] != nullptr) {
            int height = getHeight(n->childptr[i], level + 1);
            if (height > max) max = height;
          }
        }
        return max;
      }
    }
};

int main() {
  bTree b;
  string file;
  int _degree;
  cout << "Enter the file name you wish to create a B-Tree with: ";
  cin >> file;
  if(!input.is_open()){
    cout << "File does not exist.";
    exit(-1);
  }
  cout << "Enter the degree for the B-Tree: ";
  cin >> _degree;
  b.changeDegree(_degree);

  ifstream input(file);

  vector<int> digits;
  string line = "", strDigit = "";

   

  while(getline(input, line)){
    for(int i = 0; i < line.length() + 1; i++){
      if(isdigit(line[i])){
        strDigit += line[i];
      }
      else{
        int digit = stol(strDigit);
        bool exists;
        for(int i = 0; i < digits.size(); i++){
          if(digits[i] == digit){
            exists = true;
            break;
          }
        }
        if(exists == false){
          digits.push_back(digit);
        }
        strDigit = "";
      }
    }
  }
  
  for(int i = 0; i < digits.size(); i++){
    b.insert(digits[i]);
  }

  return 0;
}