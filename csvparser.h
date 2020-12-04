#ifndef csvparser_h
#define csvparser_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "kdtree.h"

#define DELIMITER ","                    /* Information separator */

#define ABNORMAL_INDICATOR '"'           /* Abnormal string format indicator
                                            - Use to indicate presence of
                                              '"' and ',' in string */

#define CENSUS_YR 0
#define BLOCK_ID 1
#define PROPERTY_ID 2
#define BASE_PROP_ID 3
#define CITY_AREA_NAME 4
#define TRADING_NAME 5
#define INDUSTRY_CODE 6
#define INDUSTRY_DESC 7
#define X_COORDINATE 8
#define Y_COORDINATE 9
#define LOCATION 10                      /* list of field orders */

/* Contains information of each record */
typedef struct {
    int census_yr, block_id, property_id, base_prop_id, industry_code;
    double coordinates[2];
    char *trade_name;
    char *location;
    char *city_area_name;
    char *industry_desc;
} record_t;

/* Function prototypes */
char* read_and_parse(FILE *file, tree_t *tree);
void field_match(char *token, int field, record_t *record);
char* check_and_correct(char *token);
void char_swap(char *s1, char *s2);
void remove_dupe_quote(char *string);
char *clean_field(char *field);

#endif /* csvparser_h */
