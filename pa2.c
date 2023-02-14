#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "pa2.h"

Tree_Node* read_elem(char* file_contents, int* idx) // CHECK IF Input garunteed to be valid
{
    Tree_Node* new_node = NULL;

    // Starts At The New Value -> Check If Internal Or External
    if(file_contents[*idx] == 'H' || file_contents[*idx] == 'V') // Incrementation Works Properly
    {
        new_node = malloc(sizeof(*new_node));
        *new_node = (Tree_Node) {.type = SPLIT, .info = &file_contents[*idx], .left = NULL, .right = NULL};
        printf(" %c", file_contents[*idx]);
        *idx += 2;
    }
    else
    {

        printf(" %c ", file_contents[*idx]); // equals num box
        // Parsing To The (
        while(file_contents[*idx] != '(')
            *idx += 1;
        *idx += 1;
        printf(" %c ", file_contents[*idx]); // equals num box
        while(file_contents[*idx] != ',')
            *idx += 1;
        *idx += 1;
        printf(" %c ", file_contents[*idx]); // equals num box
        while(file_contents[*idx] != ')')
            *idx += 1;
        *idx += 2;



    }
    return new_node; 
}
/*
void insert_to_tree(Tree_Node* head, Tree_Node* new_node)
{
    if(head -> left == NULL)
    {
        head -> left = new_node;
    } 
    else if(head -> right == NULL) 
    {
        head -> right = new_node;
    }
    else if(head -> left ->type == SPLIT)
    {
        insert_to_tree(head -> left, new_node);
    }
    return;
}
*/
Tree_Node* assemble_tree(char* input_file)
{
    FILE* fp = fopen(input_file, "r");
    if(fp == NULL) // Testing If Opening The File Works
    {
        return NULL;
    }

    fseek(fp, 0, SEEK_END); // Finding Size Of The Input File
    int bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);    

    char* file_contents = malloc(sizeof(*file_contents) * bytes);

    int cont_idx = 0;
    for(char ch = fgetc(fp); !feof(fp); ch = fgetc(fp)) // Storing File Contents In An Array To Deal With 
    {
        file_contents[cont_idx] = ch;
        cont_idx++; 
    }

    // Creating The Head Node // Parse Through Like JSON

    Tree_Node* head_node = NULL;
    int parse_idx = 0;
    while(parse_idx < bytes)
    {
        if(head_node == NULL) // Creation Of The Head Node
        {
            head_node = malloc(sizeof(*head_node)); // CHECKING IF IT IS A SPLIT VS A TREE OF JUST ONE RECTANGLE
            if(file_contents[parse_idx] == 'H' || file_contents[parse_idx] == 'V')
            head_node = malloc(sizeof(*head_node));
            *head_node = (Tree_Node) {.type = SPLIT, .info = &file_contents[parse_idx], .left = NULL, .right = NULL};
            printf("%c ", file_contents[parse_idx]);
            parse_idx += 2;

        }
        Tree_Node* new_node = read_elem(file_contents, &parse_idx);
        //insert_to_tree(head_node, new_node); 
    }

    // free(file_contents); // The Array is used to reference values by address later in the program
    fclose(fp);

    return head_node;
}

int main(int argc, char* argv[]) 
{
    // Opening The File

    char* input = argv[1];
    Tree_Node* head_node = assemble_tree(input); 
    free(head_node);
    return EXIT_SUCCESS;
}
