#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

namespace LCAImplementation{
  template <class T>
  class Node{

  public:
    Node(T);
    ~Node();
    int compareTo(Node*);
    Node<T>* right = NULL;
    Node<T>* left = NULL;
    T val;
  };

  template <class T>
  class BT{

  public:
    BT(vector<T>);
    ~BT();
    Node<T>* LCA(T, T);
    void postorder(Node<T>*, int indent);
    Node<T>* getRoot();

  private:
    Node<T>* root = NULL;
    vector<Node<T>*> list;
    bool valueExists(T, Node<T>*);
    bool listFromNodeToRoot(T, Node<T>*);
  };
}
