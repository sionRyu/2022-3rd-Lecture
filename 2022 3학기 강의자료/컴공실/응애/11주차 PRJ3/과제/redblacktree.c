#include "redblacktree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// static fucntions to implement red-black tree operations
// rotates the tree under ROOT at node X to the left
static int rbLeftRotate(rbtree * tree, rbnode * x);

// rotates the tree under ROOT at node X to the right
static int rbRightRotate(rbtree * tree, rbnode * x);

// replaces the subtree under U with the subtree under V
static int rbTransplant(rbtree * tree, rbnode * u, rbnode * v);

// returns the node with rank R
static rbnode * rbSelectByRank(rbnode * x, int r);

// fix TREE after insertion from Z
static int rbInsertFixUp (rbtree * tree, rbnode * z);

// fix TREE after deletion from X
static int rbDeleteFixUp (rbtree * tree, rbnode * x);

// destroy a node T
static int rbDestroyNode (rbnode * t);

// destroy all nodes under X
static int rbDestroySubTree(rbnode * x, rbnode * null);

// returns if A < B
static int rbnodeLess(rbnode * a, rbnode * b);


// red-black tree operations
rbtree * rbNewTree() {
	rbtree * t;

	t = (rbtree *)malloc(sizeof(rbtree));
	t->root = &t->null;
	t->null.parent = &t->null;
	t->null.left = &t->null;
	t->null.right = &t->null;
	t->null.score = -1;
	t->null.size = 0;
	t->null.color = RB_NULL;
	t->total_order = 0;
	t->size = 0;
	return t;
}

int rbDestroyTree(rbtree * tree) {
	if(tree != NULL) {
		rbDestroySubTree(tree->root, &tree->null);
		free(tree);
	}
	return 0;
}

rbnode * rbNewNode(int score, char *name) {
	rbnode * new_node;

	new_node = (rbnode *)malloc(sizeof(rbnode));
	new_node->score = score;
	new_node->name = (char*)malloc(sizeof(char) * (strlen(name)+1));
	strcpy(new_node->name, name);
	return new_node;
}

