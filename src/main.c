#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "vincolo.h"
#include "list.h"
#include "utils.h"

#define IN_SIZE 60    // Maximum size for input commands
#define BLOCK_SIZE 96 // Size of blocks for memory allocation

char command[IN_SIZE]; // Global command buffer

int main() {
    int k; // Length of the words (k)
    // Read the length of the words from standard input
    if (fscanf(stdin, "%d ", &k) == EOF) {
        return 0; // Return if there's an error in reading
    }

    ListNodePtr filtered = NULL; // Pointer to the filtered list of words
    NodePtr root = NULL; // Pointer to the root of the binary tree
    NodePtr rWord; // Pointer for the word to guess
    NodePtr rbNode; // Pointer for BST nodes
    char* keysTree; // Array to hold keys for the tree
    int numFiltrate = 0; // Count of filtered words
    int numTot = 0; // Total number of words
    int mall; // Variable to manage memory allocation
    int flag = 0; // Flag to check if any filtered words have been processed
    VincoloPtr vincoli = NULL; // Pointer to constraints list
    int tentativi, current = 0, flagOK = 0; // Variables for attempts and status flags

    // Read the command from standard input
    if (fgets(command, IN_SIZE, stdin) == NULL) {
        return 0; // Return if there's an error in reading the command
    }
    // Allocate memory for BST nodes and keys
    rbNode = (NodePtr)malloc(sizeof(Node) * BLOCK_SIZE);
    keysTree = (char*)malloc((k + 1) * BLOCK_SIZE);
    mall = 0; // Initialize memory allocation counter

    // Loop to read words until a command starts with '+'
    while (command[0] != '+') {
        // Check if memory allocation limit has been reached
        if (mall == BLOCK_SIZE) {
            rbNode = (NodePtr)malloc(sizeof(Node) * BLOCK_SIZE); // Allocate more nodes
            keysTree = (char*)malloc((k + 1) * BLOCK_SIZE); // Allocate more keys
            mall = 0; // Reset memory allocation counter
        }
        insertRB(&root, rbNode, keysTree, k, mall, command); // Insert word into the BST
        mall++; // Increment memory allocation counter
        numTot++; // Increment total words count
        numFiltrate++; // Increment filtered words count

        // Read the next command
        if (fgets(command, IN_SIZE, stdin) == NULL) {
            return 0; // Return if there's an error in reading
        }
    }
    
    do {
        if (command[1] == 'n') { // If the command is 'new game'
            // Read the word to guess
            if (fgets(command, IN_SIZE, stdin) == NULL) { // Read the guess word
                return 0; // Return if there's an error in reading
            }
            command[k] = '\0'; // Null-terminate the command string
            rWord = search(root, command, k); // Search for the guess word in the tree

            // Read the number of attempts allowed
            if (fscanf(stdin, "%d ", &tentativi) == EOF) {
                return 0; // Return if there's an error in reading
            }

            // Loop for attempts until they run out or the word is guessed correctly
            while (tentativi > current && !flagOK) {
                // Read the next guess from input
                if (fgets(command, IN_SIZE, stdin) == NULL) { // Read the guess word
                    return 0; // Return if there's an error in reading
                }

                if (command[0] == '+') { // If the command starts with '+', a game command
                    if (command[1] == 'i') { // If the command is 'input words'
                        // Read the words until another '+' is encountered
                        if (fgets(command, IN_SIZE, stdin) == NULL) {
                            return 0; // Return if there's an error in reading
                        }

                        while (command[0] != '+') { // Continue until '+' is found
                            // Check if memory allocation limit has been reached
                            if (mall == BLOCK_SIZE) {
                                rbNode = (NodePtr)malloc(sizeof(Node) * BLOCK_SIZE); // Allocate more nodes
                                keysTree = (char*)malloc((k + 1) * BLOCK_SIZE); // Allocate more keys
                                mall = 0; // Reset memory allocation counter
                            }
                            insertRB(&root, rbNode, keysTree, k, mall, command); // Insert word into the BST
                            mall++; // Increment memory allocation counter
                            numTot++; // Increment total words count

                            // Check if the word satisfies the constraints
                            if (soddisfa_vincoli(command, vincoli, k)) {
                                numFiltrate++; // Increment filtered words count
                                if (flag) {
                                    insertListNode(&filtered, command, k); // Add to filtered list
                                }
                            }

                            // Read the next command
                            if (fgets(command, IN_SIZE, stdin) == NULL) {
                                return 0; // Return if there's an error in reading
                            }
                        }
                    }
                    else { // If the command is not for input
                        if (command[1] == 's') { // If the command is 'show'
                            if (!flag) {
                                printTree(root); // Print the binary tree
                            }
                            else {
                                insertionSortList(&filtered); // Sort the filtered list
                                printList(filtered); // Print the filtered list
                            }
                        }
                    }
                }
                else { // If it's a guess attempt
                    if (!strncmp(command, rWord->key, k)) { // If the guess matches the word
                        printf("ok\n"); // Print success message
                        flagOK = 1; // Set success flag
                    }
                    else { // If the guess does not match
                        if (search(root, command, k) == NULL) { // Check if the guess exists
                            printf("not_exists\n"); // Print message if it doesn't
                        }
                        else { // If the guess exists
                            confronta_parole(command, rWord->key, &vincoli, k); // Compare the guess with the word

                            if (!flag) { // If no words have been filtered yet
                                numFiltrate = 0; // Reset the filtered count
                                updateFilteredWordsList(root, vincoli, &filtered, k, &numFiltrate); // Update the filtered words list
                                flag = 1; // Set flag to indicate filtering is done
                            }
                            else {
                                update_word_list(&filtered, vincoli, k, &numFiltrate); // Update filtered words list again
                            }
                            printf("\n%d\n", numFiltrate); // Print the count of filtered words
                            current++; // Increment the current attempt count
                        }
                    }
                }
            }

            // Reset variables for the next round
            current = 0; // Reset current attempts
            command[1] = '\0'; // Null-terminate command
            if (!flagOK) {
                printf("ko\n"); // Print failure message if word was not guessed
            }
            numFiltrate = numTot; // Reset filtered count to total count
            flagOK = 0; // Reset success flag
            flag = 0; // Reset filter flag
            free_word_list(&filtered); // Free the filtered word list
            freeVincoli(&vincoli); // Free the constraints list
        }
        else { // If the command is not for a new game
            if (command[1] == 's') { // If the command is 'show'
                if (!flag) {
                    printTree(root); // Print the binary tree
                }
                else {
                    insertionSortList(&filtered); // Sort the filtered list
                    printList(filtered); // Print the filtered list
                }
            }
            else { // If the command is not 'show'
                if (command[1] == 'i') { // If the command is 'input words'
                    if (fgets(command, IN_SIZE, stdin) == NULL) {
                        return 0; // Return if there's an error in reading
                    }

                    // Read words until a '+' command is encountered
                    while (command[0] != '+') { 
                        // Check if memory allocation limit has been reached
                        if (mall == BLOCK_SIZE) {
                            rbNode = (NodePtr)malloc(sizeof(Node) * BLOCK_SIZE); // Allocate more nodes
                            keysTree = (char*)malloc((k + 1) * BLOCK_SIZE); // Allocate more keys
                            mall = 0; // Reset memory allocation counter
                        }
                        insertRB(&root, rbNode, keysTree, k, mall, command); // Insert word into the BST
                        mall++; // Increment memory allocation counter
                        numTot++; // Increment total words count

                        // Check if the word satisfies the constraints
                        if (soddisfa_vincoli(command, vincoli, k)) {
                            numFiltrate++; // Increment filtered words count

                            if (flag) {
                                insertListNode(&filtered, command, k); // Add to filtered list
                            }
                        }

                        // Read the next command
                        if (fgets(command, IN_SIZE, stdin) == NULL) {
                            return 0; // Return if there's an error in reading
                        }
                    }
                }
            }
        }

    } while (fgets(command, IN_SIZE, stdin) != NULL); // Continue reading until EOF
    // End of main function

    return 0; // Return 0 to indicate successful execution
}
