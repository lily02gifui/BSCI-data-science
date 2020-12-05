/**
 * stack.h from a base code provided by COMP20003, 2020.
 * Used for stack operation.
 * Retrieved from https://canvas.lms.unimelb.edu.au/courses/87167/files/5465229/download?wrap=1
*/

#ifndef __STACK_PEG__
#define __STACK_PEG__

#include "utils.h"

// The upperbound solution length = size of the matrix * 4 moves = maximum possible number of pegs * 4 moves
#define STACK_SIZE SIZE*SIZE* 4

// Stack as an arrray
node_t* stack[STACK_SIZE]; 

// Variable marking the top of the stack
int stack_top_idx;  

// Needs to be called before using it for the first time
void initialize_stack();

void stack_push(node_t* n); 

void stack_pop();

node_t* stack_top();

int is_stack_empty();

void print_stack();

void free_stack();

#endif