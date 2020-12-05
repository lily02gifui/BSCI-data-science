/**
 * Program written by Alistair Moffat, as an example for the book
 * "Programming, Problem Solving, and Abstraction with C".
 * Modified by Aoi Fujii on Oct, 2020.
 * Program to store the game state for peg solitaire.
*/

#ifndef __LISTOP__
#define __LISTOP__

#include "utils.h"
#include <assert.h>

typedef struct lnode lnode_t;

struct lnode {
	node_t* data;
	lnode_t* next;
};

typedef struct {
	lnode_t* head;
	lnode_t* foot;
} list_t;

list_t* make_empty_list(void);
int is_empty_list(list_t *list);
void free_list(list_t *list);
list_t* insert_at_head(list_t *list, node_t* value);
list_t* insert_at_foot(list_t *list, node_t* value);
node_t* get_head(list_t *list);
list_t* get_tail(list_t *list);

#endif