#include "tree.hpp"

const int Tree::TOP_VAL = -1;

Tree::~Tree()
{
}

PlainTree::PlainTree(int v) : m_l(NULL), m_r(NULL), m_v(v)
{
}

bool
PlainTree::find(int v)
{
  PlainTree* t = this;
  while (t) {
    if (v == t->m_v) {
      return true;
    }
    if (v > t->m_v) {
      t = t->m_r;
    }
    else {
      t = t->m_l;
    }
  }
  return false;
}

bool
PlainTree::insert(int v)
{
  PlainTree *t = this;
  PlainTree *pt = NULL;
  while (t) {
    if (v == t->m_v) {
      return false;
    }

    pt = t;
    if (v > t->m_v) {
      t = t->m_r;
    }
    else {
      t = t->m_l;
    }
  }

  PlainTree* newt = new PlainTree(v);
  if (v > pt->m_v) {
    pt->m_r = newt;
  }
  else {
    pt->m_l = newt;
  }
  return true;
}
  
bool
PlainTree::remove(int v)
{
  PlainTree* t = this;
  PlainTree* pt = NULL;
  bool found = false;

  while (t) {
    if (v == t->m_v) {
      found = true;
      break;
    }

    pt = t;
    if (v > t->m_v) {
      t = t->m_r;
    }
    else {
      t = t->m_l;
    }
  }
  if (found == false) {
    return false;
  }

  if (t->m_l == NULL) {
    if (t->m_v > pt->m_v) {
      pt->m_r = t->m_r;
    }
    else {
      pt->m_l = t->m_r;
    }
    delete t;
  }
  else if (t->m_r == NULL) {
    if (t->m_v > pt->m_v) {
      pt->m_r = t->m_l;
    }
    else {
      pt->m_l = t->m_l;
    }
    delete t;
  }
  else {
    PlainTree* maxL = t->m_l;
    PlainTree* pmaxL = t;
    while (maxL && maxL->m_r) {
      pmaxL = maxL;
      maxL = maxL->m_r;
    }

    // removing maxL from pmaxL
    if (maxL->m_v > pmaxL->m_v) {
      pmaxL->m_r = maxL->m_l;
    }
    else {
      pmaxL->m_l = maxL->m_l;
    }

    // replacing t with maxL
    if (t->m_v > pt->m_v) {
      pt->m_r = maxL;
    }
    else {
      pt->m_l = maxL;
    }
    maxL->m_l = t->m_l;
    maxL->m_r = t->m_r;

    if (t == pmaxL) {
      pmaxL = maxL;
    }
    
    delete t;
  }
  return true;
}




AvlTree::AvlTree(int v) : m_l(NULL), m_r(NULL), m_u(NULL), m_v(v), m_b(0)
{
}

bool
AvlTree::find(int v)
{
  AvlTree* t = this;
  while (t) {
    if (v == t->m_v) {
      return true;
    }
    if (v > t->m_v) {
      t = t->m_r;
    }
    else {
      t = t->m_l;
    }
  }
  return false;
}

// rebalance1<ml, mr, 1>(): clockwise rotation
// rebalance1<mr, ml, -1>(): counter-clockwise rotation
template <AvlTree* AvlTree::*ml, AvlTree* AvlTree::*mr, int balance>
bool AvlTree::rebalance1(AvlTree* t)
{
  //          P              P
  //          |              |
  //          A              B
  //         / \            / \
  //        /   \          /   \
  //       B    (C)      (D)    A
  //      / \            (D)   / \
  //    (D) (E)              (E) (C)
  //    (D) (E)              (E)

  //      P           P
  //      |           |
  //      A           B
  //     /           / \
  //    /           /   \
  //   B           D     A
  //  / \               /
  // D   E             E

  AvlTree* P = t->m_u;
  AvlTree* A = t;
  AvlTree* B = A->*ml;
  AvlTree* E = B->*mr;
  bool decreased = true;

  if (P->m_v > A->m_v) {
    P->m_l = B;
  }
  else {
    P->m_r = B;
  }

  A->m_u = B;
  A->*ml = E;
  // A->m_r;

  B->m_u = P;
  // B->m_l;
  B->*mr = A;

  if (B->m_b == -balance) {
    A->m_b = 0;
    B->m_b = 0;
    decreased = false;
  }
  else {
    A->m_b = -balance;
    B->m_b = balance;
  }

  if (E) {
    E->m_u = A;
  }

  return decreased;
}

// rebalance2<m_l, m_r, 1>(): clockwise rotation
// rebalance2<m_r, m_l, -1>(): counter clockwise rotation
template <AvlTree* AvlTree::*ml, AvlTree* AvlTree::*mr, int balance>
bool AvlTree::rebalance2(AvlTree* t)
{
  //       P           P
  //       |           |
  //       A           E
  //      / \         / \
  //     /   \       /   \
  //    B    (C)    B     A
  //   / \         / \   / \
  // (D)  E      (D) (F|G) (C)
  //     / \
  //   (F) (G)

  //      P           P
  //      |           |
  //      A           E
  //     /           / \
  //    /           /   \
  //   B           B     A
  //    \
  //     E

  AvlTree* P = t->m_u;
  AvlTree* A = t;
  AvlTree* B = A->*ml;
  AvlTree* E = B->*mr;
  AvlTree* F = E->*ml;
  AvlTree* G = E->*mr;

  if (P->m_v > A->m_v) {
    P->m_l = E;
  }
  else {
    P->m_r = E;
  }

  // A->m_l; depeneds on F
  //  A->m_r;
  // A->m_b; depeneds on F
  A->m_u = E;

  // B->m_l;
  // B->m_r; depends on F
  // B->m_b; depeneds on F
  B->m_u = E;

  if (F) {
    B->*mr = F;
    F->m_u = B;
  }
  else {
    B->*mr = NULL;
  }

  if (E->m_b == balance) {
    B->m_b = -balance;
  }
  else {
    B->m_b = 0;
  }

  if (G) {
    A->*ml = G;
    G->m_u = A;
  }
  else {
    A->*ml = NULL;
  }

  if (E->m_b == -balance) {
    A->m_b = balance;
  }
  else {
    A->m_b = 0;
  }

  E->*ml = B;
  E->*mr = A;
  E->m_b = 0;
  E->m_u = P;

  return true;
}

