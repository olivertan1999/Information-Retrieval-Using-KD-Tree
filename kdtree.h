#ifndef kdtree_h
#define kdtree_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#define EPSILON 0.0000001
#define DIMENSION 2

typedef struct lnode linknode_t;  /* node of linkedlist */

struct lnode {
    void* data;                   /* ptr to the record data */
    linknode_t *next;             /* ptr to the next node in the linked
                                     list */
};

typedef struct node node_t;       /* node of kdtree */

struct node {
    linknode_t *data;             /* ptr to stored structure */
	node_t *left;                 /* left subtree of node */
	node_t *rght;                 /* right subtree of node */
};

typedef struct {
	node_t *root;                 /* root node of the tree */
} tree_t;

/* prototypes for the functions in this library */
tree_t *make_empty_tree(void);
tree_t *insert_in_order(tree_t *tree, linknode_t *value);
void traverse_tree(tree_t *tree, void action(void*));

#endif /* kdtree_h */