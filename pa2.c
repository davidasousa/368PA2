#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "pa2.h"

static void free_tree(Tree_Node* head) // Free the actual nodes and the info for the boxes
{ 
    if(head -> left != NULL)
        free_tree(head -> left);
    if(head -> right != NULL)
        free_tree(head -> right);

    if(head -> type == SPLIT)
    {
        free(head -> info);
        free(head);        
        return;
    }
    if(head -> type == BOX)
    {
        free(head -> info);
        free(head);
        return;
    }
}

Tree_Node* read_tree_recurse(FILE* fp)
{
    int box_num, width, height;

    int num_read = fscanf(fp, "%d(%d,%d)\n", &box_num, &width, &height);
    
    if(num_read == 3) 
    {
        Tree_Node* box_node = malloc(sizeof(*box_node));
        Box* box_info = malloc(sizeof(*box_info));

        *box_info = (Box){.box_num = box_num, .width = width, .height = height};
        *box_node = (Tree_Node){.type = BOX, .info = box_info, .left = NULL, .right = NULL};
    
        return box_node;
    }
    Split* split = malloc(sizeof(*split));
    
    int num_read2 = fscanf(fp, "%c\n", &(split -> split_type));
 
    if(num_read2 == 1)
    {
        Tree_Node* split_node = malloc(sizeof(*split_node));
        *split_node = (Tree_Node){.type = SPLIT, .info = split, .left = NULL, .right = NULL};

        split_node -> left = read_tree_recurse(fp);
        split_node -> right = read_tree_recurse(fp);

        return split_node; 
    }
    return NULL;
}

void write_output_1(Tree_Node* head, FILE* fp)
{
    if(head == NULL)
        return;

    write_output_1(head -> left, fp);
    write_output_1(head -> right, fp);

    if(head -> type == SPLIT)
        fprintf(fp, "%c\n", ((Split*)(head -> info)) -> split_type);
    if(head -> type == BOX)
        fprintf(fp, "%d(%d,%d)\n", ((Box*)(head -> info)) -> box_num, ((Box*)(head -> info)) -> width, ((Box*)(head -> info)) -> height);

    return;
}

static int find_split_width(Tree_Node* head)
{
    if(head == NULL)
        return 0;

    if(head -> type == BOX)
        return ((Box*)(head -> info)) -> width;

    int width_left = find_split_width(head -> left);
    int width_right = find_split_width(head -> right);
 
    if(((Split*)(head -> info)) -> split_type == 'V')
    {
        ((Split*)(head -> info)) -> width = width_left + width_right;
        return width_left + width_right;
    }
    else
    {
        if(width_right > width_left)
        { 
            ((Split*)(head -> info)) -> width = width_right;
            return width_right;
        }
        else
        {
            ((Split*)(head -> info)) -> width = width_left;
            return width_left;
        }
    }
}

static int find_split_height(Tree_Node* head)
{
    if(head == NULL)
        return 0;

    if(head -> type == BOX)
        return ((Box*)(head -> info)) -> height;

    int height_left = find_split_height(head -> left);
    int height_right = find_split_height(head -> right);
 
    if(((Split*)(head -> info)) -> split_type == 'H')
    {
        ((Split*)(head -> info)) -> height = height_left + height_right;
        return height_left + height_right;
    }
    else
    {
        if(height_right > height_left)
        { 
            ((Split*)(head -> info)) -> height = height_right;
            return height_right;
        }
        else
        {
            ((Split*)(head -> info)) -> height = height_left;
            return height_left;
        }
    }
}

void write_output_2(Tree_Node* head, FILE* fp)
{
    if(head == NULL)
        return;

    write_output_2(head -> left, fp);
    write_output_2(head -> right, fp);

    if(head -> type == SPLIT)
        fprintf(fp, "%c(%d,%d)\n", ((Split*)(head -> info)) -> split_type, ((Split*)(head -> info)) -> width, ((Split*)(head -> info)) -> height);
    if(head -> type == BOX)
        fprintf(fp, "%d(%d,%d)\n", ((Box*)(head -> info)) -> box_num, ((Box*)(head -> info)) -> width, ((Box*)(head -> info)) -> height);

    return;
}

// Finding X and Y coordinates works when you remove all x parts

static void find_y(Tree_Node* root, int* y, int* x)
{
    if(root == NULL || root -> type == BOX)
        return;
    
    if(((Split*)(root -> info)) -> split_type == 'H')
    {
        *y = *y + (((Split*)(root -> info)) -> height - ((Split*)(root -> left -> info)) -> height);
        root -> left -> y_cord = *y;
        root -> right -> y_cord = *y - (((Split*)(root -> right -> info)) -> height);

        printf("%d \n", *y);

    }
    if(((Split*)(root -> info)) -> split_type == 'V')
    {
        root -> left -> y_cord = *y;
        root -> right -> y_cord = *y;

    } 
    find_y(root -> left, y, x);
    *y = 0;
    find_y(root -> right, y, x);
}

void write_output_3(Tree_Node* head, FILE* fp)
{
    if(head == NULL || head -> type == BOX)
        return;

    write_output_3(head -> left, fp);
 
    if(head -> left -> type == BOX)
    {
        printf("%d((%d,%d)(%d,%d))\n", ((Box*)(head -> left -> info)) -> box_num, ((Box*)(head -> left -> info)) -> width, ((Box*)(head -> left -> info)) -> height, head -> left -> x_cord, head -> left -> y_cord);
    }
    if(head -> right -> type == BOX)
    {
        printf("%d((%d,%d)(%d,%d))\n", ((Box*)(head -> right -> info)) -> box_num, ((Box*)(head -> right -> info)) -> width, ((Box*)(head -> right -> info)) -> height, head -> right -> x_cord, head -> right -> y_cord);
    }

    write_output_3(head -> right, fp);

    return;
}

int main(int argc, char* argv[]) 
{

    if(argc != 5)
        return EXIT_FAILURE;
                                        
    FILE* fp0 = fopen(argv[1], "r");
    if(fp0 == NULL)
        return EXIT_FAILURE;
    Tree_Node* root = read_tree_recurse(fp0);
    fclose(fp0);
    
    find_split_width(root);
    find_split_height(root);

    int y = 0;
    int x = 0;
    find_y(root, &y, &x);

    // Output 1
    FILE* fp = fopen(argv[2], "w");
    if(fp == NULL)
        return EXIT_FAILURE;
    write_output_1(root, fp);
    fclose(fp);

    // Output 2
    FILE* fp2 = fopen(argv[3], "w");
    if(fp2 == NULL)
        return EXIT_FAILURE;
    write_output_2(root, fp2);
    fclose(fp2);

    // Output 3
    FILE* fp3 = fopen(argv[4], "w");
    if(fp3 == NULL)
        return EXIT_FAILURE;
    write_output_3(root, fp3);
    fclose(fp3);
    
    free_tree(root);
    return EXIT_SUCCESS;
}