int rbInsert(rbtree * tree, rbnode * z) {
	rbnode * x;
	rbnode * y;

	z->order = tree->total_order++;
	z->size = 1;

	x = tree->root;
	y = &tree->null;
	while(x != &(tree->null)) {
		x->size++; // the node is going to have another descendant
		y = x;
		if(rbnodeLess(z, x)) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	z->parent = y;
	if(y == &(tree->null)) {
		tree->root = z;
	}
	else if(rbnodeLess(z, y)) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = &tree->null;
	z->right = &tree->null;
	z->color = RB_RED;
	rbInsertFixUp(tree, z);
	tree->size++;
	return 0;
}

int rbDelete(rbtree * tree, int r) {
	rbnode * x;
	rbnode * y;
	rbnode * z;
	int y_original_color;

	// assert r is a valid rank
	if(!(1 <= r && r <= tree->size)) {
		return -1;
	}
	z = rbSelectByRank(tree->root, r);

	// z is going to be deleted from tree,
	// so all ancestors of z will have one less subtree size
	y = z->parent;
	while(y != &tree->null) {
		y->size--;
		y = y->parent;
	}


	y = z;
	y_original_color = y->color;
	if(z->left == &tree->null) {
		x = z->right;
		rbTransplant(tree, z, z->right);
	}
	else if(z->right == &tree->null) {
		x = z->left;
		rbTransplant(tree, z, z->left);
	}
	else { // z has both children
		y = z->right;
		while(y->left != &tree->null) {
			y = y->left;
		}
		y_original_color = y->color;
		x = y->right;
		if(y->parent == z) {
			x->parent = y;
		}
		else {
			rbTransplant(tree, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rbTransplant(tree, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
		y->size = z->size-1;
	}
	if(y_original_color == RB_BLACK) {
		rbDeleteFixUp(tree, x);
	}

	rbDestroyNode(z);
	tree->size--;
	return 0;
}

rbnode ** rbSelectByRange(rbtree * tree, int a, int b) {
	rbnode ** nodes;
	rbnode ** stack;
	int index;
	int stack_size;

	rbnode * t;
	int i;

	// assert 1 <= a <= b <= size of tree
	if(!(1 <= a && a <= b && b <= tree->size))
		return NULL;
	
	nodes = (rbnode **)malloc(sizeof(rbnode*)*(b-a+1));
	stack = (rbnode **)malloc(sizeof(rbnode*)*(tree->size));
	index = 0;
	stack_size = 0;
	t = tree->root;
	while(t != &(tree->null)) {
		stack[stack_size++] = t;
		t = t->left;
	}
	for(i = 1; i <= b; i++) {
		t = stack[--stack_size];
		if(a <= i) {
			nodes[index++] = t;
		}
		t = t->right;
		while(t != &(tree->null)) {
			stack[stack_size++] = t;
			t = t->left;
		}
	}

	free(stack);
	return nodes;
}

int rbRankByNode(rbtree * tree, rbnode * x) {
	int r;
	rbnode * y;
	if(tree == NULL || x == NULL) {
		return -1;
	}

	r = x->left->size + 1;
	y = x;
	while(y != tree->root) {
		if(y == y->parent->right) {
			r = r + y->parent->left->size + 1;
		}
		y = y->parent;
	}
	return r;
}

/*
   for a node x and its right child y,
       x           y
	  / \         / \
	 a   y  -->  x   c
	    / \     / \
	   b   c   a   b
 */
static int rbLeftRotate(rbtree * tree, rbnode * x) {
	rbnode * y;

	// assert x has a right child
	if(x->right == &(tree->null))
		return -1;

	y = x->right;
	// adjust subtree size for node x and y=x->right
	x->size = x->left->size + y->left->size + 1;
	y->size = x->size + y->right->size + 1;

	// adjust parent-child relationship according to the diagram
	x->right = y->left;
	if(y->left != &(tree->null)) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if(x->parent == &(tree->null)) {
		tree->root = y;
	}
	else if(x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
	return 0;
}

/*
   for a node x and its left child y,
       x           y
	  / \         / \
	 y   c  -->  a   x
	/ \             / \
   a   b           b   c
 */
static int rbRightRotate(rbtree * tree, rbnode * x) {
	rbnode * y;
	
	// assert x has a left child
	if(x->left == &(tree->null))
		return -1;

	y = x->left;

	// adjust subtree size for node x and y=x->left
	x->size = y->right->size + x->right->size + 1;
	y->size = y->left->size + x->size + 1;

	x->left = y->right;
	if(y->right != &(tree->null)) {
		y->right->parent = x;
	}
	y->parent = x->parent;
	if(x->parent == &(tree->null)) {
		tree->root = y;
	}
	else if(x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->right = x;
	x->parent = y;
	return 0;
}

static int rbTransplant(rbtree * tree, rbnode * u, rbnode * v) {
	if(u->parent == &tree->null) {
		tree->root = v;
	}
	else if(u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	v->parent = u->parent;
	return 0;
}

static rbnode * rbSelectByRank(rbnode * x, int r) {
	if(x->left->size+1 == r) {
		return x;
	}
	else if(r < x->left->size+1) {
		return rbSelectByRank(x->left, r);
	}
	else {
		return rbSelectByRank(x->right, r-(x->left->size)-1);
	}
}

static int rbInsertFixUp (rbtree * tree, rbnode * z) {
	rbnode * y;

	while(z->parent->color == RB_RED) {
		if(z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;
			if(y->color == RB_RED) {
				z->parent->color = RB_BLACK;
				y->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				z = z->parent->parent;
			}
			else {
				if(z == z->parent->right) {
					z = z->parent;
					rbLeftRotate(tree, z);
				}
				z->parent->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				rbRightRotate(tree, z->parent->parent);
			}
		}
		else { // z->parent == z->parent->parent->right
			y = z->parent->parent->left;
			if(y->color == RB_RED) {
				z->parent->color = RB_BLACK;
				y->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				z = z->parent->parent;
			}
			else {
				if(z == z->parent->left) {
					z = z->parent;
					rbRightRotate(tree, z);
				}
				z->parent->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				rbLeftRotate(tree, z->parent->parent);
			}
		}
	}
	tree->root->color = RB_BLACK;
	return 0;
}

static int rbDeleteFixUp (rbtree * tree, rbnode * x) {
	rbnode * w;
	while(x != tree->root && x->color == RB_BLACK) {
		if(x == x->parent->left) {
			w = x->parent->right;
			if(w->color == RB_RED) {
				w->color = RB_BLACK;
				x->parent->color = RB_RED;
				rbLeftRotate(tree, x->parent);
				w = x->parent->right;
			}
			if(w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
				w->color = RB_RED;
				x = x->parent;
			}
			else {
				if(w->right->color == RB_BLACK) {
					w->left->color == RB_BLACK;
					w->color = RB_RED;
					rbRightRotate(tree, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = RB_BLACK;
				w->right->color = RB_BLACK;
				rbLeftRotate(tree, x->parent);
				x = tree->root;
			}
		}
		else { // x == x->parent->right
			w = x->parent->left;
			if(w->color == RB_RED) {
				w->color = RB_BLACK;
				x->parent->color = RB_RED;
				rbRightRotate(tree, x->parent);
				w = x->parent->left;
			}
			if(w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
				w->color = RB_RED;
				x = x->parent;
			}
			else {
				if(w->left->color == RB_BLACK) {
					w->right->color == RB_BLACK;
					w->color = RB_RED;
					rbLeftRotate(tree, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = RB_BLACK;
				w->left->color = RB_BLACK;
				rbRightRotate(tree, x->parent);
				x = tree->root;
			}
		}
	}
	x->color = RB_BLACK;
	return 0;
}

static int rbDestroyNode(rbnode * t) {
	if(t->name != NULL) {
		free(t->name);
	}
	if(t != NULL) {
		free(t);
	}
	return 0;
}

static int rbDestroySubTree(rbnode * x, rbnode * null) {
	if(x == null)
		return 0;
	rbDestroySubTree(x->left, null);
	rbDestroySubTree(x->right, null);
	rbDestroyNode(x);
	return 0;
}

static int rbnodeLess(rbnode * a, rbnode * b) {
	if(a->score > b->score)
		return 1;
	if(a->score < b->score)
		return 0;
	return a->order < b->order;
}
