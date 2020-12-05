/**
 * ai.h from a base code provided by COMP20003, 2020.
 * Retrieved from https://canvas.lms.unimelb.edu.au/courses/87167/files/5465229/download?wrap=1
 * Modified by Aoi Fujii on Oct, 2020.
 * Used for ai solver operations.
*/

#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include "utils.h"
#include "hashtable.h"
#include "listop.h"

void initialize_ai();
// find a solution for the peg solitaire game.
void find_solution( state_t* init_state );
// give a new node after the given action is applied to the selected peg.
node_t* apply_action(node_t* n, position_s selected_peg, move_t action);
// free hash table, nodes in stack, and all nodes explored.
void free_memory(HashTable table, list_t* all_nodes);

#endif
