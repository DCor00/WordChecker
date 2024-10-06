#ifndef BST_H
#define BST_H

typedef struct node {
    struct node* sx; // left child
    struct node* dx; // right child
    struct node* parent;
    char* key;
} Node;

typedef Node* NodePtr;

// Function to insert a node into a binary search tree (BST)
void insert(NodePtr* root, NodePtr insertion);

// Function to search for a key in a binary search tree (BST)
NodePtr search(NodePtr root, char* key, int k);

// Function to delete the entire binary search tree
void deleteTree(NodePtr root);

// Function to insert a node into a BST
void insertRB(NodePtr* root, NodePtr rbNode, char* keys, int k, int mall, char* command);

// Function to print all keys in the binary search tree in-order
void printTree(NodePtr root);

#endif // BST_H
