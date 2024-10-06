#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "vincolo.h"


// Function to insert a new ListNode at the beginning of the list
void insertListNode(ListNodePtr* head, char* key, int k) {
    ListNodePtr newNode = (ListNodePtr)malloc(sizeof(ListNode)); // Allocate memory for a new ListNode
    newNode->key = (char*)malloc((k + 1) * sizeof(char)); // Allocate memory for the key string
    memcpy(newNode->key, key, k); // Copy the key into the new node
    newNode->key[k] = '\0'; // Null-terminate the key string
    newNode->next = *head; // Point the new node to the current head
    *head = newNode; // Update the head to the new node
}

// Function to update the word list by filtering out words that do not satisfy constraints
void update_word_list(ListNodePtr* head, VincoloPtr vincoli, int k, int* numFiltrate) {
    ListNodePtr prev = NULL; // Pointer to the previous node
    ListNodePtr curr = *head; // Pointer to the current node
    // Iterate through the linked list
    while (curr != NULL) {
        // Check if the current word satisfies the constraints
        if (!soddisfa_vincoli(curr->key, vincoli, k)) {
            // If not, decrement the filtered count
            (*numFiltrate)--; 
            // Remove the word from the list
            if (prev == NULL) {
                *head = curr->next; // Update head if at the start of the list
            } else {
                prev->next = curr->next; // Bypass the current node
            }
            ListNodePtr temp = curr; // Store the current node to free
            curr = curr->next; // Move to the next node
            free(temp->key); // Free the allocated key memory
            free(temp); // Free the current node
        } else {
            // If the word satisfies constraints, move to the next node
            prev = curr;
            curr = curr->next;
        }
    }
}

// Function to free all nodes in the word list
void free_word_list(ListNodePtr* head) {
    ListNodePtr curr = *head; // Pointer to the current node
    // Iterate through the linked list
    while (curr != NULL) {
        ListNodePtr temp = curr; // Store the current node to free
        curr = curr->next; // Move to the next node
        free(temp->key); // Free the allocated key memory
        free(temp); // Free the current node
    }
    *head = NULL; // Set head to NULL after freeing the list
}

// Function to print all keys in the linked list
void printList(ListNodePtr head) {
    ListNodePtr curr = head; // Pointer to the current node
    // Iterate through the linked list
    while (curr != NULL) {
        printf("%s\n", curr->key); // Print the key
        curr = curr->next; // Move to the next node
    }
}

// Function to update the filtered words list based on constraints
void updateFilteredWordsList(NodePtr root, VincoloPtr vincoli, ListNodePtr* filteredWordsList, int k, int* numFiltrate) {
    if (root == NULL) return; // Base case: return if tree is empty
    // Traverse left subtree
    updateFilteredWordsList(root->sx, vincoli, filteredWordsList, k, numFiltrate);
    // Check if the current node's key satisfies constraints
    if (soddisfa_vincoli(root->key, vincoli, k)) {
        (*numFiltrate)++; // Increment the filtered count
        insertListNode(filteredWordsList, root->key, k); // Insert the word into the filtered list
    }
    // Traverse right subtree
    updateFilteredWordsList(root->dx, vincoli, filteredWordsList, k, numFiltrate);
}

// Function to sort the linked list using insertion sort
void insertionSortList(ListNodePtr* head) {
    ListNodePtr sorted = NULL; // Pointer to the sorted list

    ListNodePtr curr = *head; // Pointer to the current node
    // Iterate through the original list
    while (curr != NULL) {
        ListNodePtr next = curr->next; // Store the next node

        // Insert current node into sorted list
        if (sorted == NULL || strcmp(curr->key, sorted->key) < 0) {
            curr->next = sorted; // Insert at the beginning
            sorted = curr; // Update sorted to the current node
        } else {
            ListNodePtr currSorted = sorted; // Pointer to traverse the sorted list
            // Find the correct position to insert
            while (currSorted->next != NULL && strcmp(curr->key, currSorted->next->key) > 0) {
                currSorted = currSorted->next; // Move to next node
            }
            curr->next = currSorted->next; // Link current node to the next sorted node
            currSorted->next = curr; // Insert current node into sorted list
        }

        curr = next; // Move to the next node in the original list
    }

    *head = sorted; // Update the head to the sorted list
}
