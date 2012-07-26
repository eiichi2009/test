#include <iostream>

template <class T>
class Tree
{
public:
  struct Node
  {
    T key;
    Node* child[2];
    Node(const T& k) : key(k)
    {
      child[0] = child[1] = NULL;
    }
  } *root;

  Tree() : root(NULL) {}
  Node* find(const T& k)
  {
    return find(root, k);
  }
  Node* find(Node* t, const T& k)
  {
    if (t == NULL || t->key == k) {
      return t;
    }
    if (t->key > k) {
      return find(t->child[0], k);
    }
    else {
      return find(t->child[1], k);
    }
  }

  void insert(const T& k)
  {
    root = insert(root, new Node(k));
  }
  Node* insert(Node* t, Node* x)
  {
    if (t == NULL) {
      return x;
    }
    if (t->key > x->key) {
      t->child[0] = insert(t->child[0], x);
    }
    else if (t->key < x->key) {
      t->child[1] = insert(t->child[1], x);
    }
    return t;
  }

  void erase(const T& k)
  {
    root = erase(root, k);
  }
  Node* erase(Node* t, const T& k)
  {
    if (t == NULL) {
      return t;
    }
    if (t->key == k) {
      return move_down(t->child[0], t->child[1]);
    }

    if (t->key > k) {
      t->child[0] = erase(t->child[0], k);
    }
    else {
      t->child[1] = erase(t->child[1], k);
    }
    return t;
  }
  Node* move_down(Node* lhs, Node* rhs)
  {
    if (lhs == NULL) {
      return rhs;
    }
    lhs->child[1] = move_down(lhs->child[1], rhs);
    return lhs;
  }
};

int main()
{
  Tree<int> t;

  t.insert(-1);
  t.insert(1000);
  t.insert(101);
  t.erase(-1);
  t.find(1000);

  return 0;
}

