#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vincolo.h"

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
