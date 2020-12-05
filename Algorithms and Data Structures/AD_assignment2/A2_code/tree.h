#ifndef TREEH
#define TREEH

#define DIM 2

/* tree structure for 2d tree in tree.c */
typedef struct node2d node2d_t;

/* tree structure for 2d tree in tree.c */
struct node2d {
    double* key;
	list_t* value;
	node2d_t* left;
    node2d_t* right;
    int k_index;
};

void free_tree(node2d_t* parent);
void free_2dnode(node2d_t* parent);
double distance(node2d_t* parent, double* key_input);
node2d_t* insert_tree_node(node2d_t* parent, data_t* data, double* key_value);

#endif 