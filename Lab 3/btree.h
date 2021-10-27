#ifndef HEADER_FILE
#define HEADER_FILE

#define MAX_HUFF_CODE 256

struct freq_table {
    int count;
    int code[MAX_HUFF_CODE];
    int code_size;

};

struct node {
    int character;
    int count;
    struct node *left;
    struct node *right;
};

struct linked {
    int character;
    int count;
    struct node *node;
    struct linked *next;
};

struct node* new_node(int character, int count);
struct node *create_tree(struct node *node_1, struct node *node_2);
void get_code(struct node *tree, struct freq_table *table,
        int index, int array[]);
int is_leaf(struct node *node);
struct linked* create_linked_list(int character, int count);
struct linked* insert(struct linked* link, struct linked* list);
struct linked* remove_link(int character, struct linked* list);
void linked_incr_count(int character, struct linked* list);



#endif
