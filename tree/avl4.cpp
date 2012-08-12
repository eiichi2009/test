#include <iostream>
#include <cstdlib>

#define FALSE 0
#define TRUE 1

using namespace std;

struct Node
{
  int data ;
  int balfact ;
  Node* left ;
  Node* right ;
};

class Tree
{
private:
  Node* root ;

public:
  Tree();
  ~Tree();

  Node* insert(int data, int* h);
  void display(Node* root);
  void setroot (Node* avl);
  Node* deldata(Node* root, int data, int* h);
  static Node* buildtree(Node* root, int data, int* h);
  static Node* del (Node* node, Node* root, int* h);
  static Node* balright (Node* root, int* h);
  static Node* balleft (Node* root, int* h);
  static void deltree (Node* root);
};

Tree::Tree(): root(NULL)
{
}

Node* Tree::insert(int data, int* h)
{
  root = buildtree(root, data, h);
  return root;
}

Node* Tree::buildtree(Node* root, int data, int* h)
{
  Node* node1;
  Node* node2;
 
  if (root == NULL) {
    root = new Node;
    root->data = data;
    root->left = NULL;
    root->right = NULL;
    root->balfact = 0;
    *h = TRUE;
    return(root);
  }

  if (data < root->data) {
    root->left = buildtree(root->left, data, h);
 
    // If left subtree is higher
    if (*h) {
      switch(root->balfact) {
      case 1:
	node1 = root->left;
	if (node1->balfact == 1) {
	  cout << "\nRight rotation.";
	  root->left = node1->right;
	  node1->right = root;
	  root->balfact = 0;
	  root = node1;
	}
	else {
	  cout << "\nDouble rotation, left then right.";
	  node2 = node1->right;
	  node1->right = node2->left;
	  node2->left = node1;
	  root->left = node2->right;
	  node2->right = root;
	  if (node2->balfact == 1) {
	    root->balfact = -1;
	  }
	  else {
	    root->balfact = 0;
	  }
	  if (node2->balfact == -1) {
	    node1->balfact = 1;
	  }
	  else {
	    node1->balfact = 0;
	  }
	  root = node2;
	}
	root->balfact = 0;
	*h = FALSE;
	break;
 
      case 0:
	root->balfact = 1;
	break;
      case -1:
	root->balfact = 0;
	*h = FALSE;
      }
    }
  }
 
  if (data > root->data) {
    root->right = buildtree(root->right, data, h);
 
    if (*h) {
      switch(root->balfact) {
      case 1:
	root->balfact = 0;
	*h = FALSE;
	break;
      case 0:
	root->balfact = -1;
	break;
      case -1:
	node1 = root->right;
	if (node1->balfact == -1) {
	  cout << "\nLeft rotation.";
	  root->right = node1->left;
	  node1->left = root;
	  root->balfact = 0;
	  root = node1;
	}
	else {
	  cout << "\nDouble rotation, right then left.";
	  node2 = node1->left;
	  node1->left = node2->right;
	  node2->right = node1;
	  root->right = node2->left;
	  node2->left = root;
	  if (node2->balfact == -1) {
	    root->balfact = 1;
	  }
	  else {
	    root->balfact = 0;
	  }
	  if (node2->balfact == 1) {
	    node1->balfact = -1;
	  }
	  else {
	    node1->balfact = 0;
	  }
	  root = node2;
	}
	root->balfact = 0;
	*h = FALSE;
      }
    }
  }
  return(root);
}

void Tree::display(Node* root)
{
  if (root != NULL) {
    display(root->left);
    cout << root->data << "\t";
    display(root->right);
  }
}

Node* Tree::deldata(Node* root, int data, int* h)
{
  Node* node;
  if (root == NULL) {
    return(root);
  }
  else {
    if (data < root->data) {
      root->left = deldata(root->left, data, h);
      if (*h) {
	root = balright(root, h);
      }
    }
    else {
      if (data > root->data) {
	root->right = deldata(root->right, data, h);
	if (*h) {
	  root = balleft(root, h);
	}
      }
      else {
	node = root;
	if (node->right == NULL) {
	  root = node->left;
	  *h = TRUE;
	  delete(node);
	}
	else {
	  if (node->left == NULL) {
	    root = node->right;
	    *h = TRUE;
	    delete(node);
	  }
	  else {
	    node->right = del(node->right, node, h);
	    if (*h) {
	      root = balleft(root, h);
	    }
	  }
	}
      }
    }
  }
  return root;
}

