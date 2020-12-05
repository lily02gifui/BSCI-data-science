#ifndef INOUTH
#define INOUTH

// output format for the result of searching dict
#define OUTNAME "%s --> "
#define OUTDATA "Census year: %s || Block ID: %s || Property ID: %s || Base property ID: %s || CLUE small area: %s || Industry (ANZSIC4) code: %s || Industry (ANZSIC4) description: %s || x coordinate: %s || y coordinate: %s || Location: %s || \n"
#define OUTNOTFOUND "NOTFOUND \n"

void check_quote(char* line, char* newline, int* in_quote, int* i, int* newi);
void modify_line(char* line, char* newline);
void write_out(char* filename, data_t* data, char* input_key);
void write_no_out(char* filename, char* input_key);

#endif