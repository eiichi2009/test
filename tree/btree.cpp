#include <iostream>
#include <list>
#include <typeinfo>

using namespace std;

class Node;
typedef int value;
typedef int key;
typedef std::pair<key,value>			LeafPair;
typedef std::pair<key,Node*>			NonLeafPair;
typedef std::list<LeafPair>::iterator		lpIterator;
typedef std::list<NonLeafPair>::iterator	nlpIterator;

class Node
{
protected:
  int maxItem;

public:
  Node() : maxItem(0) {}
  virtual ~Node() {}

  bool overflow() const
  {
    return size() > maxItem;
  }

  bool underflow() const
  {
    return size() < (maxItem+1) / 2;
  }

  virtual int size() const = 0;
  virtual Node* OnlyOneChild() = 0;
  virtual void insert(key k, value v) = 0;
  virtual bool find(key k, value& v) = 0;
  virtual bool erase(key k) = 0;
  virtual pair<key,Node*> split() = 0;
  virtual void combine(Node* nd) = 0;
  virtual void show() = 0;
};

class Leaf : public Node
{
  std::list<LeafPair> data; // <key,value> ~~ <key,value> ~~ ...

public:
  Leaf(int mi)
  {
    maxItem = mi;
  }

  ~Leaf() {}

  int size() const
  {
    return data.size();
  }

  Node* OnlyOneChild()
  {
    return NULL;
  }

  void insert(key k, value v)
  {
    lpIterator it;
    for (it = data.begin(); it != data.end(); ++it) {
      if (it->first >= k) {
	break;
      }
    }
    if (it != data.end() && it->first == k) {
      it->second = v;
    }
    else {
      data.insert(it, LeafPair(k,v));
    }
    return;
  }

  bool find(key k, value& v)
  {
    for (lpIterator it = data.begin(); it != data.end(); ++it) {
      if (it->first == k) {
	v = it->second;
	return true;
      }
    }
    return false;
  }

  bool erase(key k)
  {
    for (lpIterator it = data.begin(); it != data.end(); ++it) {
      if (it->first == k) {
	data.erase(it);
	return true;
      }
    }
    return false;
  }

  NonLeafPair split()
  {
    Leaf* new_nd = new Leaf(maxItem);
    for (int i = 0; i < (maxItem+1) / 2; ++i) {
      LeafPair lp = data.front();
      data.pop_front();
      new_nd->data.push_back(lp);
    }
    return NonLeafPair(data.front().first, newNode);
  }

  void combine(Node* nd)
  {
    if (typeid(*nd) == typeid(Leaf)) {
      Leaf* l = (Leaf*)nd;
      while (l->data.empty() == false) {
	LeafPair lp = l->data.back();
	l->data.pop_back();
	data.push_front(lp);
      }
    }
    else {
      throw("cannot combine non-leaf with leaf");
    }
  }

  void show()
  {
    for (lpIterator it = data.begin(); it != data.end(); ++it) {
      cout << it->first << "(" << it->second << ") ";
    }
    cout << endl;
  }
};

class NonLeaf : public Node
{
private:
  std::list<NonLeafPair> data; // <key,Node>,<key,Node>, ...

public:
  NonLeaf(int mi)
  {
    maxItem = mi;
  }

  NonLeaf(int mi, Node& nd)
  {
    maxItem = mi;
    data.push_front(NonLeafPair(0, &nd));
  }

  ~NonLeaf() {}

  int size() const
  {
    return data.size();
  }

  nlpIterator locate(key k)
  {
    nlpIterator it;
    for (it = data.begin(); it != data.end(); ++it) {
      if (it->first > k) {
	break;
      }
    }
    if (it == data.end()) {
      --it;
    }
    return it;
  }

  void insert(key k, value v)
  {
    nlpIterator it = locate(k);
    Node* nd = it->second;
    nd->insert(k,v);
    if (nd->overflow()) {
      data.insert(it, nd->split());
    }
    return;
  }

  bool find(key k, value& v)
  {
    return locate(k)->second->find(k,v);
  }

