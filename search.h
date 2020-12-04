#ifndef search_h
#define search_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "kdtree.h"
#include "csvparser.h"

/* prototypes for the functions in this library */
int search_coordinate(tree_t *tree, const char* outputfile, char **key);
double *get_coordinate(char **key);
int search_coordinate_radius(tree_t *tree, const char* outputfile, char **key);
double *get_coordinate_radius(double *radius, char **key);
int traverse_search_tree(tree_t *tree, char *key, double *coordinates,
                          const char *outputfile);
void recursive_traverse_search(node_t *root, double *key_coordinates, 
                               double *min_diff, node_t **min_diff_found, 
                               int *num_cmp, unsigned depth);
int traverse_radius_search(tree_t *tree, double *coordinates, char *key, 
                            double radius, const char *outputfile);
int recursive_radius_search(node_t *root, double *key_coordinate, char *key,
                            double radius, int *found_flag, const char *outputfile,
                            unsigned depth);
void append_output(const char *outputfile, linknode_t *node, char *key);
void append_radius_output(const char *outputfile, node_t *node, char *key);
void append_radius_fail(const char *outputfile, char *key);
double calc_dist(double root_x, double root_y, double key_x, double key_y);
char *duplicate_string(char *src);

#endif 