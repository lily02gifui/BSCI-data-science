/**
 * ai.c from a base code provided by COMP20003, 2020.
 * Retrieved from https://canvas.lms.unimelb.edu.au/courses/87167/files/5465229/download?wrap=1
 * Modified by Aoi Fujii on Oct, 2020.
 * Implements ai solver operations for peg solitaire game.
*/

#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"
#include "listop.h"

/**
 * Copies the game state from src to dst. 
*/
void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}

/**
 * Creates initial node to store the game state 
*/
node_t* create_init_node( state_t* init_state ){
	node_t* new_n = (node_t*) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
 * Modified by Aoi Fujii.
*/
node_t* apply_action(node_t* n, position_s selected_peg, move_t action ){

    node_t* new_node = (node_t*)malloc(sizeof(node_t));
	assert(new_node != NULL);

	// new node points to the parent n
	new_node->parent = n;

	// assign state before action 
	new_node->state = n->state;
	
	//move the cursor to the selected peg before moving its position
	new_node->state.cursor.x = selected_peg.x;
	new_node->state.cursor.y = selected_peg.y;

	// update the state with the chosen action
    execute_move_t( &(new_node->state), &selected_peg, action);

	// update the depth of the node
	new_node->depth = (n->depth + 1);

	// updates the action used to create the node
	new_node->move = action;

	return new_node;
}

/**
 * Frees memories in hash table, list of nodes, and stack.
 * Created by Aoi Fujii.
*/
void free_memory(HashTable table, list_t* all_nodes){
	// free nodes in stack
	free_stack();

	// free hash
	int freed = ht_destroy(&table);

	//free explored nodes from the root
	free_list(all_nodes);
}

/**
 * Finds a solution path as per algorithm description in the handout
 * Modified by Aoi Fujii.
 */
void find_solution( state_t* init_state  ){

	HashTable table;

	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE, sizeof(int8_t) * SIZE * SIZE, 16769023);

	// Initialize Stack
	initialize_stack();

	// create the initial node
	node_t* root = create_init_node( init_state );
	
	// store the state of all nodes explored
	list_t* all_nodes = make_empty_list(); 

	// add n to the stack to be explored
	stack_push(root); 

	int remaining_pegs = num_pegs(&(root->state));

	while (!is_stack_empty()){
		// get the first node to explore
		node_t* n = stack_top();
		expanded_nodes++;

		//add to the list of explored nodes
		all_nodes = insert_at_foot(all_nodes, n);

		// remove the node from stuck
		stack_pop();
		
		// add to the hash table to avoid duplicate
		ht_insert(&table, &(n->state.field), &(n->state.field));

		// found a better solution: can remove more pegs
		if (num_pegs(&(n->state)) < remaining_pegs){
			save_solution(n);
			remaining_pegs = num_pegs(&(n->state));
		}

		//for each jump action a ∈ [0, m) × [0, m) × {Lef t, Right, U p, Down}
		for(int x = 0; x < SIZE; x++){
			for(int y = 0; y < SIZE; y++){
				for(move_t jump = left; jump <= down; jump++){
					// selected peg to be moved
					position_s selected_peg;
					selected_peg.x = x;
					selected_peg.y = y;
					
					// the action is legal for node n
					if(can_apply(&(n->state), &selected_peg, jump)){

						// create a child node
						node_t* new_node = apply_action(n, selected_peg, jump); 
						generated_nodes++;	

						// Peg Solitaire Solved
						if (won(&(new_node->state))){
							// store the solution path
							save_solution(new_node);
							remaining_pegs = num_pegs(&(new_node->state));
							
							// free new node
							free(new_node);
							// free stack, hash, and nodes in memory
							free_memory(table, all_nodes);

							return;
						}

						// Avoid duplicates
						if (!ht_contains(&table, &(new_node->state.field))) {
							// DFS strategy: add to stack to explore
							stack_push(new_node);
						}
						else{
							free(new_node);
						}
					}
				}
			}
		}
		
		// exceeded maximum budget
		if (expanded_nodes >= budget) {
			// free stack, hash, and nodes in memory
			free_memory(table, all_nodes);
			return;
		}
	}
}

