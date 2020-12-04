/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* This is the program that obtains input key and searches informations       *
* from the CLUE dataset then output the result in the outputfile specified   *
* by user                                                                    *
* Developed by: Oliver Ming Hui Tan                                          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "search.h"

/* Search the dictionary based on the key coordinates given and output the 
    results into the output file specified by the user */
int
search_coordinate(tree_t *tree, const char* outputfile, char **key) {
    double *search_coordinates;
    int num_cmp;
    
    if ((search_coordinates = get_coordinate(key)) != NULL) {
        /* Traverse the KD tree to search for matching key strings */
        num_cmp = traverse_search_tree(tree, *key, search_coordinates, 
                    outputfile);
        
        /* Add a newline after searching a key */
        FILE *fp = fopen(outputfile, "a");
        if (!fp) {
            fprintf(stderr, "Error appending to file '%s'\n", outputfile);
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "\n");
        fclose(fp);
        
        free(search_coordinates);
        return num_cmp;
    }
    
    return 0;
}

/* Obtain the key coordinates input by the user and clean them */
double
*get_coordinate(char **key) {
    char *search_key = NULL;
    size_t lineBufferLength = 0;
    /* Flag to check if a line is read */
    ssize_t read_flag;
    
    if ((read_flag = getline(&search_key, &lineBufferLength, stdin)) == -1) {
        /* No key is found */
        free(search_key);
        return NULL;
    }
    
    /* Create a duplicate string of search key for output later */
    *key = duplicate_string(search_key);
    
    double *search_coordinates = (double *) malloc(sizeof(double) * DIMENSION);
    assert(search_coordinates != NULL);
    
    int i = 0, j = 0;
    char *tmp = search_key;
    
    /* Tokenize the search coordinate using <space> as delimiter */
    while ((search_key[j] != ' ')) {
        j++;
    }
    search_key[j] = '\0';
    /* Convert the coordinate read into float */
    search_coordinates[0] = atof(search_key);
    
    /* Proceed to the next (aka the last) coordinate */
    i = j + 1;
    search_key += i;

    while (search_key[j] != '\n' && search_key[j] != '\0' 
            && search_key[j] != EOF) {
        j++;
    }
    search_key[j] = '\0';
    search_coordinates[1] = atof(search_key);
    
    free(tmp);
    return search_coordinates;
}

/* Search the dictionary based on the coordinate and radius given and 
   output the results into the output file specified by the user */
int
search_coordinate_radius(tree_t *tree, const char* outputfile, char **key) {
    double *search_coordinate;
    double radius;
    int num_cmp;
    
    if ((search_coordinate = get_coordinate_radius(&radius, key)) != NULL) {
        /* Traverse the KD Tree to search for matching key strings */
        num_cmp = traverse_radius_search(tree, search_coordinate, *key, radius, 
                                outputfile);
        
        /* Add a newline in the output file after searching a key */
        FILE *fp = fopen(outputfile, "a");
        if(!fp) {
            fprintf(stderr, "Error appending to file '%s'\n", outputfile);
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "\n");
        fclose(fp);
        
        free(search_coordinate);
        return num_cmp;
    }
    return 0;
}

/* Obtain the coordinates and radius input by the user and clean them */
double
*get_coordinate_radius(double *radius, char **key) {
    char *search_key = NULL;
    size_t lineBufferLength = 0;
    /* Flag to check if a line is read */
    ssize_t read_flag;
    
    if ((read_flag = getline(&search_key, &lineBufferLength, stdin)) == -1) {
        /* No key is found */
        free(search_key);
        return NULL;
    }
    
    /* Create a duplicate string of search key for output later */
    *key = duplicate_string(search_key);
    
    double *search_coordinates = (double *) malloc(sizeof(double) * DIMENSION);
    assert(search_coordinates != NULL);
    
    int i = 0, j = 0;
    int coordinate_recorded = 0;
    int curr_coordinate = 0;
    /* Temporary pointer to the search key to be freed later */
    char *tmp = search_key;
    
    /* Tokenize using <space> as delimiter and record the coordinates given
        as float */
    while (coordinate_recorded < DIMENSION) {
        while (search_key[j] != ' ') {
            j++;
        }
        search_key[j] = '\0';
        search_coordinates[curr_coordinate++] = atof(search_key);
        
        i = j + 1;
        search_key += i;
        
        coordinate_recorded++;
    }
    
    /* Record the last input as the radius */
    *radius = atof(search_key);

    free(tmp);
    
    return search_coordinates;
}

/* Traverse the KD tree and find the nearest point to the given input 
    coordinate */
