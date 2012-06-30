#ifndef AVL_HPP

#include <iostream>
#include <vector>
#include <algorithm>

class Tree
{
private:
  Tree* m_l;
  Tree* m_r;
  Tree* m_u;
  int m_b; // left(-1), balanced(0), right(1)
  int m_v;

  const static int TOP_VAL;

public:
  Tree(int v = TOP_VAL);
  bool insert(int v);
  bool remove(int v);

  template <Tree* Tree::*m_l, Tree* Tree::*m_r, int balance>
  friend bool rebalance1(Tree* t);

  template <Tree* Tree::*m_l, Tree* Tree::*m_r, int balance>
  friend void rebalance2(Tree* t);
};

#endif // AVL_HPP
