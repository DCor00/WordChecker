#ifndef LIST_H
#define LIST_H

typedef struct listNode {
    char* key;
    struct listNode* next;
} ListNode;

typedef ListNode* ListNodePtr;

// Function to insert a new ListNode at the beginning of the list
void insertListNode(ListNodePtr* head, char* key, int k);

// Function to free all nodes in the word list
void free_word_list(ListNodePtr* head);

// Function to print all keys in the linked list
void printList(ListNodePtr head);

// Function to update the word list by filtering out words that do not satisfy constraints
void update_word_list(ListNodePtr* head, VincoloPtr vincoli, int k, int* numFiltrate);

// Function to update the filtered words list based on constraints
void updateFilteredWordsList(NodePtr root, VincoloPtr vincoli, ListNodePtr* filteredWordsList, int k, int* numFiltrate);

#endif // LIST_H
