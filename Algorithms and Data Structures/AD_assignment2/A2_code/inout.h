#ifndef INOUTH
#define INOUTH

// output format for the result of searching dict
#define OUTNAME_NEAR "%lf %lf --> "
#define OUTNAME_RADIUS "%lf %lf %lf --> "
#define OUTDATA "Census year: %s || Block ID: %s || Property ID: %s || Base property ID: %s || CLUE small area: %s || Trading Name: %s || Industry (ANZSIC4) code: %s || Industry (ANZSIC4) description: %s || x coordinate: %0.4lf || y coordinate: %0.4lf || Location: %s || \n"
#define OUTNOTFOUND "NOTFOUND \n"

void check_quote(char* line, char* newline, int* in_quote, int* i, int* newi);
void modify_line(char* line, char* newline);
void write_out(char* filename, data_t* data, double* input_key, double radius);
void write_no_out(char* filename, double* input_key, double radius);

#endif