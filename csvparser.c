/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* This is a CSV Parser program that parses and records informations in the   *
* CLUE Dataset to a KD Tree data structure                                   *
* Developed by: Oliver Ming Hui Tan                                          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "csvparser.h"

/* Read the csv and record each row of information into a KD Tree. 
   User is responsible to free the return pointer of this function after
   use */
char
*read_and_parse(FILE *file, tree_t *tree) {
    char *line = NULL;
    size_t lineBufferLength = 0;
    /* Flag to check if a line is read */
    ssize_t read_flag = 0;
    /* Indicate the field order to parse the records */
    int field = 0;
    char *token;
    
    /* Skips header line */
    read_flag = getline(&line, &lineBufferLength, file);
    
    /* Read and record each row of information into the KD Tree */
    while((read_flag = getline(&line, &lineBufferLength, file)) != -1){
        /* Reset field order after each row */
        field = 0;
        /* Separate information via tokenisation method */
        token = strtok(line, DELIMITER);
        
        record_t *new_record = (record_t *) malloc(sizeof(record_t));
        assert(new_record != NULL);
        
        /* Walk through tokens and match each token to their respective
           field */
        while(token != NULL) {
            field_match(token, field, new_record);
            
            /* Point the token to the next information to be recorded */
            token = strtok(NULL, DELIMITER);
            field++;
        }
        
        /* Insert each data as a linked-list node */
        linknode_t *new_node = (linknode_t *) malloc(sizeof(linknode_t));
        assert(new_node != NULL);
        new_node->data = new_record;
        new_node->next = NULL;
        /* Insert the linked-list node as a data of a node in the KD Tree */
        tree = insert_in_order(tree, new_node);
        
    }
    
    return line;
}

/* Match and record each information according to their respective field
   orders */
void
field_match(char *token, int field, record_t *record) {
    if (field == CENSUS_YR) {
        record->census_yr = atoi(token);
        
    } else if (field == BLOCK_ID) {
        record->block_id = atoi(token);
 
    } else if (field == PROPERTY_ID) {
        record->property_id = atoi(token);
        
    } else if (field == BASE_PROP_ID) {
        record->base_prop_id = atoi(token);
        
    } else if (field == CITY_AREA_NAME) {
        /* Check the string before recording the information */
        char *info = check_and_correct(token);
        record->city_area_name = (char*) malloc(sizeof(char) * (strlen(info) + 1));
        assert(record->city_area_name != NULL);
        strcpy(record->city_area_name, info);
        
    } else if (field == TRADING_NAME) {
        char *info = check_and_correct(token);
        record->trade_name = (char*) malloc(sizeof(char) * (strlen(info) + 1));
        assert(record->trade_name != NULL);
        strcpy(record->trade_name, info);
        
    } else if (field == INDUSTRY_CODE) {
        record->industry_code = atoi(token);
        
    } else if (field == INDUSTRY_DESC) {
        char *info = check_and_correct(token);
        record->industry_desc = (char*) malloc(sizeof(char) * (strlen(info) + 1));
        assert(record->industry_desc != NULL);
        strcpy(record->industry_desc, info);
        
    } else if (field == X_COORDINATE) {
        (record->coordinates)[0] = atof(token);
        
    } else if (field == Y_COORDINATE) {
        (record->coordinates)[1] = atof(token);
        
    } else {
        char *info = check_and_correct(token);
        record->location = (char*) malloc(sizeof(char) * (strlen(info) + 1));
        assert(record->location != NULL);
        strcpy(record->location, info);
    }
}

/* Check if the string contains delimiter and return the corrected string to
   be recorded into its field */
char
*check_and_correct(char *token) {
    /* Check if token starts with abnormal-string indicator (") */
    if (token[0] != ABNORMAL_INDICATOR) {
        return token;
    }
    
    /* Skips to the end of the token */
    int i = 0;
    while (token[i] != '\0' && token[i] != '\n' && token[i] != '\r' && token[i] != EOF) {
        i++;
    }
    
    /* Check if token ends with abnormal-string indicator (") */
    if (token[i - 1] == ABNORMAL_INDICATOR) {
        /* If so, that means the information for the field is complete.
           Clean and return the token to be recorded */
        remove_dupe_quote(token);
        char* clean_token = clean_field(token);
        
        return clean_token;
        
    } else {
        /* Otherwise, continue to concatenate the token with the next token
           until the information for the field is complete */
        
        /* Keep the pointer to the original token */
        char *info = token;
        
        char *prev_tok = token;
        while ((token = strtok(NULL, DELIMITER)) != NULL) {
            /* Change the end-string of the previous token back to the
               delimiter (,) to concatenate the previous token with the
               current token */
            prev_tok[strlen(prev_tok)] = ',';
            
            /* Again, check if the current token ends with abnormal-string
               indicator */
            if (token[strlen(token) - 1] == ABNORMAL_INDICATOR) {
                break;
            }
            
            prev_tok = token;
        }
        remove_dupe_quote(info);
        
        return clean_field(info);
    }
}

/* Remove abnormal-string indicator (") at the start and end of the string
   (if present) and return the clean string */
char 
*clean_field(char *field) {
    int i = 0;
    
    /* Skip the first abnormal-string indicator */
    char* new_string = &field[1];
    
    /* Skip to the end of the string */
    while (new_string[i] != '\0' && new_string[i] != '\r' 
            && new_string[i] != '\n') {
        i++;
    }
    
    /* From the end of the string, find the last abnormal-string indicator and
       replace it with end string */
    for (int j = i; j >= 0; j--) {
        if (new_string[j] == ABNORMAL_INDICATOR) {
            new_string[j] = '\0';
            break;
        }
    }
    
    return new_string;
}

/* Remove duplicate quotation marks in the string */
void
remove_dupe_quote(char *string) {
    int i = 0, j = 0, dupe_count = 0;
    
    /* Check for duplicate quotation marks and move them to the end of the
       string */
    for(i = 1; string[i] != '\0' && string[i] != '\r' 
            && string[i] != '\n'; i++) {
        if (string[i] == '"' && string[i - 1] == '"') {
            dupe_count++;
            for (j = i; string[j + 1] != '\0' && string[j + 1] != '\r' 
                && string[j + 1] != '\n'; j++) {
                char_swap(&string[j], &string[j + 1]);
            }
        }
    }
    
    /* Remove the duplicate quotation marks at the end of the string */
    string[strlen(string) - (dupe_count / 2)] = '\0';
}

/* Swap positions of the two characters */
void
char_swap(char *s1, char *s2) {
    char tmp;
    tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

