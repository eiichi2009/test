#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/intrusive/avl_set.hpp>

using namespace boost::intrusive;
using namespace std;

template <class T>
struct AvlTree
{
  struct Node
  {
    T key;
    Node* child[2];
    Node* parent;
    int balance;
    Node(const T& k) : key(k)
    {
      child[0] = child[1] = NULL;
      parent = NULL;
      balance = 0;
    }
  } *root;

  void show()
  {
    show(root, 1);
  }
  void show(Node* t, int n)
  {
    if (t) {
      show(t->child[0], n+1);
      for (int i = 0; i < n; ++i) {
	cout << " ";
      }
      cout << t->key << "(" << t->balance << ")" << endl;
      show(t->child[1], n+1);
    }
  }

  AvlTree() : root(NULL)
  {
  }

  ~AvlTree()
  {
    root = eraseAll(root);
  }

  Node* eraseAll(Node* t)
  {
    if (t) {
      if (t->child[0]) {
	eraseAll(t->child[0]);
      }
      if (t->child[1]) {
	eraseAll(t->child[1]);
      }
      delete t;
    }
    return NULL;
  }

  bool rebalance1(Node* t, int a, int b, int balance)
  {
    /*******************************
              P              P
              |              |
              A              B
             / \            / \
            /   \          /   \
           B    (C)      (D)    A
          / \            (D)   / \
        (D) (E)              (E) (C)
        (D) (E)              (E)
    
          P           P
          |           |
          A           B
         /           / \
        /           /	\
       B           D     A
      / \               /
     D   E             E

    *******************************/

    Node* P = t->parent;
    Node* A = t;
    Node* B = A->child[a];
    Node* E = B->child[b];
    bool decreased = true;

    if (P) {
      if (P->key > A->key) {
	P->child[0] = B;
      }
      else {
	P->child[1] = B;
      }
    }

    A->parent = B;
    A->child[a] = E;
    // A->child[1];

    B->parent = P;
    if (P == NULL) {
      root = B;
    }
    // B->child[0];
    B->child[b] = A;

    if (B->balance == -balance) {
      A->balance = 0;
      B->balance = 0;
      decreased = false;
    }
    else {
      A->balance = -balance;
      B->balance = balance;
    }

    if (E) {
      E->parent = A;
    }

    return decreased;
  }

  bool rebalance2(Node* t, int a, int b, int balance)
  {
    /*******************************
           P           P
           |           |
           A           E
          / \         / \
         /   \       /   \
        B    (C)    B     A
       / \         / \   / \
     (D)  E      (D) (F|G) (C)
         / \
       (F) (G)

          P           P
          |           |
          A           E
         /           / \
        /           /	\
       B           B     A
    	\
         E
    *******************************/

    Node* P = t->parent;
    Node* A = t;
    Node* B = A->child[a];
    Node* E = B->child[b];
    Node* F = E->child[a];
    Node* G = E->child[b];

    if (P) {
      if (P->key > A->key) {
	P->child[0] = E;
      }
      else {
	P->child[1] = E;
      }
    }

    // A->child[0]; depeneds on F
    //  A->child[1];
    // A->balance; depeneds on F
    A->parent = E;

    // B->child[0];
    // B->child[1]; depends on F
    // B->balance; depeneds on F
    B->parent = E;

    if (F) {
      B->child[b] = F;
      F->parent = B;
    }
    else {
      B->child[b] = NULL;
    }

    if (E->balance == balance) {
      B->balance = -balance;
    }
    else {
      B->balance = 0;
    }

    if (G) {
      A->child[a] = G;
      G->parent = A;
    }
    else {
      A->child[a] = NULL;
    }

    if (E->balance == -balance) {
      A->balance = balance;
    }
    else {
      A->balance = 0;
    }

    E->child[a] = B;
    E->child[b] = A;
    E->balance = 0;
    E->parent = P;
    if (P == NULL) {
      root = E;
    }

    return true;
  }

  bool single_right(AvlTree::Node* t)
  {
    return rebalance1(t, 0, 1, 1);
  }

  bool double_right(AvlTree::Node* t)
  {
    return rebalance2(t, 0, 1, 1);
  }

  bool single_left(AvlTree::Node* t)
  {
    return rebalance1(t, 1, 0, -1);
  }

