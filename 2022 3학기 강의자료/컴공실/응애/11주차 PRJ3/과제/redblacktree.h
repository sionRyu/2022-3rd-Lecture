#ifndef __REDBLACKTREE_H__
#define __REDBLACKTREE_H__

typedef struct _rbnode {
	// user fields for rbnode
	// comparison defined by rbnodeLess()
	int score;
	unsigned int order;
	char * name;

	// operative variables for the order statistics red-black tree
	int size; 	// size of the subtree with this node as root
	int color;	// color of this node. RB_RED=0, RB_BLACK=1.
	struct _rbnode * parent;
	struct _rbnode * left;
	struct _rbnode * right;
} rbnode;

typedef struct _rbtree {
	rbnode * root;
	int size;
	unsigned int total_order;
	rbnode null;
} rbtree;

#define RB_RED 0
#define RB_BLACK 1
#define RB_NULL 2

// create a new tree and initalize it
rbtree * rbNewTree();

// destroy a TREE
int rbDestroyTree(rbtree * tree);

// returns a new node initialized with the given SCORE and NAME
rbnode * rbNewNode(int score, char * name);

// inserts a node Z to TREE
int rbInsert(rbtree * tree, rbnode * z);

// deletes the node with rank R from TREE
int rbDelete(rbtree * tree, int r);

// get nodes of TREE from rank A to rank B
rbnode ** rbSelectByRange(rbtree * tree, int a, int b);

// get rank of the given node X
int rbRankByNode(rbtree * tree, rbnode * x);

#endif // __REDBLACKTREE_H__
