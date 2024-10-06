#ifndef VINCOLO_H
#define VINCOLO_H

typedef struct vincolo {
    int appartiene;  // 1 if symbol belongs to r, 0 otherwise
    int* posizioni;  // positions constraints
    int n_minimo;    // minimum occurrences
    int n_esatto;    // exact occurrences
    char carattere;
    struct vincolo* next;
} Vincolo;

typedef Vincolo* VincoloPtr;

// Function to insert a new Vincolo into the linked list
VincoloPtr insertListVincolo(VincoloPtr* head, char key, int k);

// Function to search for a Vincolo by character key in the linked list
VincoloPtr searchVincolo(VincoloPtr head, char key);

// Function to check if a word satisfies all constraints
int soddisfa_vincoli(char* word, VincoloPtr vincoli, int k);

// Function to free all nodes in the Vincolo list
void freeVincoli(VincoloPtr* head);

#endif // VINCOLO_H