  bool erase(key k)
  {
    nlpIterator it = locate(k);
    Node* nd = it->second;
    if (nd->erase(k)) {
      if (nd->underflow()) {
	nlpIterator larger_it = it;
	if (it == data.begin()) {
	  ++larger_it;
	}
	nlpIterator smaller_it = larger_it;
	--smaller_it;

	Node* larger_nd = larger_it->second;
	Node* smaller_nd = smaller_it->second;

	larger_nd->combine(smaller_nd);
	delete smaller_nd;
	data.erase(smaller_it);
	if (larger_nd->overflow()) {
	  data.insert(larger_it, larger_nd->split());
	}
      }
      return true;
    }
    else {
      return false;
    }
  }

  NonLeafPair split()
  {
    NonLeaf* new_nd = new NonLeaf(maxItem);
    for (int i = 0; i < (maxItem+1) / 2; ++i) {
      NonLeafPair nlp = data.front();
      data.pop_front();
      new_nd->data.push_back(nlp);
    }
    return NonLeafPair(new_nd->data.back().first, new_nd);
  }

  void combine(Node* nd)
  {
    if (typeid(*nd) == typeid(NonLeaf)) {
      NonLeaf* nl = (NonLeaf*)nd;
      while (nl->data.empty() == false) {
	NonLeafPair nlp = nl->data.back();
	nl->data.pop_back();
	data.push_front(nlp);
      }
    }
    else {
      throw("cannot combine Leaf with NonLeaf");
    }
  }

  Node* OnlyOneChild()
  {
    if (data.size() == 1) {
      return data.begin()->second;
    }
    else {
      return NULL;
    }
  }

  Node* GetFirstChild()
  {
    return data.front().second;
  }

  void SplitFirstChild()
  {
    data.push_front(data.front().second->split());
  }

  void show()
  {
    for (nlpIterator it = data.begin(); it != data.end(); ++it) {
      cout << "[" << it->first << "] ";
      it->second->show();
    }
  }
};

class BTree
{
  Node* root;
  int M; // the number of data in NonLeaf
  int L; // the number of data in Leaf

  /***********************************************************
    NonLeaf
    +----------+----------+----------+----------+----------+------------+
    | child[0] | child[1] | child[2] | child[3] | child[4] | child[M-1] |
    +----------+----------+----------+----------+----------+------------+
          |          |
	  |          |
    Leaf  |          |
    +---------+---------+---------+-----------+
    | data[0] | data[1] | data[2] | data[L-1] |
    +---------+---------+---------+-----------+
                     |
                     |
            Leaf     |
            +---------+---------+---------+-----------+
            | data[0] | data[1] | data[2] | data[L-1] |
            +---------+---------+---------+-----------+
  ***********************************************************/

public:
  BTree(int m_, int l_) : root(NULL), M(m_), L(l_) {}
  ~BTree() {}

  void show() const
  {
    cout << "###" << endl;
    root->show();
  }

  void insert(key k, value v)
  {
    if (root == NULL) {
      root = new Leaf(L);
    }
    root->insert(k,v);

    if (root->overflow()) {
      Node* ex_root = root;
      root = new NonLeaf(M, *ex_root);
      ((NonLeaf*)root)->SplitFirstChild();
    }
    return;
  }

  bool find(key k, value& v) const
  {
    if (root == NULL) {
      return false;
    }
    else {
      return root->find(k,v);
    }
  }

  bool erase(key k)
  {
    if (root == NULL) {
      return false;
    }
    else {
      bool found = root->erase(k);
      Node* tmp = root->OnlyOneChild();
      if (tmp) {
	delete root;
	root = tmp;
      }

      return found;
    }
  }
};

int
main()
{
  BTree t(6,4);
  t.insert(100,10);  t.show();
  t.insert(101,10);  t.show();
  t.insert(102,10);  t.show();
  t.insert(103,10);  t.show();
  t.insert(104,10);  t.show();
  t.insert(105,10);  t.show();
  t.insert(106,10);  t.show();
  return 0;
}