int
traverse_search_tree(tree_t *tree, char *key, double *coordinates, 
                     const char *outputfile) {
	assert(tree != NULL);
    int num_cmp = 0;
    node_t *root = tree->root;
    double *root_coordinates = ((record_t*)((root->data)->data))->coordinates;
    /* Initialise the nearest distance with the distance between the key
        coordinate and the root coordinate */
    double nearest_dist = calc_dist(root_coordinates[0], root_coordinates[1],
                                    coordinates[0], coordinates[1]);
    node_t *nearest_node = NULL;
    
	recursive_traverse_search(tree->root, coordinates, &nearest_dist,
                              &nearest_node, &num_cmp, 0);
    
    /* Traverse the linked-list if theres any in the node and print
        all the stores at the coordinate */
    if (nearest_node != NULL) {
        linknode_t *curr = nearest_node->data;
        while (curr != NULL) {
            append_output(outputfile, curr, key);
            curr = curr->next;
        }
    }
    return num_cmp;
}

/* Recursively traverse the KD tree to find the nearest point to the key 
    coordinate */
void
recursive_traverse_search(node_t *root, double *key_coordinate, 
                          double *nearest_dist, node_t **nearest_node, 
                          int *num_cmp, unsigned depth) {
	if (root) {
        *num_cmp += 1;
        
        double *coordinates = ((record_t*)((root->data)->data))->coordinates;
        double eud_dist = calc_dist(coordinates[0], coordinates[1], 
                                    key_coordinate[0], key_coordinate[1]);
        /* Level indicates the dimension to compare based on the current
            depth of the node */
        unsigned level = depth % DIMENSION;
        /* Compares the position of the coordinates and the key coordinates
            based on the level of the node in the tree */
        double dim_dist = coordinates[level] - key_coordinate[level];
        
        if (eud_dist <= *nearest_dist) {
            *nearest_dist = eud_dist;
            *nearest_node = root;
        }
        
        if (dim_dist > 0) {
            /* Positive dim_dist indicates the current coordinate is to the 
                right of the key coordinate so search the left child instead */
            recursive_traverse_search(root->left, key_coordinate, nearest_dist,
                                      nearest_node, num_cmp, depth + 1);
            
            if (fabs(dim_dist) < *nearest_dist) {
                /* However, if the current coordinate lies inside the radius of
                    the nearest distance, search right child as well */
                recursive_traverse_search(root->rght, key_coordinate, 
                                          nearest_dist, nearest_node, 
                                          num_cmp, depth + 1);
            }
        } else {
            /* Similar to the previous case but in opposite direction */
            recursive_traverse_search(root->rght, key_coordinate, nearest_dist,
                                      nearest_node, num_cmp, depth + 1);
            
            if (fabs(dim_dist) < *nearest_dist) {
                recursive_traverse_search(root->left, key_coordinate, 
                                          nearest_dist, nearest_node,
                                          num_cmp, depth + 1);
            }
        }
    }
}

/* Traverse the KD tree and find points within the radius of the given input 
    coordinate */
int
traverse_radius_search(tree_t *tree, double *coordinates, char *key, 
                       double radius, const char *outputfile) {
	assert(tree != NULL);
    int num_cmp = 0;
    /* Flag to indicate if any points are found */
    int found_flag = 0;
    
	num_cmp += recursive_radius_search(tree->root, coordinates, key, radius,
                                          &found_flag, outputfile, 0);
    
    if (found_flag == 0) {
        append_radius_fail(outputfile, key);
    }
    
    return num_cmp;
}

/* Recursively traverse the KD tree to find points within radius distance to
    the key coordinate */
int
recursive_radius_search(node_t *root, double *key_coordinate, char *key, 
                        double radius, int *found_flag, const char *outputfile, 
                        unsigned depth) {
	if (root) {
        double *coordinates = ((record_t*)((root->data)->data))->coordinates;
        double eud_dist = calc_dist(coordinates[0], coordinates[1],
                                    key_coordinate[0], key_coordinate[1]);
        /* Level indicates the dimension to compare based on the current
            depth of the node */
        unsigned level = depth % DIMENSION;
        /* Compares the position of the coordinates and the key coordinates
            based on the level of the node in the tree */
        double dim_dist = coordinates[level] - key_coordinate[level];
                                    
        if (eud_dist <= radius) {
            append_radius_output(outputfile, root, key);
            *found_flag += 1;
        }
        
        /* If current node lies inside the radius, search both child of
            the node */
        if (fabs(dim_dist) <= radius) {
            return recursive_radius_search(root->left, key_coordinate, key,
                                           radius, found_flag, outputfile, 
                                           depth + 1) +
                recursive_radius_search(root->rght, key_coordinate, key, 
                                        radius, found_flag, outputfile, 
                                        depth + 1) + 1;
            
        } else if (dim_dist > 0) {
            /* Otherwise check if the node lies to the right of the key
                coordinate, if so search left child instead */
            return recursive_radius_search(root->left, key_coordinate, key,
                                           radius, found_flag, outputfile, 
                                           depth + 1) + 1;
            
        } else {
            /* If not, search right child */
            return recursive_radius_search(root->rght, key_coordinate, key,
                                           radius, found_flag, outputfile,
                                           depth + 1) + 1;
        }
        
    }
    return 0;
}