Node* Tree::del(Node* succ, Node* node, int* h)
{
  Node* temp = succ;
 
  if (succ->left != NULL) {
    succ->left = del(succ->left, node, h);
    if (*h) {
      succ = balright(succ, h);
    }
  }
  else {
    temp = succ;
    node->data = succ->data;
    succ = succ->right;
    delete(temp);
    *h = TRUE;
  }
  return succ;
}

Node* Tree::balright(Node* root, int* h)
{
  Node* temp1;
  Node* temp2;
  switch (root->balfact) {
  case 1:
    root->balfact = 0;
    break;
  case 0:
    root->balfact = -1;
    *h = FALSE;
    break;
  case -1:
    temp1 = root->right;
    if (temp1->balfact <= 0) {
      cout << "\nLeft rotation.";
      root->right = temp1->left;
      temp1->left = root;
      if (temp1->balfact == 0) {
	root->balfact = -1;
	temp1->balfact = 1;
	*h = FALSE;
      }
      else {
	root->balfact = temp1->balfact = 0;
      }
      root = temp1;
    }
    else {
      cout << "\nDouble rotation, right then left.";
      temp2 = temp1->left;
      temp1->left = temp2->right;
      temp2->right = temp1;
      root->right = temp2->left;
      temp2->left = root;
      if (temp2->balfact == -1) {
	root->balfact = 1;
      }
      else {
	root->balfact = 0;
      }
      if (temp2->balfact == 1) {
	temp1->balfact = -1;
      }
      else {
	temp1->balfact = 0;
      }
      root = temp2;
      temp2->balfact = 0;
    }
  }
  return root;
}

Node* Tree::balleft(Node* root, int* h)
{
  Node* temp1,* temp2;
  switch (root->balfact) {
  case -1:
    root->balfact = 0;
    break;
 
  case 0:
    root->balfact = 1;
    *h = FALSE;
    break;
 
  case 1:
    temp1 = root->left;
    if (temp1->balfact >= 0) {
      cout << "\nRight rotation.";
      root->left = temp1->right;
      temp1->right = root;
 
      if (temp1->balfact == 0) {
	root->balfact = 1;
	temp1->balfact = -1;
	*h = FALSE;
      }
      else {
	root->balfact = temp1->balfact = 0;
      }
      root = temp1;
    }
    else {
      cout << "\nDouble rotation, left then right.";
      temp2 = temp1->right;
      temp1->right = temp2->left;
      temp2->left = temp1;
      root->left = temp2->right;
      temp2->right = root;
      if (temp2->balfact == 1) {
	root->balfact = -1;
      }
      else {
	root->balfact = 0;
      }
      if (temp2-> balfact == -1) {
	temp1->balfact = 1;
      }
      else {
	temp1->balfact = 0;
      }
      root = temp2;
      temp2->balfact = 0;
    }
  }
  return root;
}

void Tree::setroot(Node* avl)
{
  root = avl;
}

Tree::~Tree()
{
  deltree(root);
}

void Tree::deltree(Node* root)
{
  if (root != NULL) {
    deltree(root->left);
    deltree(root->right);
  }
  delete(root);
}

int
main()
{
  Tree at;
  Node* avl = NULL;
  int h;
  avl = at.insert(20, &h);
  at.setroot(avl);
  avl = at.insert(6, &h);
  at.setroot(avl);
  avl = at.insert(29, &h);
  at.setroot(avl);
  avl = at.insert(5, &h);
  at.setroot(avl);
  avl = at.insert(12, &h);
  at.setroot(avl);
  avl = at.insert(25, &h);
  at.setroot(avl);
  avl = at.insert(32, &h);
  at.setroot(avl);
  avl = at.insert(10, &h);
  at.setroot(avl);
  avl = at.insert(15, &h);
  at.setroot(avl);
  avl = at.insert(27, &h);
  at.setroot(avl);
  avl = at.insert(13, &h);
  at.setroot(avl);
  cout << endl << "AVL tree:\n";
  at.display(avl);
  avl = at.deldata(avl, 20, &h);
  at.setroot(avl);
  avl = at.deldata(avl, 12, &h);
  at.setroot(avl);
  cout << endl << "AVL tree after deletion of a node:\n";
  at.display(avl);

  cout << endl;
  return 0;
}