  bool double_left(AvlTree::Node* t)
  {
    return rebalance2(t, 1, 0, -1);
  }

  bool insert(const T& v)
  {
    Node *t = root;
    Node *pt = NULL;
    while (t) {
      if (v == t->key) {
	return false;
      }

      pt = t;
      if (v > t->key) {
	t = t->child[1];
      }
      else {
	t = t->child[0];
      }
    }

    Node* newt = new Node(v);
    if (pt) {
      if (v > pt->key) {
	pt->child[1] = newt;
	pt->balance++;
      }
      else {
	pt->child[0] = newt;
	pt->balance--;
      }
      newt->parent = pt;
    }
    else {
      root = newt;
      return true;
    }

    Node* ct = newt;
    t = pt;
    while (t->parent != NULL) {
      if (t->balance == -2) {
	if (ct->balance == -1) {
	  single_right(t);
	}
	else if (ct->balance == 1) {
	  double_right(t);
	}
	break;
      }
      else if (t->balance == 2) {
	if (ct->balance == 1) {
	  single_left(t);
	}
	else if (ct->balance == -1) {
	  double_left(t);
	}
	break;
      }
      else if (t->balance == 0) {
	break;
      }
      else {
	pt = t->parent;

	if (t->key > pt->key) {
	  pt->balance++;
	}
	else {
	  pt->balance--;
	}
	ct = t;
	t = t->parent;
      }
    }
    return true;
  }

  bool erase(const T& v)
  {
    //    cout << "erase(" << v << ")" << endl;
    Node* t = root;
    Node* pt = NULL;
    bool found = false;

    while (t) {
      if (v == t->key) {
	found = true;
	break;
      }

      pt = t;
      if (v > t->key) {
	t = t->child[1];
      }
      else {
	t = t->child[0];
      }
    }
    if (found == false) {
      return false;
    }

    if (t->child[0] == NULL) {
      if (pt) {
	if (t->key > pt->key) {
	  pt->child[1] = t->child[1];
	  pt->balance--;
	}
	else {
	  pt->child[0] = t->child[1];
	  pt->balance++;
	}
	if (t->child[1]) {
	  t->child[1]->parent = pt;
	}
	delete t;
      }
      else {
	root = t->child[1];
	if (t->child[1]) {
	  t->child[1]->parent = NULL;
	}
	delete t;
	return true;
      }
    }
    else if (t->child[1] == NULL) {
      if (pt) {
	if (t->key > pt->key) {
	  pt->child[1] = t->child[0];
	  pt->balance--;
	}
	else {
	  pt->child[0] = t->child[0];
	  pt->balance++;
	}
	t->child[0]->parent = pt;
	delete t;
      }
      else {
	root = t->child[0];
	t->child[0]->parent = NULL;
	delete t;
	return true;
      }
    }
    else {
      Node* maxL = t->child[0];
      Node* pmaxL = t;
      while (maxL->child[1]) {
	pmaxL = maxL;
	maxL = maxL->child[1];
      }

      // removing maxL from pmaxL
      if (maxL->key > pmaxL->key) {
	pmaxL->child[1] = maxL->child[0];
	pmaxL->balance--;
      }
      else {
	pmaxL->child[0] = maxL->child[0];
	pmaxL->balance++;
      }
      if (maxL->child[0]) {
	maxL->child[0]->parent = pmaxL;
      }

      // replacing t with maxL
      if (pt) {
	if (t->key > pt->key) {
	  pt->child[1] = maxL;
	}
	else {
	  pt->child[0] = maxL;
	}
      }
      else {
	root = maxL;
      }
      maxL->parent = pt;
      maxL->child[0] = t->child[0];
      if (t->child[0]) {
	t->child[0]->parent = maxL;
      }
      maxL->child[1] = t->child[1];
      if (t->child[1]) {
	t->child[1]->parent = maxL;
      }
      maxL->balance = t->balance;

      if (t == pmaxL) {
	//	cout << "HHHHHHHHHHHHHHHHHHHHH" << endl;
	pmaxL = maxL;
      }
      else {
	delete t;
      }
      t = maxL;
      pt = pmaxL;
    }

    if (pt == NULL) {
      return true;
    }

    t = pt;
    while (t->parent != NULL) {
      pt = t->parent;
      bool pt_left_leg = true;
      if (pt->key < t->key) {
	// pt's left leg
	pt_left_leg = false;
      }

      if (t->balance == -2) {
	if (t->child[0]->balance == 1) {
	  double_right(t);
	}
	else {
	  single_right(t);
	}
      }
      else if (t->balance == 2) {
	if (t->child[1]->balance == -1) {
	  double_left(t);
	}
	else {
	  single_left(t);
	}
      }

      if (pt_left_leg) {
	t = pt->child[0];
      }
      else {
	t = pt->child[1];
      }

      if (t->balance != 0) {
	break;
      }

      if (pt_left_leg) {
	pt->balance++;
      }
      else {
	pt->balance--;
      }
      t = pt;
    }
    return true;
  }

