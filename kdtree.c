/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* This is a KD Tree implementation adapted from binary search tree           *
* implementation by Alistair Moffat                                          *
* Title: Treeops.c                                                           *
* Author: Alistair Moffat                                                    *
* Modified by: Oliver Ming Hui Tan                                           *
* Available at: https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/treeops.c *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "kdtree.h"
#include "csvparser.h"

/* Create an empty KD Tree */
tree_t
*make_empty_tree(void) {
	tree_t *tree;
	tree = malloc(sizeof(*tree));
	assert(tree != NULL);
    
	/* Initialize tree to empty */
	tree->root = NULL;
    
	return tree;
}

static node_t *recursive_insert(node_t *root, node_t *new, unsigned depth);

/* Recursively insert node to the left or right child of the current node */
static node_t
*recursive_insert(node_t *root, node_t *new, unsigned depth) {
	/* If position is empty insert here */
    if (root == NULL) {
		return new;
	}
    
    record_t *root_data = (root->data)->data;
    record_t *new_data = (new->data)->data;
    /* Level indicates the dimension to compare based on the current
        depth of the node */
    unsigned level = depth % DIMENSION;
    /* Compares the position of the coordinates and the key coordinates
        based on the level of the node in the tree */
    double dim_dist = (new_data->coordinates)[level] - 
                        (root_data->coordinates)[level];
    
    if (dim_dist < 0) {
        /* Current node is less than root node based on the current level 
            so insert to left child of root node */
        root->left = recursive_insert(root->left, new, depth + 1);
        
    } else if (dim_dist > 0) {
        /* Otherwise insert to right child of root node */
        root->rght = recursive_insert(root->rght, new, depth + 1);
    
    } else {
        /* If the other coordinate is the same, it is a duplicate 
            coordinate */
        if (((new_data->coordinates)[1 - level] - 
              (root_data->coordinates)[1 - level]) < EPSILON) {
            
            /* Insert duplicate coordinate as linkedlist (as stack) */
            linknode_t *tmp = root->data;
            (new->data)->next = tmp;
            root->data = new->data;
            /* The new KD node created is no longer needed since the new 
                node is being inserted as linked list node, so free it */
            free(new);

        } else {
            /* Otherwise by convention insert to the right child of 
                root node */
            root->rght = recursive_insert(root->rght, new, depth + 1);
        }
    }
    
	return root;
}

/* Returns a pointer to an altered tree that now includes
   the object "value" in its correct location. */
tree_t
*insert_in_order(tree_t *tree, linknode_t *value) {
	node_t *new;
	/* make the new node */
	new = malloc(sizeof(*new));
	assert(tree != NULL && new != NULL);
    
    /* Record information into the node */
	new->data = value;
	new->left = new->rght = NULL;
    
	/* and insert it into the tree */
	tree->root = recursive_insert(tree->root, new, 0);
    
	return tree;
}
