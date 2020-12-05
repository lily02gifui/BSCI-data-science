/* main.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
* modified 20/09/2020
*
* A program to construct a 2d tree to store the information contained
* in the data ﬁle speciﬁed in the command line argument. Search the tree
* for records, based on keys. The keys will be read in from stdin.
* Look up each key and output the information to the output ﬁle speciﬁed
* by the second command line parameter. If the key is not found, 
* output will be the word NOTFOUND.
* output the number of comparison to stdout
* Makefile produces an executable program called dict. 
* To run the dict program, type:
* ./dict datafile outputfile then type in keys or
* ./dict datafile outputfile < keyfile
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "dict.h"
#include "tree.h"
#include "map1.h"
#include "map2.h"


int main(int argc, char** argv) {
    // check stdin, must include datafile and outfile name
    if (argc <= 2) {
        fprintf(stderr, "incorrect input\n");
        exit(EXIT_FAILURE);
    }
    
    char* in_file = argv[1];
    char* out_file = argv[2];
    char* input_key = NULL;
    size_t len=0;
    ssize_t nread;
    
    // store search key
    double key_value[KEY_SIZE];
    int k_index = 0;
    // store distance for search
    double dist=0;
    // track the number of comparison for seaching
    int comparison = 0;
    
    // create 2d tree from the data in in_file
    node2d_t* parent = create_dict(in_file);
    assert(parent!=NULL);
    
    // store nearest values for searching
    list_t* nearest = make_empty_list();
    
    // get key from stdin
    while ((nread = getline(&input_key, &len, stdin)) != -1) {
        // reset number of comparisons for each keys
        comparison = 0;
        double radius;
        
        // remove '\n' from key
        input_key[strcspn(input_key, "\n")] = '\0';
        
        // get geyvalues (and radius for search for map2)
        scan_input(input_key, key_value, &radius);
        
        // search the 2d tree and get the values corresponds to keys
        list_t** mlist = &nearest;
        search_tree(parent, key_value, k_index,
             mlist, &dist, &comparison, radius);
        nearest = *mlist;
        
        //nearest record(s) should be printed to the output file
        print_list(out_file, nearest, key_value, radius);
        
        //clear records in the list
        nearest = delete_list_nodes(nearest);
        
        // output the number of key comparisons to stdout.
        // for map2
        if (radius!=-1) {
        fprintf(stdout, "%lf %lf %0.4lf --> %d\n", key_value[KEY_X], key_value[KEY_Y], radius, comparison);
        }
        // for map1
        else {
            fprintf(stdout, "%lf %lf --> %d\n", key_value[KEY_X], key_value[KEY_Y], comparison);
        }
    }
    
    // node already deleted in delete_list_nodes
    free(nearest);
    free(input_key);
    free_tree(parent);
    
    return 0;
}