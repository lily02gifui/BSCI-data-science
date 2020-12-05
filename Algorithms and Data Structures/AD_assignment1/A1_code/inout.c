/* inout.c
*
* Created by Aoi Fujii (afujii@student.unimelb.edu.au)
* 25/08/2020
*
* This program is to modify a line by converting separating comma to ; and 
* removing unnecessary quotes for the insert_line function in dict.c
* It also outputs the given data value to the output file.
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "inout.h"


/* Remove unnecessary quotes from the i th character in "line", and
* copy the resulting value into "newi" th character of the "newline".
* Use in_quote as a flag to track whether the character is in the quote. */
void check_quote(char* line, char* newline, int* in_quote, int* i, int* newi) {

    if (line[*i] == '"') {
        // inside the first single quote
        if (!*in_quote) {
            *in_quote = 1;
            //do not store this quote in newline
            *newi -= 1;
        }
        
        // inside double quote
        else if (*in_quote && line[*i + 1] == '"') {
            // skipping the duplicate double quote
            newline[*newi] = '"';
            *i += 1;
        }

        //the end of the quote
        else {
            *in_quote = 0;
            // do not store this quote in newline
            *newi -= 1;
        }    
    }
    
    // not a quotation mark so copy the value into newline
    else {
        newline[*newi] = line[*i];
    }
}


/* Read the line, remove unnecessary quotes, and convert the separating comma
* to ";". Store the value into the newline */
void modify_line(char* line, char* newline) {
    
    int in_quote = 0;
    int i = 0;
    int newi = 0;
        
    for (i = 0; line[i] != '\0'; i++, newi++) {
        // remove unnecessary quotes, copy the value
        check_quote(line, newline, &in_quote, &i, &newi);
        
        //convert separating comma to ";"
        if (line[i] == ',' && !in_quote) {
            newline[newi] = ';';
        }  
    }
    
    newline[newi] = '\0';
    
    //remove newline char
    newline = strtok(newline, "\n");
    // remove carriage return
    newline = strtok(newline, "\r");
    
}


/* Append data regarding the serching key to the provided file */
void write_out(char* filename, data_t* data, char* input_key) {
    
    FILE* fp;
    fp = fopen(filename, "a");
    assert(fp != NULL);
    
    fprintf(fp, OUTNAME, input_key);
    fprintf(fp, OUTDATA, data->census_year, data->block_id, data->property_id, 
        data->base_property_id, data->clue_small_area, data->industry_code, 
        data->industry_description, data->xcoord, data->ycoord, data->location);
    
    fclose(fp);  
}


/* Append the output when the matching data with the input key is not found */
void write_no_out(char* filename, char* input_key) {
    FILE* fp;
    fp = fopen(filename, "a");
    assert(fp != NULL);
    fprintf(fp, OUTNAME, input_key);
    fputs(OUTNOTFOUND, fp);
    fclose(fp);
}