  Node* find(const T& k) const
  {
    Node* p = root;
    while (p) {
      if (p->key == k) {
	return p;
      }
      else if (p->key > k) {
	p = p->child[0];
      }
      else {
	p = p->child[1];
      }
    }
    return NULL;
  }
};

#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

void
test1(int n)
{
  srand(time(NULL));

  vector<int> v;
  for (int i = 0; i < n; ++i) {
    v.push_back(rand() % n);
  }
  random_shuffle(v.begin(), v.end());

  AvlTree<int> t;
  for (int i = 0; i < n; ++i) {
    t.insert(v[i]);
  }

  for (int i = 0; i < n; ++i) {
    t.erase(v[i]);
  }
}

void
test2()
{
  AvlTree<int> t;
  int a[] = {0, 0, 1, 6, 9, 8, 7, 4, 3};
  for (unsigned int i = 0; i < sizeof(a)/sizeof(int); ++i) {
    t.insert(a[i]);
  }
  t.show();
  for (unsigned int i = 0; i < sizeof(a)/sizeof(int); ++i) {
    t.erase(a[i]);
  }
}

void
test3()
{
  AvlTree<int> t;
  t.insert(1);
  t.insert(1);
  t.insert(0);
  t.erase(1);
  t.erase(0);
}

class test : public avl_set_base_hook<optimize_size<true> >
{
  avl_set_member_hook<> member_hook;
  int key;

public:
  test(int k) : key(k) {}
  void set(int k)
  {
    key = k;
  }

  bool operator<(const test& rhs) const
  {
    return this->key < rhs.key;
  }
  bool operator>(const test& rhs) const
  {
    return this->key > rhs.key;
  }
  bool operator==(const test& rhs) const
  {
    return this->key == rhs.key;
  }
};

int boost_test1(int n)
{
  avl_set<test> st;
  for (int i = 0; i < n; ++i) {
    test *t = new test(i);
    st.insert(*t);
  }

  test t(100);
  int count = 0;
  for (int i = 0; i < n; ++i) {
    t.set(i);
    avl_set<test>::iterator it = st.find(t);
    if (it != st.end()) {
      ++count;
    }
  }
  cout << "found: " << count << endl;
  return 0;
}

int AvlTree_test1(int n)
{
  AvlTree<test> st;
  for (int i = 0; i < n; ++i) {
    test *t = new test(i);
    st.insert(*t);
  }

  test t(100);
  int count = 0;
  for (int i = 0; i < n; ++i) {
    t.set(i);
    AvlTree<test>::Node* it = st.find(t);
    if (it) {
      ++count;
    }
  }
  cout << "found: " << count << endl;
  return 0;
}

#include <sys/time.h>
struct __b__
{
  double start;
  __b__()
  {
    start = sec();
  }
  ~__b__()
  {
    cout << "finished: " << sec() - start << endl;
  }
  double sec()
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec*1e-6;
  }
  operator bool()
  {
    return false;
  }
};
#define bench() if (__b__ __b = __b__()); else


int
main(int ac, char **av)
{
  int test_num = atoi(av[1]);
  int n = atoi(av[2]);

  if (test_num == 1) {
    cout << "AvlTree test: " << n << endl;
    bench() {
      AvlTree_test1(n);
    }
  }

  if (test_num == 2) {
    cout << "boost test: " << n << endl;
    bench() {
      boost_test1(n);
    }
  }

  return 0;
}
