/* map2.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 20/09/2020
*
* This program are used for ./map2
* scan the input for the format used in ./map2
* search 2d tree by radius search for ./map2
*
*/


#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tree.h"
#include "map2.h"


/* scan the input and assign the value to key_value */
void scan_input(char* input_key, double* key_value, double* radius) {
    
    if(sscanf(input_key, "%lf %lf %lf", &key_value[KEY_X], &key_value[KEY_Y], radius)!=INPUT_RADIUS){
        printf("Error: input was not valid.\n");
        exit(EXIT_FAILURE);
    }   
}


/* seach the value corresponds to the key_input by radius search method. 
* store the value within radius to "near" */
void search_tree(node2d_t* parent, double* key_input, int k_index,
             list_t** near, double* dist, int* comparison, double radius) {
    
    double d;
    double dk;
    double dk2;
    
    // no parent node to search
    if (parent == NULL) {
        return;
    }
    
    // visit the node
    // calculate square distance
    d = distance(parent, key_input);
    dk = parent->key[k_index] - key_input[k_index];
    dk2 = dk * dk;
    ++(*comparison);
    
    // found a point within radius
    if (d <= (radius*radius)) {
        //store the point
        *near = concat_list(*near, parent->value);
    }
    
    // update k index for the next search node
    k_index++;
    if (k_index >= DIM) {
        k_index = 0;
    }
    
    //target node is on the left when the compared value is smaller than visited node(parent)
    // otherwise search right side
    search_tree(dk > 0 ? parent->left : parent->right, key_input, k_index, near, dist, comparison, radius);
    
    // stop searching the other side if their closest possible distance is bigger than the best distance
    if (dk2 > (radius*radius)) {
        return;
    }
    
    // search the other side of the parent node
    search_tree(dk > 0 ? parent->right : parent->left, key_input, k_index, near, dist, comparison, radius);
}