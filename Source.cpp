#include "DrawTree.h"

TreeNode* FreeTree(TreeNode* t)
{
    if (t != NULL)
    {
        FreeTree(t->left);
        FreeTree(t->right);
        delete t;
    }

    return NULL;
}

TreeNode* find_min(TreeNode* t)
{
    if (t == NULL)
    {
        return NULL;
    }
    else if (t->left == NULL)
    {
        return t;
    }
    else
    {
        return find_min(t->left);
    }
}

TreeNode* find_max(TreeNode* t)
{
    if (t == NULL)
    {
        return NULL;
    }
    else if (t->right == NULL)
    {
        return t;
    }
    else
    {
        return find_max(t->right);
    }
}

TreeNode* find(int elem, TreeNode* t)
{
    if (t == NULL)
    {
        return NULL;
    }

    if (elem < t->val)
    {
        return find(elem, t->left);
    }
    else if (elem > t->val)
    {
        return find(elem, t->right);
    }
    else
    {
        return t;
    }
}

//Insert i into the tree t, duplicate will be discarded
//Return a pointer to the resulting tree.                 
TreeNode* insert(int value, TreeNode* t)
{
    if (t == NULL)
    {
        TreeNode* new_node = new TreeNode(value);
        return new_node;
    }

    if (value < t->val)
    {
        t->left = insert(value, t->left);
    }
    else if (value > t->val)
    {
        t->right = insert(value, t->right);
    }
    else //duplicate, ignore it
    {
        return t;
    }

    return t;
}

//Deletes node from the tree
// Return a pointer to the resulting tree
TreeNode* remove(int value, TreeNode* t)
{
    if (t == NULL) {
        return NULL;
    }

    TreeNode* tmp_cell = NULL;
    if (value < t->val)
    {
        t->left = remove(value, t->left);
    }
    else if (value > t->val)
    {
        t->right = remove(value, t->right);
    }
    else if (t->left && t->right)
    {
        tmp_cell = find_min(t->right);
        t->val = tmp_cell->val;
        t->right = remove(t->val, t->right);
    }
    else
    {
        tmp_cell = t;
        if (t->left == NULL)
            t = t->right;
        else if (t->right == NULL)
            t = t->left;

        delete tmp_cell;
    }

    return t;
}


//driver routine
void main()
{
    TreeNode* root = NULL;

    printf("\nAfter inserting val 10..\n");
    root = insert(10, root);
    print_ascii_tree(root);

    printf("\nAfter inserting val 5..\n");
    root = insert(5, root);
    print_ascii_tree(root);

    printf("\nAfter inserting val 15..\n");
    root = insert(15, root);
    print_ascii_tree(root);

    printf("\nAfter inserting vals 9, 13..\n");
    root = insert(9, root);
    root = insert(13, root);
    print_ascii_tree(root);

    printf("\nAfter inserting vals 2, 6, 12, 14, ..\n");
    root = insert(2, root);
    root = insert(6, root);
    root = insert(12, root);
    root = insert(14, root);
    print_ascii_tree(root);


    printf("\n\nAfter deleting a node (14) with no child..\n");
    root = remove(14, root);
    print_ascii_tree(root);

    printf("\n\nAfter deleting a node (13) with left child..\n");
    root = remove(13, root);
    print_ascii_tree(root);

    printf("\n\nAfter deleting a node (5) with left and right children..\n");
    root = remove(5, root);
    print_ascii_tree(root);

    printf("\n\nAfter deleting a node (10, root node) with left and right children..\n");
    root = remove(10, root);
    print_ascii_tree(root);

    FreeTree(root);
}