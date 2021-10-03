# sample-binary-tree-cpp

This project is modified from http://web.archive.org/web/20090617110918/http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

# Getting Started
## How to compile
```
g++ Source.cpp
clang++ Source.cpp 
```

## How to run
```
./a.out
```

## Build by docker
```
docker build -t mycpp .
docker run -it --rm mycpp
./BinaryTree
```

# About sample-binary-tree-cpp
## nodes in binary tree  
```cpp
struct TreeNode
{
    TreeNode() : left(NULL), right(NULL), val(0) {}
    TreeNode(int n) : left(NULL), right(NULL), val(n) {}

    TreeNode* left;
    TreeNode* right;
    int val;
};
```

## Result of running this sample code
```
After inserting val 10..
10

After inserting val 5..
 10
 /
5

After inserting val 15..
 10
 / \
5  15

After inserting vals 9, 13..
   10
   / \
  /   \
 /     \
5      15
 \     /
  9   13

After inserting vals 2, 6, 12, 14, ..
     10
     / \
    /   \
   /     \
  5      15
 / \     /
2   9   13
   /   / \
  6   /   \
     12   14


After deleting a node (14) with no child..
     10
     / \
    /   \
   /     \
  5      15
 / \     /
2   9   13
   /   /
  6   12


After deleting a node (13) with left child..
     10
     / \
    /   \
   /     \
  5      15
 / \     /
2   9   12
   /
  6


After deleting a node (5) with left and right children..
     10
     / \
    /   \
   /     \
  6      15
 / \     /
2   9   12


After deleting a node (10, root node) with left and right children..
   12
   / \
  6  15
 / \
2   9
```
