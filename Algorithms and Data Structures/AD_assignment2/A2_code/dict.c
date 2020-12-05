/* dict.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
* modified 20/09/2020
*
* A program to read in data from the Business establishment trading name
* and industry classiﬁcation 2018 dataset
* and store each field in the 2d tree.
* Also search the data that have a matching key with the input value
* and output the data values.
* 
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "dict.h"
#include "inout.h"


/* store data in the newline into the 2d tree structure */
node2d_t* insert_line(char* newline, node2d_t* parent) {
    
    data_t* data = (data_t*)malloc(sizeof(data_t));
    assert(data != NULL);
    
    // buffer for each field
    char b_census[MAX_FIELD + 1];
    char b_block[MAX_FIELD + 1];
    char b_property[MAX_FIELD + 1];
    char b_base_prop[MAX_FIELD + 1];
    char b_clue[MAX_FIELD + 1];
    char b_trading[MAX_FIELD + 1];
    char b_indust_code[MAX_FIELD + 1];
    char b_indust_desc[MAX_FIELD + 1];
    char b_xcoord[MAX_FIELD + 1];
    char b_ycoord[MAX_FIELD + 1];
    char b_loc[MAX_FIELD + 1];
    
    // invalid input
    if (newline == NULL ||
       sscanf(newline, SCAN_FIELD, b_census, b_block, b_property, 
              b_base_prop, b_clue, b_trading, b_indust_code, 
              b_indust_desc, b_xcoord, b_ycoord, b_loc) != FIELD_NUM) {
        
        printf("Error: input was not valid.\n");
        exit(EXIT_FAILURE);
    }
    
    // store data in newline into buffers
    else {
        // assign memory for data and key
        data->census_year = (char*)malloc(sizeof(char) * 
                                          (strlen(b_census) + 1));
        assert(data->census_year != NULL);
        data->block_id = (char*)malloc(sizeof(char) * (strlen(b_block) + 1));
        assert(data->block_id != NULL);
        data->property_id = (char*)malloc(sizeof(char) * 
                                          (strlen(b_property) + 1));
        assert(data->property_id != NULL);
        data->base_property_id = (char*)malloc(sizeof(char) * 
                                               (strlen(b_base_prop) + 1));
        assert(data->base_property_id != NULL);
        data->clue_small_area = (char*)malloc(sizeof(char) * 
                                              (strlen(b_clue) + 1));
        assert(data->clue_small_area != NULL);
        data->trading_name = (char*)malloc(sizeof(char) * 
                                              (strlen(b_trading) + 1));
        assert(data->trading_name != NULL);
        data->industry_code = (char*)malloc(sizeof(char) * 
                                            (strlen(b_indust_code) + 1));
        assert(data->industry_code != NULL);
        data->industry_description = (char*)malloc(sizeof(char) * 
                                                   (strlen(b_indust_desc) + 1));
        assert(data->industry_description != NULL);
        data->xcoord = (char*)malloc(sizeof(char) * (strlen(b_xcoord) + 1));
        assert(data->xcoord != NULL);
        data->ycoord = (char*)malloc(sizeof(char) * (strlen(b_ycoord) + 1));
        assert(data->ycoord != NULL);
        data->location = (char*)malloc(sizeof(char) * (strlen(b_loc) + 1));
        assert(data->location != NULL);
        
        // assign key
        double* key = (double*)malloc(sizeof(double) * (KEY_SIZE));
        assert(key != NULL);
        
        // assign value
        strcpy(data->census_year, b_census);
        strcpy(data->block_id, b_block);
        strcpy(data->property_id, b_property);
        strcpy(data->base_property_id, b_base_prop);
        strcpy(data->clue_small_area, b_clue);
        strcpy(data->trading_name, b_trading);
        strcpy(data->industry_code, b_indust_code);
        strcpy(data->industry_description, b_indust_desc);
        strcpy(data->xcoord, b_xcoord);
        strcpy(data->ycoord, b_ycoord);
        strcpy(data->location, b_loc);
        key[KEY_X] = atof(data->xcoord);
        key[KEY_Y] = atof(data->ycoord);
        
        // if parent is NULL, create a root node
        if (!parent) {
            parent = (node2d_t*)malloc(sizeof(node2d_t));
            
            assert(parent != NULL);
            parent->left = NULL;
            parent->right = NULL;
            parent->key = key;
            parent->k_index = 0;
            parent->value = insert_at_foot(make_empty_list(), data);   
        }
        
        // add key and data to the 2d tree
        else {
            parent = insert_tree_node(parent, data, key);
        }
    } 
    
    return parent;
}


/* Reads all the records from the input file and
* store them in the 2d tree. */
node2d_t* create_dict(char* file_name) {
    
    FILE* fp = NULL;
    char* line = NULL;
    size_t len = 0;
    char newline[MAX_LINE + 1];
    node2d_t* parent = NULL;
    
    fp = fopen(file_name, "r");
    if (fp == NULL) {
       perror("fopen");
       exit(EXIT_FAILURE);
    }
    
    //skip header
    if (getline(&line, &len, fp) == -1) {
        fprintf(stderr, "invalid file\n");
    }
    
    // read each line from the file
    while ((getline(&line, &len, fp)) != -1) {
    
        // store the line value in more processable form
        modify_line(line, newline);
        
        // store the newline into a list
        parent = insert_line(newline, parent);
    }

    free(line);
    fclose(fp);
    return parent;   
}
