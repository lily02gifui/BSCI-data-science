/* map1.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 20/09/2020
*
* This program are used for ./map1
* scan the input for the format used in ./map1
* search 2d tree by nearest neighbour search for ./map1
*
*/


#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tree.h"
#include "map1.h"


/* scan the input and assign the value to key_value */
void scan_input(char* input_key, double* key_value, double* radius) {
    if(sscanf(input_key, "%lf %lf %lf", &key_value[KEY_X], &key_value[KEY_Y], radius)!=INPUT_NEAREST){
        printf("Error: input was not valid.\n");
        exit(EXIT_FAILURE);
    }   
    *radius = -1;
}


/* seach the value corresponds to the key_input by nearest neighbour search method. 
* store the nearest value in "nearest" */
void search_tree(node2d_t* parent, double* key_input, int k_index,
             list_t** nearest, double* dist, int* comparison, double radius) {
    
    double d;
    double dx;
    double dx2;
    
    // no parent node to search
    if (parent == NULL) {
        return;
    }
    
    // visit the node
    // calculate square distance 
    d = distance(parent, key_input);
    dx = parent->key[k_index] - key_input[k_index];
    dx2 = dx * dx;
    ++(*comparison);
    
    // found nearlest point to be stored
    if (is_empty_list(*nearest) || d <= *dist) {
        
        // add the closest point if the distance are the same
        if ((!is_empty_list(*nearest)) && d == *dist) {
            
            // add values into the "nearest"
            *nearest = concat_list(*nearest, parent->value);
            
        }
        else {
            // clear the list
            delete_list_nodes(*nearest);
            
            // store new nearest values
            *nearest = concat_list(*nearest, parent->value);
        } 
        
        *dist = d;
    }
    
    // found exact match
    if (!*dist) {
        return;
    }
 
    // update k_index for the next node
    k_index++;
    if (k_index >= DIM) {
        k_index = 0;
    }
    
    // target node is on the left when the compared value is smaller than visited node(parent)
    // otherwise search right side
    search_tree(dx > 0 ? parent->left : parent->right, key_input, k_index, nearest, dist, comparison, radius);
    
    // stop searching the other side if their closest possible distance is bigger than the best distance
    if (dx2 > *dist) {
        return;
    }
    
    // look for the other side of the parent node
    search_tree(dx > 0 ? parent->right : parent->left, key_input, k_index, nearest, dist, comparison, radius);
}
