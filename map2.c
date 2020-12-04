/*****************************************************************************
*    COMP20003 Assignment 2 (Stage 2)                                        *
*    Melbourne Census Dataset Information Retrieval using a KD Tree          *
*    (Search all points within query radius from the input coordinates)      *
*    Developed by: Oliver Ming Hui Tan                                       *                                                   *
*    Date: 18 September 2020                                                 *
*                                                                            *
******************************************************************************/

#include "csvparser.h"
#include "kdtree.h"
#include "search.h"

/* Function prototypes */
void free_all(tree_t *tree, char *buffer);
void recursive_free_tree(node_t *root);

/* Create a dictionary based on KD tree to store information read from
 * the csv file and print the information based on the key input by the user
 * into an output file specified by the user.
 *
 * To run the program type:
 * ./map2 <csv_filename> <output_filename> < <keyfile_name> 
 * 
 *      <csv_filename> arg     - Dataset file
 *      <output_filename> arg  - Output file to record search results
 *      <keyfile_name> arg     - File of keys to be searched (one 
 *                               coordinates-radius key separated by <space> 
 *                               per line) 
 */
int main(int argc, const char * argv[]) {
    const char *filename = NULL;
    const char *outputfile = NULL;
    tree_t *tree;
    char *buffer;
    
    /* Checks if filenames are given */
    if (!argv[1]) {
        fprintf(stderr, "No file read.");
        return EXIT_FAILURE;
    }
    
    if (!argv[2]) {
        fprintf(stderr, "Output file name not found.");
        return EXIT_FAILURE;
    }

    filename = argv[1];
    outputfile = argv[2];
    
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return EXIT_FAILURE;
    }
    
    tree = make_empty_tree();
    assert(tree != NULL);
    
    /* Read and store information into the KD Tree */
    buffer = read_and_parse(fp, tree);
    
    /* Search all points within the input radius of the input coordinates in
        the dictionary and print them into the outputfile */
    char *key = NULL;
    int num_cmp;
    while ((num_cmp = search_coordinate_radius(tree, outputfile, &key)) != 0) {
        /* Print the number of comparison required for each search */
        printf("%s --> %d\n", key, num_cmp);
        free(key);
    }

    free_all(tree, buffer);
    fclose(fp);
    
    return 0;
}


/* Recursively free all allocated memory along with each node in 
    the KD Tree */
void
recursive_free_tree(node_t *root) {
	if (root) {
		recursive_free_tree(root->left);
		recursive_free_tree(root->rght);
		
        /* Traverse the linked list in each node and free each linked list
           node along with its data */
        linknode_t *curr = root->data;
        linknode_t *prev;
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
            
            /* Free allocated memory used for records in the data */
            char *trade_name = ((record_t*)prev->data)->trade_name;
            free(trade_name);
            char *city_area_name = ((record_t*)prev->data)->city_area_name;
            free(city_area_name);
            char *location = ((record_t*)prev->data)->location;
            free(location);
            char *industry_desc = ((record_t*)prev->data)->industry_desc;
            free(industry_desc);
       
            free(prev->data);
            free(prev);
        }
        
        free(root);
	}

}

/* Release all memory allocated in the tree structure and the
    buffer */
void
free_all(tree_t *tree, char *buffer) {
	assert(tree != NULL);
	recursive_free_tree(tree->root);
    free(tree);
    free(buffer);
}
