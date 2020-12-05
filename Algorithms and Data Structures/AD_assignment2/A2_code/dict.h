#ifndef DICTH
#define DICTH

#include "tree.h"

// maximum length of field in the dataset
#define MAX_FIELD 128
#define MAX_LINE 512
#define FIELD_NUM 11
#define SCAN_FIELD "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]"
#define KEY_SIZE 2
#define KEY_X 0
#define KEY_Y 1

node2d_t* insert_line(char* newline, node2d_t* parent);
node2d_t* create_dict(char* file_name);

#endif