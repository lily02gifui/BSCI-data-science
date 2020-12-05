/* main.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
*
* A program to construct a linked list to store the information contained
* in the data ﬁle speciﬁed in the command line argument. Search the linked
* list for records, based on keys. The keys will be read in from stdin.
* Look up each key and output the information to the output ﬁle speciﬁed
* by the second command line parameter. If the key is not found, 
* output will be the word NOTFOUND.
* Makefile produces an executable program called dict. 
* To run the dict program, type:
* ./dict datafile outputfile then type in keys or
* ./dict datafile outputfile < keyfile
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "dict.h"


int main(int argc, char** argv) {
    // check stdin, must include datafile and outfile name
    if (argc <= 2) {
        fprintf(stderr, "incorrect input\n");
        exit(EXIT_FAILURE);
    }
    
    char* in_file = argv[1];
    char* out_file = argv[2];
    char* input_key = NULL;
    size_t len = 0;
    
    list_t* list = make_empty_list();
    
    // read the data into linked list
    list = create_dict(in_file, list);
    
    // get key from stdin
    while (getline(&input_key, &len, stdin) != -1) {
        // remove '\n' from key
        input_key[strcspn(input_key, "\n")] = '\0';
        
        // find the data with matching key and write into the output
        search_list(list, input_key, out_file);
    }
    
    free(input_key);
    free_list(list);
    
    return 0;
}
