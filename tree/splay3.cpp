#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

typedef int keyT;

struct Tree
{
  struct Node
  {
    keyT key;
    Node* child[2];
    Node(keyT k=0) : key(k)
    {
      child[0] = child[1] = NULL;
    }
  };

  Node* root;
  Tree() : root(NULL) {}
  ~Tree()
  {
    if (root) {
      destroy_tree(root);
    }
  }

  void destroy_tree(Node* x)
  {
    if (x) {
      destroy_tree(x->child[0]);
      destroy_tree(x->child[1]);
      delete x;
    }
  }

  Node* rotate_R(Node* x)
  {
    Node* s = x->child[0];
    x->child[0] = s->child[1];
    s->child[1] = x;
    return s;
  }

  Node* rotate_L(Node* x)
  {
    Node* s = x->child[1];
    x->child[1] = s->child[0];
    s->child[0] = x;
    return s;
  }

  Node* splay(Node* nd, keyT x)
  {
    Node* p = nd;

    Node w;
    Node* lp = &w; // left branch of x after splay. Only uses rp->child[0]
    Node* rp = &w; // right branch. Only uses rp->child[1]

    while (1) {
      if (p->key == x) {
	break;
      }
      else if (p->key > x) {
	// p is larger than x. So p will be right branch (rp->child[1])
	if (p->child[0] == NULL) {
	  break;
	}
	if (p->child[0]->key > x) {
	  // Right zig zig
	  p = rotate_R(p);
	  if (p->child[0] == NULL) {
	    break;
	  }
	}
	rp->child[0] = p;
	rp = p;
	p = p->child[0];
      }
      else {
	// p is smaller than x. So p will be left branch (lp->child[0])
	if (p->child[1] == NULL) {
	  break;
	}
	if (p->child[1]->key < x) {
	  // Left zig zig
	  p = rotate_L(p);
	  if (p->child[1] == NULL) {
	    break;
	  }
	}
	lp->child[1] = p;
	lp = p;
	p = p->child[1];
      }
    }

    rp->child[0] = p->child[1];
    lp->child[1] = p->child[0];
    p->child[0] = w.child[1];
    p->child[1] = w.child[0];
    return p;
  }

  bool find(keyT k)
  {
    root = splay(root, k);
    if (root->key == k) {
      return true;
    }
    else {
      return false;
    }
  }

  bool find2(keyT k)
  {
    if (root == NULL) {
      return false;
    }
    bool ret = false;

    Node* p = root;
    while (p) {
      if (p->key == k) {
	ret = true;
	break;
      }
      if (p->key > k) {
	p = p->child[0];
      }
      else {
	p = p->child[1];
      }
    }
    return ret;
  }

  bool insert(keyT k)
  {
    if (root == NULL) {
      root = new Node(k);
      return true;
    }

    Node* p = root;
    Node** pp = NULL;
    while (p) {
      if (p->key == k) {
	return false;
      }
      if (p->key > k) {
	pp = &(p->child[0]);
	p = p->child[0];
      }
      else {
	pp = &(p->child[1]);
	p = p->child[1];
      }
    }
    *pp = new Node(k);
    return true;
  }

  bool erase(keyT k)
  {
    if (root == NULL) {
      return false;
    }
    Node* p = root;
    Node** pp = &root;
    while (p) {
      if (p->key == k) {
	break;
      }
      if (p->key > k) {
	pp = &(p->child[0]);
	p = p->child[0];
      }
      else {
	pp = &(p->child[1]);
	p = p->child[1];
      }
    }

    if (p == NULL) {
      return false;
    }

    if (p->child[0] == NULL) {
      *pp = p->child[1];
    }
    else if (p->child[1] == NULL) {
      *pp = p->child[0];
    }
    else {
      Node* leftMax = extractLeftMax(p);
      leftMax->child[0] = p->child[0];
      leftMax->child[1] = p->child[1];
      *pp = leftMax;
    }
    delete p;
    return true;
  }

  Node* extractLeftMax(Node* p)
  {
    // child[0] and child[1] are not NULL
    Node* tgt = p->child[0];
    if (tgt->child[1] == NULL) {
      p->child[0] = tgt->child[0];
      return tgt;
    }

    Node** ptgt = NULL;
    while (tgt->child[1]) {
      ptgt = &(tgt->child[1]);
      tgt = tgt->child[1];
    }
    *ptgt = tgt->child[0];
    return tgt;
  }

  void show() const
  {
    cout << "###" << endl;
    show(root, 0);
  }

  void show(Node* t, int n) const
  {
    if (t) {
      show(t->child[0], n+1);
      for (int i = 0; i < n; ++i) {
	cout << " ";
      }
      cout << t->key << endl;
      show(t->child[1], n+1);
    }
  }
};

int
main(int ac, char **av)
{
  const int N = 1000;

  Tree t;
  for (int i = 0; i < N; ++i) {
    t.insert(rand() % N);
  }
  t.show();

  for (int i = 0; i < N; ++i) {
    t.find(rand() % N);
  }
  t.show();

  for (int i = 0; i < N; ++i) {
    t.erase(i);
  }
  t.show();

  return 0;
}
