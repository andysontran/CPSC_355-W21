/*
Author: Tran, Andy (Son)
UCID: 30019100
Course: CPSC 355 W2021

Description: 
Written in C, a program to emulate a search engine. The program reads a table of integers from a file 
or randomly creates the table and displays it to the user. The rows of this table represent text documents, and the
columns represent words that may appear in these text documents. Each cell with coordinates [i,j] in the table 
contains the number of occurrences of word j in document i. The program returns a list of the top n documents containing word j. 
The user specifies both n and j. 
*/

// import necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// define fixed table dimensions and constant values used in the program
#define DOCS 7       
#define WORDS 10
#define RANDMIN 0
#define RANDMAX 9
#define MIN 0

// begin function prototype listing
int randomNum (int *min, int *max);
void display (int (*table)[WORDS]);
void initializeFile (int (*table)[WORDS], FILE *inf);
void initializeRand (int (*table)[WORDS], int *min, int *max);
void fillArr (int *arr);
int userPrompt (const char *msg, int *min, int *max);
int* topRelevantDocs (int (*table)[WORDS], int *ind, int *doc);
void logToFile (char *argv[], int (*table)[WORDS], int *top, int *ind, int *doc, FILE *ouf);

int main (int argc, char *argv[]) {
    // begin variable listing
    int table[DOCS][WORDS] = {{}};          // init 7x10 table and populate with zeros
    FILE *inf, *ouf;                        // init input/output file pointers
    int min = RANDMIN, max = RANDMAX;       // init min/max range for random values
    int indmin = MIN, indmax = WORDS - 1;   // init min/max range for indices
    int docmin = MIN, docmax = DOCS;        // init min/max range for documents
    // user input validation for command-line arguments
    if (argc > 2) { // user passes more than one command-line argument
        printf("Too many arguments passed.\n");
        printf("Please only enter the filename.txt or leave blank to generate random values.\n");
        exit(1); // exit program   
    }
    switch (argc) { 
        case (2): // user passes one command-line argument
            if ( (inf = fopen(argv[1], "r") ) == NULL ) { // if the file DNE
                printf("Error Occurred. Cannot open file: %s\n", argv[1]);
                printf("Either the filename is incorrect or it does not exist.\n");
                exit(1); // exit program
            }    
            inf = fopen(argv[1], "r"); // if file exists, read from file
            printf("The file %s is opened.\n", argv[1]);
            printf("Initializing table with provided values.\n");
            initializeFile(table, inf); // call initializeFile to initialize table using values from provided file 
            break;
        case (1): // user passes no command-line arguments
            printf("No arguments passed.\n"); 
            printf("Initializing table with random values.\n");
            initializeRand(table, &min, &max); // call initializeRand to initialize table using random values
            break;
    }
    display(table);
    // call userPrompt and initialize index from user input
    int ind = userPrompt("Enter the index of the word you are searching for", &indmin, &indmax); 
    // call userPrompt and initialize doc from user input
    int doc = userPrompt("How many top documents do you want to retrieve", &docmin, &docmax); 
    int* top = topRelevantDocs(table, &ind, &doc); // call topRelevantDocs to begin search 
    logToFile(argv, table, top, &ind, &doc, ouf); // call logToFile for documentation
    return 0; // finish execution
}

/* Function: randomNum

Uses system time in order to generate a unique integer value bounded by min and max with each call

Parameters: 

    *min - The lowerbound integer
    *max - The upperbound integer

Returns:

    num - The randomly generated integer value 
*/
int randomNum(int *min, int *max) { 
    static bool first = true; 
    if (first) { 
        srand( time(NULL) ); // initialize system time as seed value for rand
        first = false;
    }
    int num = rand() % (*max - *min + 1) + *min; // call rand and arithmetics for value boundaries
    return num; // return random int
}

/* Function: display

Displays to the user a 7x10 table of random/given positive integers between 0 and 9

Parameters: 

    (*table)[WORDS] - The 7x10 array of random/given positive integers
*/
void display(int (*table)[WORDS]) { 
    int i, j;
    for (i = 0; i < DOCS; i++) { // loop through array rows i
        for (j = 0; j < WORDS; j++) { // loop through array columns j
            printf("%d ", table[i][j]); // print value at [i,j]
        }
        printf("\n");
    }
}

/* Function: initializeFile

Modifies a 7x10 table with the provided positive integers in the file passed by the user

Parameters: 

    (*table)[WORDS] - The 7x10 empty array (initialized to zeros)
    *inf - The input file provided by the user  
*/
void initializeFile(int (*table)[WORDS], FILE *inf) {
    for (int i = 0; i < DOCS; i++) { // loop through array rows i
        for (int j = 0; j < WORDS; j++) { // loop through array columns j
            fscanf(inf, "%d", &table[i][j]); // assign value of inf to [i,j]
        }
    }
    fclose(inf); // close file
}

/* Function: initializeRand

Modifies a 7x10 table with randomly generated positive integers

Parameters: 

    (*table)[WORDS] - The 7x10 empty array (initialized to zeros)
    *min - The lower bound integer
    *max - The upper bound integer  
*/
void initializeRand(int (*table)[WORDS], int *min, int *max) { 
    for (int i = 0; i < DOCS; i++) { // loop through array rows i
        for (int j = 0; j < WORDS; j++) { // loop through array columns j
            table[i][j] = randomNum(min, max); // call randomNum with bounds min, max and assign result to [i,j]
        }
    }
}

