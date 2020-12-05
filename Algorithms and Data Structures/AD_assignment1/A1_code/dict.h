#ifndef DICTH
#define DICTH

// maximum length of field in the dataset
#define MAX_FIELD 128
#define MAX_LINE 512
#define FIELD_NUM 11
#define SCAN_FIELD "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]"

list_t* insert_line(char* newline, list_t* list);
list_t* create_dict(char* file_name, list_t* list);
void* recursive_search_list(node_t* node, char* input_key, char* filename, 
                            int found);
void* search_list(list_t* list, void* input_key, char* filename);

#endif