bool
AvlTree::insert(int v)
{
  AvlTree *t = this;
  AvlTree *pt = NULL;
  while (t) {
    if (v == t->m_v) {
      return false;
    }

    pt = t;
    if (v > t->m_v) {
      t = t->m_r;
    }
    else {
      t = t->m_l;
    }
  }

  AvlTree* newt = new AvlTree(v);
  if (v > pt->m_v) {
    pt->m_r = newt;
    pt->m_b++;
  }
  else {
    pt->m_l = newt;
    pt->m_b--;
  }
  newt->m_u = pt;

  AvlTree* ct = newt;
  t = pt;
  while (t->m_v != AvlTree::TOP_VAL) {
    if (t->m_b == -2) {
      if (ct->m_b == -1) {
	rebalance1<&AvlTree::m_l, &AvlTree::m_r, 1>(t);
      }
      else if (ct->m_b == 1) {
	rebalance2<&AvlTree::m_l, &AvlTree::m_r, 1>(t);
      }
      break;
    }
    else if (t->m_b == 2) {
      if (ct->m_b == 1) {
	rebalance1<&AvlTree::m_r, &AvlTree::m_l, -1>(t);
      }
      else if (ct->m_b == -1) {
	rebalance2<&AvlTree::m_r, &AvlTree::m_l, -1>(t);
      }
      break;
    }
    else if (t->m_b == 0) {
      break;
    }
    else {
      pt = t->m_u;

      if (t->m_v > pt->m_v) {
	pt->m_b++;
      }
      else {
	pt->m_b--;
      }
      ct = t;
      t = t->m_u;
    }
  }
  return true;
}
  
bool
AvlTree::remove(int v)
{
  AvlTree* t = this;
  AvlTree* pt = NULL;
  bool found = false;

  while (t) {
    if (v == t->m_v) {
      found = true;
      break;
    }

    pt = t;
    if (v > t->m_v) {
      t = t->m_r;
    }
    else {
      t = t->m_l;
    }
  }
  if (found == false) {
    return false;
  }

  if (t->m_l == NULL) {
    if (t->m_v > pt->m_v) {
      pt->m_r = t->m_r;
      pt->m_b--;
    }
    else {
      pt->m_l = t->m_r;
      pt->m_b++;
    }
    if (t->m_r) {
      t->m_r->m_u = pt;
    }
    delete t;
  }
  else if (t->m_r == NULL) {
    if (t->m_v > pt->m_v) {
      pt->m_r = t->m_l;
      pt->m_b--;
    }
    else {
      pt->m_l = t->m_l;
      pt->m_b++;
    }
    t->m_l->m_u = pt;
    delete t;
  }
  else {
    AvlTree* maxL = t->m_l;
    AvlTree* pmaxL = t;
    while (maxL) {
      pmaxL = maxL;
      maxL = maxL->m_r;
    }

    // replacing t with pmaxL 

    // removing pmaxL from ppmaxL
    AvlTree* ppmaxL = pmaxL->m_u;
    if (pmaxL->m_v > ppmaxL->m_v) {
      ppmaxL->m_r = pmaxL->m_l;
      ppmaxL->m_b--;
    }
    else {
      ppmaxL->m_l = pmaxL->m_l;
      ppmaxL->m_b++;
    }
    if (pmaxL->m_l) {
      pmaxL->m_l->m_u = ppmaxL;
    }

    // replacing t with pmaxL
    if (t->m_v > pt->m_v) {
      pt->m_r = pmaxL;
    }
    else {
      pt->m_l = pmaxL;
    }
    pmaxL->m_u = t->m_u;
    pmaxL->m_l = t->m_l;

    if (t->m_l) {
      t->m_l->m_u = pmaxL;
    }
    pmaxL->m_r = t->m_r;
    if (t->m_r) {
      t->m_r->m_u = pmaxL;
    }
    pmaxL->m_b = t->m_b;

    if (t == ppmaxL) {
      ppmaxL = pmaxL;
    }
    
    delete t;
    t = pmaxL;
    pt = ppmaxL;
  }

  t = pt;
  while (t->m_v != AvlTree::TOP_VAL) {
    pt = t->m_u;
    bool pt_left_leg = true;
    if (pt->m_v < t->m_v) {
      // pt's left leg
      pt_left_leg = false;
    }

    if (t->m_b == -2) {
      if (t->m_l->m_b == 1) {
	rebalance2<&AvlTree::m_l, &AvlTree::m_r, 1>(t);
      }
      else {
	rebalance1<&AvlTree::m_l, &AvlTree::m_r, 1>(t);
      }
    }
    else if (t->m_b == 2) {
      if (t->m_r->m_b == -1) {
	rebalance2<&AvlTree::m_r, &AvlTree::m_l, -1>(t);
      }
      else {
	rebalance1<&AvlTree::m_r, &AvlTree::m_l, -1>(t);
      }
    }

    if (pt_left_leg) {
      t = pt->m_l;
    }
    else {
      t = pt->m_r;
    }

    if (t->m_b != 0) {
      break;
    }

    if (pt_left_leg) {
      pt->m_b++;
    }
    else {
      pt->m_b--;
    }
    t = pt;
  }
  return true;
}
