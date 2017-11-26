#include <iostream>
#include <memory>
#include <algorithm>

#define INFINITY_NUM (1<<20)
#define MAX_HEIGHT 1000
#define LABEL_LENGTH 11

struct TreeNode
{
    TreeNode() : left(NULL), right(NULL), val(0) {}
    TreeNode(int n) : left(NULL), right(NULL), val(n) {}

    TreeNode* left;
    TreeNode* right;
    int val;
};

class AsciiNode
{
public:
    AsciiNode() :
        left(NULL), 
        right(NULL), 
        edge_length(0),
        height(0),
        lablen(0),
        parent_dir(0),
        g_print_next(0),
        g_gap(3) {
        std::fill(label, label + LABEL_LENGTH, 0);
        std::fill(g_lprofile, g_lprofile + MAX_HEIGHT, 0);
        std::fill(g_rprofile, g_rprofile + MAX_HEIGHT, 0);
    }

    //prints ascii tree for given Tree structure
    void print_ascii_tree(TreeNode* t)
    {
        if (t == NULL) {
            return;
        }

        AsciiNode* proot = build_ascii_tree(t);
        compute_edge_lengths(proot);

        for (int i = 0; i<proot->height && i < MAX_HEIGHT; i++) {
            g_lprofile[i] = INFINITY_NUM;
        }
        compute_g_lprofile(proot, 0, 0);

        int xmin = 0;
        for (int i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
            xmin = std::min(xmin, g_lprofile[i]);
        }

        for (int i = 0; i < proot->height; i++)
        {
            g_print_next = 0;
            print_level(proot, -xmin, i);
            printf("\n");
        }

        if (proot->height >= MAX_HEIGHT) {
            printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
        }

        free_ascii_tree(proot);
    }


private:
    AsciiNode * build_ascii_tree_recursive(TreeNode* t)
    {
        if (t == NULL) {
            return NULL;
        }

        AsciiNode* node = new AsciiNode();
        node->left = build_ascii_tree_recursive(t->left);
        node->right = build_ascii_tree_recursive(t->right);

        if (node->left != NULL) {
            node->left->parent_dir = -1;
        }

        if (node->right != NULL) {
            node->right->parent_dir = 1;
        }

        sprintf(node->label, "%d", t->val);
        node->lablen = strlen(node->label);

        return node;
    }

    //Copy the tree into the ascii node structre
    AsciiNode * build_ascii_tree(TreeNode* t)
    {
        if (t == NULL) {
            return NULL;
        }

        AsciiNode *node = build_ascii_tree_recursive(t);
        node->parent_dir = 0;

        return node;
    }

    //Free all the nodes of the given tree
    void free_ascii_tree(AsciiNode *node)
    {
        if (node == NULL) {
            return;
        }

        free_ascii_tree(node->left);
        free_ascii_tree(node->right);

        delete node;
    }

    //The following function fills in the g_lprofile array for the given tree.
    //It assumes that the center of the label of the root of this tree
    //is located at a position (x,y).  It assumes that the edge_length
    //fields have been computed for this tree.
    void compute_g_lprofile(AsciiNode *node, int x, int y)
    {
        if (node == NULL) {
            return;
        }

        int isleft = (node->parent_dir == -1);
        g_lprofile[y] = std::min(g_lprofile[y], x - ((node->lablen - isleft) / 2));
        if (node->left != NULL)
        {
            for (int i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
                g_lprofile[y + i] = std::min(g_lprofile[y + i], x - i);
            }
        }

        compute_g_lprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
        compute_g_lprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
    }

    void compute_g_rprofile(AsciiNode *node, int x, int y)
    {
        if (node == NULL) {
            return;
        }

        int notleft = (node->parent_dir != -1);
        g_rprofile[y] = std::max(g_rprofile[y], x + ((node->lablen - notleft) / 2));
        if (node->right != NULL)
        {
            for (int i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
                g_rprofile[y + i] = std::max(g_rprofile[y + i], x + i);
            }
        }

        compute_g_rprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
        compute_g_rprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
    }

