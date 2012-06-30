#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>

class Tree
{
public:
  const static int TOP_VAL;

  virtual ~Tree();

  virtual bool insert(int v) = 0;
  virtual bool remove(int v) = 0;
  virtual bool find(int v) = 0;
};


class PlainTree : public Tree
{
private:
  PlainTree* m_l;
  PlainTree* m_r;
  int m_v;

public:
  PlainTree(int v = TOP_VAL);
  bool insert(int v);
  bool remove(int v);
  bool find(int v);
};

class AvlTree : public Tree
{
private:
  AvlTree* m_l;
  AvlTree* m_r;
  AvlTree* m_u;
  int m_v;
  int m_b;

public:
  AvlTree(int v = TOP_VAL);
  bool insert(int v);
  bool remove(int v);
  bool find(int v);

  template <AvlTree* AvlTree::*ml, AvlTree* AvlTree::*mr, int balance>
  bool rebalance1(AvlTree* t);

  template <AvlTree* AvlTree::*ml, AvlTree* AvlTree::*mr, int balance>
  bool rebalance2(AvlTree* t);
};

#endif // TREE_HPP