/* Function: topRelevantDocs

Searches the 7x10 table for the user-specified index of the word and the number of top documents containing the word

Parameters: 

    (*table)[WORDS] - The 7x10 array of random/given positive integers
    *ind - The user specified index to search
    *doc - The user specified number of top documents to return

Returns:

    top - The array containing doc number of indices of the top documents from search
*/
int* topRelevantDocs(int (*table)[WORDS], int *ind, int *doc) {
    // begin variable listing
    int index[DOCS];                // init index array
    float freq[DOCS];               // init freq array
    static int top[DOCS];           // init top array to be returned
    int i, j, max;                  // init i, j, k vars for loop
    float temp;                     // init max, temp vars for sorting  
    fillArr(index);                 // populate index array with indices
    for (i = 0; i < DOCS; i++) { 
       freq[i] = 0.0;
       for (j = 0; j < WORDS; j++) {
           freq[i] = freq[i] + table[i][j]; // assign size of i'th document to freq[i]           
       }
       freq[i] = table[i][*ind] / freq[i]; // divide frequency of word at index by size of i'th document, assign result
    }
    // partial selection sort, move doc max elements to the front of freq array
    for (i = 0; i < *doc; i++) { // loop for as many instances as assigned by user for number of top documents
        max = i; // record position of first element in freq array        
        for (j = i + 1; j < DOCS; j++) { 
            if (freq[j] > freq[max]) { 
                max = j; // record position of j as largest element thus far
            }
        }
        temp = freq[i];             // store value of freq[i]  
        freq[i] = freq[max];        // replace freq[i] with freq[max]
        freq[max] = temp;           // replace temp value (old freq[i]) at index of freq[max]
        // now assign to index 
        temp = index[i];            // store value of index[i]
        index[i] = index[max];      // replace index[i] with index[max]
        index[max] = temp;          // replace temp value (old index[i] at index of index[max])
    } // array sorting ends
    printf("The top %d documents with the highest frequencies of the searched word at index %d are:\n", *doc, *ind);
    for (i = 0; i < *doc; i++) {
        top[i] = index[i]; // fill top array with doc number of indicies from sorted (top-bottom) index array
        printf("Document %d\n", top[i]);
    }
    return top; // return top array
}

/* Function: logToFile

Creates a log file named assign1.log before exiting the program containing the following: the initial table, user 
inputs and search results 

Parameters: 

    *argv[] - The command-line arguments passed by the user
    (*table)[WORDS] - The 7x10 array of random/given positive integers
    *top - The array containing doc number of indices of the top documents from search
    *ind - The user specified index to search
    *doc - The user specified number of top documents to return
    *ouf - The output file
*/
void logToFile (char *argv[], int (*table)[WORDS], int *top, int *ind, int *doc, FILE *ouf) {
    ouf = fopen("assign1.log", "w"); // open assign1.log file for writing 
    if (argv[1] == NULL) { // if the user passed no command-line arguments 
        fprintf(ouf, "No arguments passed.\n");
        fprintf(ouf, "Initializing table with random values.\n");
    } else { // if the user passed a file and it exists
        fprintf(ouf, "User passed an argument. The file %s is opened.\n", argv[1]);
        fprintf(ouf, "Initializing table with provided values.\n");
    }
    int i, j;
    for (i = 0; i < DOCS; i++) { 
        for (j = 0; j < WORDS; j++) {
            fprintf(ouf, "%d ", table[i][j]); // prints the 7x10 table to file
        }
        fprintf(ouf, "\n");
    }
    fprintf(ouf, "User input %d for index and %d for number of top documents to retrieve.\n", *ind, *doc); // user inputs
    fprintf(ouf, "The top %d documents with the highest frequencies of the searched word at index %d are:\n", *doc, *ind);   
    for (i = 0; i < *doc; i++) {
        fprintf(ouf, "Document %d\n", top[i]); // prints top document search results to file
    }
    fclose(ouf); // close file 
    printf("Log file generated.\n");
}

// Additional functions

/* Function: fillArr

Fills array with indices of number equal to indices of documents in the table

Parameters: 

    *arr - The array to be modified
*/
void fillArr(int *arr) {
    for (int i = 0; i < DOCS; ++i) { // populate arr[i] with i until last index of documents
        arr[i] = i; 
    }
}

/* Function: userPrompt

Formats for user prompts and performs input validation

Parameters: 

    *msg - The message to be shown to the user in the terminal
    *min - The lowerbound integer
    *max - The upperbound integer

Returns:

    value - The user-inputted integer
*/
int userPrompt (const char *msg, int *min, int *max) {
    // begin variable listing
    int val;                  // init value var for return
    int rc;                     // init rc var to check sscanf integer value
    char line[1024];            // init line array to hold user input
    printf("%s (from %d to %d): ", msg, *min, *max);
    // parse user input using fgets in line buffer and assign to value, check for bounds
    while (fgets(line, sizeof(line), stdin) != 0 && (rc = sscanf(line, "%d", &val)) == 1 && (val < *min || val > *max)) { 
        printf("Invalid input. Enter a value between %d and %d: ", *min, *max); // user input not within bounds, try again
    }
    while (rc != 1) { // user inputted non-integer, prevent infinite loop 
        printf("Error reading input. Program exiting.\n"); 
        exit(1); // exit program
    }
    return val; // return user input
}