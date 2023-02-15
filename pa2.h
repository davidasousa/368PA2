#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

// Children Vs Internal Nodes Will Be Identified Using The Fact That Children Have No Children Whilst Internal Nodes Do

typedef struct _Tree_Node { // Nodes Contain A Type Enum And Their Data In A Union

    enum type {SPLIT, BOX}type;
    void* info;
    struct _Tree_Node *left;
    struct _Tree_Node *right;
} Tree_Node;

typedef struct _Box {
    int box_num;
    int x_cord;
    int y_cord;
} Box;

Tree_Node* assemble_tree(char* input_file, char** contents);

Tree_Node* read_elem(char* file_contents, int* idx);

void insert_to_tree(Tree_Node* head, Tree_Node* new_node);
