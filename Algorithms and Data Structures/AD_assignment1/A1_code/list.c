/* list.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
*
* A program to  handle linked list, including 
* creating the list, freeing the list, and inserting a node 
* at the end of the linked list.
*
* Reference: listop.c by Alister Moffat (an example for the book
* "Programming, Problem Solving, and Abstraction with C").
* https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"


/* Adopted from listop.c.
* Creates an empty list and returns it. */
list_t* make_empty_list(void) {
	list_t* list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list != NULL);
	list->head = list->foot = NULL;
	return list;
}


/* Frees the list with data from "the Business establishment
* trading name and industry classiﬁcation 2018 dataset" */
void free_list(list_t* list) {
	node_t* curr;
    node_t* prev;
	assert(list != NULL);
	curr = list->head;
    
	while (curr) {
		prev = curr;
		curr = curr->next;
        
        // free data fields
        free(prev->data->census_year);
        free(prev->data->block_id);
        free(prev->data->property_id);
        free(prev->data->base_property_id);
        free(prev->data->clue_small_area);
        //free(prev->data->trading_name);
        free(prev->data->industry_code);
        free(prev->data->industry_description);
        free(prev->data->xcoord);
        free(prev->data->ycoord);
        free(prev->data->location);
        
        // free data_t*
        free(prev->data);
        
        // free key
        free(prev->key);
        
        // free node
		free(prev);
	}
	free(list);
}


/* Adopted from listop.c 
* Modified by Aoi Fujii in 25/08/2020.
* Create a node with the input value and insert it at the end of the list.
* Returns the modified list. */
list_t* insert_at_foot(list_t* list, data_t* value, char* key_value) {
	node_t* new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list != NULL && new != NULL);
    
    new->key = key_value;
	new->data = value;
	new->next = NULL;
    
	if (list->foot == NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	}
    else {
		list->foot->next = new;
		list->foot = new;
	}
    
	return list;
}
