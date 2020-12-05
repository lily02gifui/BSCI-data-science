#ifndef MAP1H
#define MAP1H

#define INPUT_NEAREST 2
#define KEY_X 0
#define KEY_Y 1

void scan_input(char* input_key, double* key_value, double* radius);
void search_tree(node2d_t* parent, double* key_input, int k_index,
             list_t** nearest, double* dist, int* comparison, double radius);
#endif