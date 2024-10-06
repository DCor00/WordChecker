#include <stdio.h>     // Standard input-output header
#include <stdlib.h>    // Standard library header for memory allocation
#include <string.h>    // String manipulation header

#define IN_SIZE 60    // Maximum size for input commands
#define BLOCK_SIZE 96 // Size of blocks for memory allocation

char command[IN_SIZE]; // Global command buffer

// Structure definition for a node in a binary search tree (BST)
struct node {
    struct node* sx;    // Pointer to the left child (sinistro)
    struct node* dx;    // Pointer to the right child (destro)
    struct node* parent; // Pointer to the parent node
    char* key;          // Key for the node
};
typedef struct node Node;      // Typedef for easy usage
typedef Node* NodePtr;         // Typedef for a pointer to a Node

// Structure definition for a constraint (vincolo)
struct vincolo {
    int appartiene;   // Flag: 1 if the symbol belongs to r, 0 otherwise
    int* posizioni;   // Array indicating positions (0: not allowed, 1: allowed)
    int n_minimo;     // Minimum number of times the symbol must appear in r
    int n_esatto;     // Exact number of times the symbol must appear in r
    char carattere;   // Character associated with the constraint
    struct vincolo* next; // Pointer to the next constraint in the list
};
typedef struct vincolo Vincolo;    // Typedef for easy usage
typedef Vincolo* VincoloPtr;        // Typedef for a pointer to a Vincolo

// Structure definition for a node in a linked list
struct listNode {
    char* key;              // Key for the list node
    struct listNode* next;  // Pointer to the next node in the list
};
typedef struct listNode ListNode;      // Typedef for easy usage
typedef ListNode* ListNodePtr;         // Typedef for a pointer to a ListNode

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

// Function to insert a node into a red-black tree (RB tree)
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

// Function to count occurrences of a character in a string
int countOccurrences(char s1[], char c, int k) {
    int i, count = 0; // Loop index and count variable
    // Loop through the string from the end to the beginning
    for (i = k; i--;) {
        if (s1[i] == c) {
            count++; // Increment count if character matches
        }
    }
    return count; // Return the total count
}

// Function to compare two words and update constraints based on their comparison
void confronta_parole(char p[], char r[], VincoloPtr* vincoli, int k) {
    int i, j; // Loop indices
    char* res = (char*)malloc(k + 1); // Allocate memory for the result string
    int occTot; // Total occurrences of a character
    VincoloPtr current; // Pointer to the current Vincolo

    // Loop through each character in the words
    for (i = 0; i < k; i++) {
        // Search for the current character in the list of Vincoli
        current = searchVincolo(*vincoli, p[i]);
        // If not found, insert a new Vincolo for the character
        if(current == NULL) {
            current = insertListVincolo(vincoli, p[i], k);
        }
        
        // If characters in both words match
        if (r[i] == p[i]) {
            res[i] = '+'; // Indicate a match

            int countR, countP; // Counters for occurrences
            current->appartiene = 1; // Mark the character as belonging

            current->posizioni[i] = 1; // Mark the position as occupied

            // Count occurrences of the character in both words
            countR = countOccurrences(r, p[i], k); // Count in r
            countP = countOccurrences(p, p[i], k); // Count in p

            // Update the minimum occurrence constraint
            if ((current->n_minimo == 0 || current->n_minimo < countR)) {
                if (countP < countR) {
                    current->n_minimo = countP; // Set minimum to occurrences in p
                } else {
                    current->n_minimo = countR; // Set minimum to occurrences in r
                }
            }
        }
        // If characters do not match
        else {
            occTot = countOccurrences(r, p[i], k); // Count occurrences of p[i] in r
            if (occTot == 0) {
                res[i] = '/'; // Indicate absence

                current->appartiene = 0; // Mark the character as not belonging

            } else {
                int occCorr = 0, scorrPre = 0; // Correct and previous occurrences
                // Loop through characters in reverse
                for (j = k; j--;) {
                    if (p[j] == r[j] && p[i] == p[j]) {
                        occCorr++; // Count correct occurrences of p[i]
                    }

                    // Check for previous occurrences that are incorrect
                    if (p[j] == p[i] && j < i && r[j] != p[j]) { 
                        scorrPre++; // Count previous occurrences that do not match
                    }
                }

                // If previous occurrences do not meet the required count
                if (!(scorrPre >= occTot - occCorr)) {
                    res[i] = '|'; // Indicate partial match

                    int countR, countP; // Counters for occurrences

                    current->appartiene = 1; // Mark the character as belonging

                    current->posizioni[i] = 0; // Mark the position as unoccupied
                    // Count occurrences in both words
                    countR = countOccurrences(r, p[i], k);
                    countP = countOccurrences(p, p[i], k);

                    // Update the minimum occurrence constraint
                    if ((current->n_minimo == 0 || current->n_minimo < countR)) {
                        if (countP < countR) {
                            current->n_minimo = countP; // Set minimum to occurrences in p
                        } else {
                            current->n_minimo = countR; // Set minimum to occurrences in r
                        }
                    }
                }
                // If previous occurrences meet the required count
                else {
                    res[i] = '/'; // Indicate absence

                    current->appartiene = 1; // Mark the character as belonging
                    current->posizioni[i] = 0; // Mark the position as unoccupied
                    current->n_esatto = occTot; // Set exact occurrences
                }
            }
        }
    }
    res[k] = '\0'; // Null-terminate the result string
    printf("%s", res); // Print the result
    free(res); // Free the allocated memory for result
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

// Function to print all keys in the linked list
void printList(ListNodePtr head) {
    ListNodePtr curr = head; // Pointer to the current node
    // Iterate through the linked list
    while (curr != NULL) {
        printf("%s\n", curr->key); // Print the key
        curr = curr->next; // Move to the next node
    }
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
