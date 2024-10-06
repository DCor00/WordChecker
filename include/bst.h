#ifndef BST_H
#define BST_H

typedef struct node {
    struct node* sx; // left child
    struct node* dx; // right child
    struct node* parent;
    char* key;
} Node;

typedef Node* NodePtr;

void insert(NodePtr* root, NodePtr insertion);
NodePtr search(NodePtr root, char* key, int k);
void deleteTree(NodePtr root);
void insertRB(NodePtr* root, NodePtr rbNode, char* keys, int k, int mall, char* command);
void printTree(NodePtr root);

#endif // BST_H