    //This function fills in the edge_length and 
    //height fields of the specified tree
    void compute_edge_lengths(AsciiNode *node)
    {
        if (node == NULL) {
            return;
        }

        compute_edge_lengths(node->left);
        compute_edge_lengths(node->right);

        /* first fill in the edge_length of node */
        if (node->right == NULL && node->left == NULL)
        {
            node->edge_length = 0;
        }
        else
        {
            int hmin = 0;
            if (node->left != NULL)
            {
                for (int i = 0; i<node->left->height && i < MAX_HEIGHT; i++) {
                    g_rprofile[i] = -INFINITY_NUM;
                }
                compute_g_rprofile(node->left, 0, 0);
                hmin = node->left->height;
            }
            else
            {
                hmin = 0;
            }
            if (node->right != NULL)
            {
                for (int i = 0; i<node->right->height && i < MAX_HEIGHT; i++) {
                    g_lprofile[i] = INFINITY_NUM;
                }
                compute_g_lprofile(node->right, 0, 0);
                hmin = std::min(node->right->height, hmin);
            }
            else
            {
                hmin = 0;
            }

            int delta = 4;
            for (int i = 0; i<hmin; i++) {
                delta = std::max(delta, g_gap + 1 + g_rprofile[i] - g_lprofile[i]);
            }

            //If the node has two children of height 1, then we allow the
            //two leaves to be within 1, instead of 2 
            if (((node->left != NULL && node->left->height == 1) ||
                (node->right != NULL && node->right->height == 1)) && delta>4)
            {
                delta--;
            }

            node->edge_length = ((delta + 1) / 2) - 1;
        }

        //now fill in the height of node
        int h = 1;
        if (node->left != NULL)
        {
            h = std::max(node->left->height + node->edge_length + 1, h);
        }
        if (node->right != NULL)
        {
            h = std::max(node->right->height + node->edge_length + 1, h);
        }
        node->height = h;
    }

    //This function prints the given level of the given tree, assuming
    //that the node has the given x cordinate.
    void print_level(AsciiNode *node, int x, int level)
    {
        if (node == NULL) {
            return;
        }

        int isleft = (node->parent_dir == -1);
        if (level == 0)
        {
            int i = 0;
            for (; i<(x - g_print_next - ((node->lablen - isleft) / 2)); i++) {
                printf(" ");
            }
            g_print_next += i;
            printf("%s", node->label);
            g_print_next += node->lablen;
        }
        else if (node->edge_length >= level)
        {
            if (node->left != NULL)
            {
                int i = 0;
                for (; i<(x - g_print_next - (level)); i++) {
                    printf(" ");
                }
                g_print_next += i;
                printf("/");
                g_print_next++;
            }
            if (node->right != NULL)
            {
                int i = 0;
                for (; i<(x - g_print_next + (level)); i++) {
                    printf(" ");
                }
                g_print_next += i;
                printf("\\");
                g_print_next++;
            }
        }
        else
        {
            print_level(node->left,
                x - node->edge_length - 1,
                level - node->edge_length - 1);
            print_level(node->right,
                x + node->edge_length + 1,
                level - node->edge_length - 1);
        }
    }

private:
    AsciiNode *left, *right;

    //length of the edge from this node to its children
    int edge_length;
    int height;
    int lablen;

    //-1=I am left, 0=I am root, 1=right   
    int parent_dir;

    //max supported unit32 in dec, 10 digits max
    char label[LABEL_LENGTH];

    //used for printing next node in the same level, this is the x coordinate of the next char printed
    int g_print_next;
    
    int g_lprofile[MAX_HEIGHT];
    int g_rprofile[MAX_HEIGHT];

    //adjust gap between left and right nodes
    const int g_gap;
};