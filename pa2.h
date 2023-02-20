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
    int x_cord;
    int y_cord;
} Tree_Node;
// splits should store boxs

typedef struct _Box { // Splits should have these values as well fix after creating output 1
    int box_num;
    int width;
    int height;
} Box;

typedef struct _Split { // Splits should have these values as well fix after creating output 1
    char split_type;
    int width;
    int height;
} Split;

Tree_Node* read_tree_recurse(FILE* fp);

void write_output_1(Tree_Node* head, FILE* fp);

void write_output_2(Tree_Node* head, FILE* fp);

void write_output_3(Tree_Node* head, FILE* fp);

