#include <stdio.h>
#include <stdlib.h>
#include "vincolo.h"

// Function to insert a new Vincolo into the linked list
VincoloPtr insertListVincolo(VincoloPtr* head, char key, int k) {
    int i; // Loop index
    VincoloPtr newNode = (VincoloPtr)malloc(sizeof(Vincolo)); // Allocate memory for a new Vincolo
    newNode->posizioni = (int*)malloc(k * sizeof(int)); // Allocate memory for positions
    newNode->carattere = key; // Set the character of the new Vincolo
    newNode->appartiene = 0; // Initialize appartiene to 0
    newNode->n_esatto = -1; // Initialize n_esatto to -1 (undefined)
    newNode->n_minimo = 0; // Initialize n_minimo to 0
    newNode->next = NULL; // Initialize the next pointer to NULL

    // Initialize the positions array to -1
    for (i = 0; i < k; i++) {
        newNode->posizioni[i] = -1;
    }

    // Insert the new node in the linked list in sorted order
    if (*head == NULL || (*head)->carattere > key) {
        newNode->next = *head; // Point next to the current head
        *head = newNode; // Update head to the new node
    } else {
        VincoloPtr prev = *head; // Pointer for the previous node
        VincoloPtr curr = (*head)->next; // Pointer for the current node
        // Traverse the list to find the correct insertion point
        while (curr != NULL && curr->carattere < key) {
            prev = curr; // Move prev to curr
            curr = curr->next; // Move curr to next
        }
        prev->next = newNode; // Insert newNode after prev
        newNode->next = curr; // Link the new node to curr
    }
    return newNode; // Return the new node
}

// Function to search for a Vincolo by character key in the linked list
VincoloPtr searchVincolo(VincoloPtr head, char key) {
    // Traverse the linked list to find the Vincolo with the given key
    while (head != NULL && head->carattere < key)
        head = head->next;

    // Return NULL if not found, otherwise return the found Vincolo
    if (head == NULL || head->carattere != key)
        return NULL;
    else
        return head;
}


// Function to check if a word satisfies all constraints
int soddisfa_vincoli(char* word, VincoloPtr vincoli, int k) {
    VincoloPtr current = vincoli; // Pointer to iterate through Vincoli
    // Loop through each Vincolo in the list
    while (current != NULL) {
        char carattere = current->carattere; // Get the character of the current Vincolo
        // Check if the Vincolo has been set
        if (current->appartiene != -1) {
            // If it should not belong
            if (current->appartiene == 0) {
                if (strchr(word, carattere) != NULL) {
                    return 0; // Return false if the character is found in the word
                }
            }
            int count = 0; // Counter for occurrences
            // If the character should belong
            if (current->appartiene == 1) {
                int i;
                // Loop through the word to check positions
                for (i = k; i--;) {
                    // Check for mandatory positions
                    if (current->posizioni[i] == 1 && word[i] != carattere) {
                        return 0; // Return false if required position does not match
                    }
                    // Check for prohibited positions
                    if (current->posizioni[i] == 0 && word[i] == carattere) {
                        return 0; // Return false if prohibited position matches
                    }
                    // Count occurrences of the character
                    if(word[i] == carattere) {
                        count++;
                    }
                }
            }
            
            // Check against the minimum occurrences requirement
            if (count < current->n_minimo) {
                return 0; // Return false if minimum is not met
            }
            // Check for exact occurrences requirement
            if (current->n_esatto != -1) {
                if (count != current->n_esatto) {
                    return 0; // Return false if exact count does not match
                }
            }
            count = 0; // Reset count for the next Vincolo
        }
        
        current = current->next; // Move to the next Vincolo
    }
    return 1; // Return true if all constraints are satisfied
}

// Function to free all nodes in the Vincolo list
void freeVincoli(VincoloPtr* head) {
    VincoloPtr curr = *head; // Pointer to the current Vincolo
    // Iterate through the linked list
    while (curr != NULL) {
        VincoloPtr temp = curr; // Store the current Vincolo to free
        curr = curr->next; // Move to the next Vincolo
        free(temp->posizioni); // Free the allocated positions memory
        free(temp); // Free the current Vincolo
    }
    *head = NULL; // Set head to NULL after freeing the list
}
