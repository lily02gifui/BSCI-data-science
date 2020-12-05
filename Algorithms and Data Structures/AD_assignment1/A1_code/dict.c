/* dict.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
*
* A program to read in data from the Business establishment trading name
* and industry classiﬁcation 2018 dataset
* and store each field in the linked list.
* Also search the data that have matching key with the input value
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


/* store data in the newline into each field in the linked list */
list_t* insert_line(char* newline, list_t* list) {
    
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
        char* key = (char*)malloc(sizeof(char) * (strlen(b_trading) + 1));
        assert(key != NULL);
        
        // assign value
        strcpy(data->census_year, b_census);
        strcpy(data->block_id, b_block);
        strcpy(data->property_id, b_property);
        strcpy(data->base_property_id, b_base_prop);
        strcpy(data->clue_small_area, b_clue);
        strcpy(data->industry_code, b_indust_code);
        strcpy(data->industry_description, b_indust_desc);
        strcpy(data->xcoord, b_xcoord);
        strcpy(data->ycoord, b_ycoord);
        strcpy(data->location, b_loc);
        strcpy(key, b_trading);
        
        // add key and data to the linked list
        list = insert_at_foot(list, data, key);
    } 
    
    return list;
}


/* Reads all the records from the input file and
* store them in the linked list. */
list_t* create_dict(char* file_name, list_t* list) {
    
    FILE* fp = NULL;
    char* line = NULL;
    size_t len = 0;
    char newline[MAX_LINE + 1];
    
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
        list = insert_line(newline, list);
    }

    free(line);
    fclose(fp);
    return list;   
}


/* Search the matching record and output the result in the provided file */
void* recursive_search_list(node_t* node, char* input_key, char* filename,
                            int found) {
    // the end of the list
    if (!node) {
        // no matching key was found from the list
        if (!found) {
            write_no_out(filename, input_key);
        }
        return NULL;
    }
    
    // found the matching data
    if (strcmp(input_key, node->key) == 0) {
        //write data into output file
        found = 1;
        write_out(filename, node->data, input_key);
    }
    
    // further search the matching record
    return recursive_search_list(node->next, input_key, filename, found);
}


/* recursively search each node in the linked list */
void* search_list(list_t* list, void* input_key, char* filename) {
    assert(list != NULL);
    // flag to track of whether at least one matching data is found
    int found = 0;
    
    // search matching record and output the result in the provided file 
    return recursive_search_list(list->head, input_key, filename, found);
}
