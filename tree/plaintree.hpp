#ifndef PLAINTREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>

class Tree
{
private:
  Tree* m_l;
  Tree* m_r;
  int m_v;
  const static int TOP_VAL;

public:
  Tree(int v = TOP_VAL);
  bool insert(int v);
  bool remove(int v);
};

#endif // PLAINTREE_HPP