/* Calculate the euclidean distance between two points on a cartesian plane */
double calc_dist(double root_x, double root_y, double key_x, double key_y) {
    double dist;
    dist = sqrt(pow(root_x - key_x, 2) + pow(root_y - key_y, 2));
    return dist;
}

/* Append the information of the nearest point to key coordinate into the 
    outputfile */
void 
append_output(const char *outputfile, linknode_t *node, char *key) {
    FILE *fp = fopen(outputfile, "a");
    if (!fp) {
        fprintf(stderr, "Error appending to file '%s'\n", outputfile);
        exit(EXIT_FAILURE);
    }
    
    linknode_t *curr = node;
    fprintf(fp, "%s --> Census year: %d || Block ID: %d || Property ID: %d "
                "|| Base property ID: %d || CLUE small area: %s || "
                "Trading Name: %s || Industry (ANZSIC4) code: %d || "
                "Industry (ANZSIC4) description: %s || "
                "x coordinate: %.4lf || y coordinate: %.4lf || "
                "Location: %s || \n",
            key, ((record_t*)(curr->data))->census_yr,
            ((record_t*)(curr->data))->block_id,
            ((record_t*)(curr->data))->property_id,
            ((record_t*)(curr->data))->base_prop_id,
            ((record_t*)(curr->data))->city_area_name,
            ((record_t*)(curr->data))->trade_name,
            ((record_t*)(curr->data))->industry_code,
            ((record_t*)(curr->data))->industry_desc,
            (((record_t*)(curr->data))->coordinates)[0],
            (((record_t*)(curr->data))->coordinates)[1],
            ((record_t*)(curr->data))->location);
    
    fflush(fp);
    fclose(fp);
}

/* Append the information of the points within radius distance from key 
    coordinate into the outputfile */
void 
append_radius_output(const char *outputfile, node_t *node, char *key) {
    FILE *fp = fopen(outputfile, "a");
    if (!fp) {
        fprintf(stderr, "Error appending to file '%s'\n", outputfile);
        exit(EXIT_FAILURE);
    }
    
    linknode_t *curr = node->data;
    while (curr != NULL) {
        fprintf(fp, "%s --> Census year: %d || Block ID: %d || "
                    "Property ID: %d || Base property ID: %d || "
                    "CLUE small area: %s || Trading Name: %s || " 
                    "Industry (ANZSIC4) code: %d || "
                    "Industry (ANZSIC4) description: %s || "
                    "x coordinate: %.4lf || y coordinate: %.4lf || "
                    "Location: %s || \n",
                key, ((record_t*)(curr->data))->census_yr,
                ((record_t*)(curr->data))->block_id,
                ((record_t*)(curr->data))->property_id,
                ((record_t*)(curr->data))->base_prop_id,
                ((record_t*)(curr->data))->city_area_name,
                ((record_t*)(curr->data))->trade_name,
                ((record_t*)(curr->data))->industry_code,
                ((record_t*)(curr->data))->industry_desc,
                (((record_t*)(curr->data))->coordinates)[0],
                (((record_t*)(curr->data))->coordinates)[1],
                ((record_t*)(curr->data))->location);
        
        curr = curr->next;
    }
    
    fflush(fp);
    fclose(fp);
}

/* Append the failed search result into the outputfile */
void 
append_radius_fail(const char *outputfile, char *key) {
    FILE *fp = fopen(outputfile, "a");
    if (!fp) {
        fprintf(stderr, "Error appending to file '%s'\n", outputfile);
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "%s --> NOTFOUND\n", key);
    
    fflush(fp);
    fclose(fp);
}

/* Create a duplicate string without newline */
char
*duplicate_string(char *src) {
    char *dupe_str = (char *) malloc(sizeof(char) * (strlen(src) + 1));
    assert(dupe_str != NULL);
    
    strcpy(dupe_str, src);
    
    /* Replace newline with end string at the end of string */
    int i = 0;
    while (dupe_str[i] != '\0' && dupe_str[i] != '\r' && dupe_str[i] != '\n') {
        i++;
    }
    dupe_str[i] = '\0';
    
    return dupe_str;
}
