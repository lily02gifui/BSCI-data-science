/* list.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
* modified 20/09/2020
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
#include "inout.h"


/* Adopted from listop.c.
* Creates an empty list and returns it. */
list_t* make_empty_list(void) {
	list_t* list;
	list = (list_t*)malloc(sizeof(list_t));
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
        free(prev->data->trading_name);
        free(prev->data->industry_code);
        free(prev->data->industry_description);
        free(prev->data->xcoord);
        free(prev->data->ycoord);
        free(prev->data->location);
        
        // free data_t*
        free(prev->data);
        
        // free node
		free(prev);
	}
	free(list);
}


/* deletes the elements of the linked list.
* return the empty list */
list_t* delete_list_nodes(list_t* list) {
	node_t* curr;
    node_t* prev;
	assert(list != NULL);
	curr = list->head;
    
	while (curr) {
		prev = curr;
		curr = curr->next;
        
        // free node
		free(prev);
	}
    list->head = list->foot = NULL;
	return list;
}


/* Adopted from listop.c 
* Modified by Aoi Fujii in 25/08/2020.
* Create a node with the input value and insert it at the end of the list.
* Returns the modified list. */
list_t* insert_at_foot(list_t* list, data_t* value) {
	node_t* new;
	new = (node_t*)malloc(sizeof(node_t));
	assert(list != NULL && new != NULL);
    
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


/* Adopted from listops.c
* get data value from the head of the list */
data_t* get_head(list_t* list) {
	assert(list != NULL && list->head != NULL);
	return list->head->data;
}


/* concatenate two lists(add value in list2 to the end of list1)
* return the modified first list */
list_t* concat_list(list_t* list1, list_t* list2) {
    
    assert(list1 != NULL && list2 != NULL);
    
    // copy the value of list2 and insert at the end of list1
    node_t* curr = list2->head;
    while (curr) {
        list1 = insert_at_foot(list1, curr->data);
        curr = curr->next;
    }
    
    return list1;
}


/* retruns 1 if list is empty */
int is_empty_list(list_t* list) {
	assert(list != NULL);
	return (list->head == NULL);
}


/* erite data in the list to the given file */
void print_list(char* filename, list_t* list, double* input_key, double radius) {
    // if the list is empty, write out NOTFOUND
    if (is_empty_list(list)) {
        write_no_out(filename, input_key, radius);
        return;
    }
    
    // print out data from each node of the list
    node_t* head = list->head;
    while (head) {
        write_out(filename, head->data, input_key, radius);
        head = head->next;
    } 
}
