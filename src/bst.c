#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"


// Function to insert a node into a binary search tree (BST)
void insert(NodePtr* root, NodePtr insertion) {    
    // Insertion process for a simple BST
    NodePtr p; // Pointer for parent node
    NodePtr c; // Pointer for current node

    p = NULL; // Initialize parent to NULL
    c = *root; // Start with the root node

    // Traverse the tree to find the correct insertion point
    while (c != NULL) {
        p = c; // Update parent
        // Compare keys to determine traversal direction
        if (strcmp(insertion->key, c->key) < 0) {
            c = c->sx; // Go left
        } else {
            c = c->dx; // Go right
        }
        insertion->parent = p; // Set the parent of the new node
    }
    // Insert the new node in the tree
    if (p == NULL) {
        *root = insertion; // Tree was empty; new node is now the root
    } else {
        // Insert the node as a child of the parent
        if (strcmp(insertion->key, p->key) < 0) {
            p->sx = insertion; // Insert as left child
        } else {
            p->dx = insertion; // Insert as right child
        }
    }
    return; // End of function
}

// Function to insert a node into a BST
void insertRB(NodePtr* root, NodePtr rbNode, char* keys, int k, int mall, char* command) {
    // Assign the key to the node from the keys array
    rbNode[mall].key = &keys[(k + 1) * mall]; // Point to the correct position in the keys array
    memcpy(rbNode[mall].key, command, k); // Copy the command into the node's key
    rbNode[mall].key[k] = '\0'; // Null-terminate the string
    rbNode[mall].dx = NULL; // Initialize right child to NULL
    rbNode[mall].sx = NULL; // Initialize left child to NULL
    rbNode[mall].parent = NULL; // Initialize parent to NULL
    insert(root, &rbNode[mall]); // Insert the new node into the BST
}

// Function to search for a key in a binary search tree (BST)
NodePtr search(NodePtr root, char* key, int k) {
    NodePtr x = root; // Start with the root node
    // Traverse the tree to find the node with the matching key
    while (x != NULL && strncmp(key, x->key, k) != 0) {
        if (strncmp(key, x->key, k) < 0) {
            x = x->sx; // Go left if key is less
        } else {
            x = x->dx; // Go right if key is greater
        }
    }
    return x; // Return the found node (or NULL if not found)
}

// Function to delete the entire binary search tree
void deleteTree(NodePtr root) {
    if (root == NULL) {
        return; // Base case: empty tree
    }
    // Recursively delete left and right subtrees
    deleteTree(root->sx);
    deleteTree(root->dx);
    free(root->key); // Free the key string
    free(root); // Free the node itself
}

// Function to print all keys in the binary search tree in-order
void printTree(NodePtr root) {
    if (root == NULL) return; // Base case: return if the tree is empty
    // Traverse left subtree
    printTree(root->sx);
    printf("%s\n", root->key); // Print the key of the current node
    // Traverse right subtree
    printTree(root->dx);
}
