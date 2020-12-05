/* tree.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 20/09/2020
*
* this program is for tree operation.
* used to build a tree, free tree, free tree node 
* and calculate teh distance between parent node and searching key node
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "dict.h"
#include "tree.h"


/* Takes a parent pointer (null for the root),
* and returns the tree with the child in the right position. 
* Returns the item in a new tree with null left/right pointers.*/
node2d_t* insert_tree_node(node2d_t* parent, data_t* data, double* key_value) {
    
    assert(parent!=NULL);
    node2d_t** insertLocation = &parent;
    
    // k index for root node is 0
    int parent_k_index = 0;

    // find the insert location until it reaches the leaf node
    while (*insertLocation) {
        // compare the value of xcoord/ycoord depending on the k_index of the parent node 
        parent_k_index = (*insertLocation)->k_index;
        
        // add to the left side
        if (key_value[parent_k_index] < (*insertLocation)->key[parent_k_index]) {
            insertLocation = &((*insertLocation)->left);
        } 
        
        // add to the node if x & y coordinates are the same
        else if ((key_value[parent_k_index] == (*insertLocation)->key[parent_k_index]) 
                && (key_value[(parent_k_index+1)%2] == (*insertLocation)->key[(parent_k_index+1)%2])) {
            
            (*insertLocation)->value = insert_at_foot((*insertLocation)->value, data);
            // delete duplicate key value
            free(key_value);
            return parent;
        }
        
        // add it to the right side
        else {
            insertLocation = &((*insertLocation)->right);
        }
    }
    
    // got to the leaf node to insert value
    *insertLocation = (node2d_t*)malloc(sizeof(node2d_t));
    assert(*insertLocation != NULL);
    (*insertLocation)->left = NULL;
    (*insertLocation)->right = NULL;
    (*insertLocation)->key = key_value;
    //calculate k index from the parent node
    (*insertLocation)->k_index = (parent_k_index + 1) % 2; 
    // create a list to store values
    list_t* value_list = make_empty_list();
    (*insertLocation)->value = insert_at_foot(value_list, data);
    
    return parent;
}


/* free the tree memory */
void free_tree(node2d_t* parent) {
    
    if (parent != NULL) {
        free_tree(parent->left);
        parent->left = NULL;
        free_tree(parent->right);
        parent->right = NULL;
        free_2dnode(parent);
        parent = NULL;
    }
}


/* free a node of the tree */
void free_2dnode(node2d_t* parent) {
    assert(parent != NULL);
    assert(parent->left == NULL);
    assert(parent->right == NULL);
    
    // free list_t* value
    free_list(parent->value);
      
    // free key
    free(parent->key);
        
    // free node2d_t*
    free(parent);
    parent=NULL;
}


/* calculate the squared distance between parent and node in a 2d plane */
double distance(node2d_t* parent, double* key_input){
    double dx = parent->key[0] - key_input[0];
    double dy = parent->key[1] - key_input[1];
    return (dx * dx + dy * dy);
}



