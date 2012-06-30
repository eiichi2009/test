#include "plaintree.cpp"

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
