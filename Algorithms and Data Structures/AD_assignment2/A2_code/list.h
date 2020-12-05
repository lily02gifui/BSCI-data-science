#ifndef LISTH
#define LISTH

/* linked structure for linked list in list.c */
typedef struct node node_t;

/* holds data from "the Business establishment trading name
* and industry classiﬁcation 2018 dataset" */
typedef struct {
	char* census_year;
	char* block_id;
	char* property_id;
	char* base_property_id;
	char* clue_small_area;
    char* trading_name;
	char* industry_code;
	char* industry_description;
	char* xcoord;
	char* ycoord;
	char* location;
} data_t;

/* Linked structure for linked list in list.c */
struct node {
    //char* key;
	data_t* data;
	node_t* next;
};

/* linked structure for linked list in list.c */
typedef struct {
	node_t* head;
	node_t* foot;
} list_t;

list_t* make_empty_list(void);
void free_list(list_t* list);
list_t* insert_at_foot(list_t* list, data_t* value);
data_t* get_head(list_t *list);
list_t* concat_list(list_t* list1, list_t* list2);
int is_empty_list(list_t* list);
void print_list(char* filename, list_t* list, double* input_key, double radius);
list_t* delete_list_nodes(list_t* list);

#endif