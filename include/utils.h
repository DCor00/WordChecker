#ifndef UTILS_H
#define UTILS_H

// Function to count occurrences of a character in a string
int countOccurrences(char s1[], char c, int k);

// Function to compare two words and update constraints based on their comparison
void confronta_parole(char p[], char r[], VincoloPtr* vincoli, int k);

// Function to sort the linked list using insertion sort
void insertionSortList(ListNodePtr* head);

#endif // UTILS_H
