## Submitter info
* Author: Tran, Andy (Son)
* UCID: #########
* Course: CPSC 355 W2021

## Assignment 1 info 
This assignment involves a program written in C to emulate a search engine. The program reads a table of integers from a file 
or randomly creates the table and displays it to the user. The rows of this table represent text documents, and the
columns represent words that may appear in these text documents. Each cell with coordinates [i,j] in the table 
contains the number of occurrences of word j in document i. The program returns a list of the top n documents containing word j. 
The user specifies both n and j. 

## Technologies
Assignment is created with:
* C compiler: gcc (GCC) 8.3.1 20190223 (Red Hat 8.3.1-2)

## Setup
To run this assignment, download the necessary files to root:
* assign1.c
* assign1.script
* occurrences.txt 

```
$ assign1.script
```

## Implementation
Imported libraries:
* stdio.h:      For I/O functions
* stdlib.h:     Standard libraries  
* time.h:       For rand function to use system time as a seed value 
* stdbool.h:    Declare boolean values to prepare rand function 

Defined constants: -- Used to prevent from hardcoding the bounds. 
* DOCS = 7:     Num of rows/documents expected
* WORDS = 10:   Num of columns/words expected
* RANDMIN = 0:  Lower bound for RNG
* RANDMAX = 0:  Upper bound for RNG
* MIN = 0:      Minimum value for documents/words/indices

Neccessary functions:
* initializeFile
    * Modifies a 7x10 table with the provided positive integers in the file passed by the user.
        * Parameters: 
            * (*table)[WORDS] - The 7x10 empty array (initialized to zeros)
            * *inf - The input file provided by the user
* initializeRand
    * Modifies a 7x10 table with randomly generated positive integers.
        * Parameters: 
            * (*table)[WORDS] - The 7x10 empty array (initialized to zeros)
            * *min - The lower bound integer
            * *max - The upper bound integer
* randomNum
    * Uses system time in order to generate a unique integer value bounded by min and max with each call.
        * Parameters: 
            * *min - The lowerbound integer
            * *max - The upperbound integer
        * Returns:
            * num - The randomly generated integer value 
* display
    ( Displays to the user a 7x10 table of random/given positive integers between 0 and 9.
        * Parameters: 
            * (*table)[WORDS] - The 7x10 array of random/given positive integers
* topRelevantDocs
    * Searches the 7x10 table for the user-specified index of the word and the number of top documents containing the word.
        * Parameters: 
            * (*table)[WORDS] - The 7x10 array of random/given positive integers
            * *ind - The user specified index to search
            * *doc - The user specified number of top documents to return
        * Returns:
            * top - The array containing doc number of indices of the top documents from search
* logToFile
    * Creates a log file named assign1.log before exiting the program containing the following: the initial table, user inputs and search results.
        * Parameters: 
            * *argv[] - The command-line arguments passed by the user
            * (*table)[WORDS] - The 7x10 array of random/given positive integers
            * *top - The array containing doc number of indices of the top documents from search
            * *ind - The user specified index to search
            * *doc - The user specified number of top documents to return
            * *ouf - The output file

Additional functions: -- Modularity for reused code.
* fillArr
    * Fills array with indices of number equal to indices of documents in the table.
        * Parameters: 
            * *arr - The array to be modified
* userPrompt
    * Formats for user prompts and performs input validation.
        * Parameters: 
            * *msg - The message to be shown to the user in the terminal
            * *min - The lowerbound integer
            * *max - The upperbound integer
        * Returns:
            * val - The user-inputted integer

Possible sequences:
* The user provides a command-line argument containing a filename:
    * Too many arguments:
        * Program checks that argc > 2.
        * Throws error and exits program.
    * Filename is incorrect/DNE:
        * Program checks that inf when opening argv[1] == NULL.
        * Throws error and exits program.
    * Filename exists:
        * Program reads file and initializes table from provided values using initializeFile function.
        * Calls display function to print table.
        * Gives user prompts for desired index and number of top documents to search using userPrompt function.
            * Validates user input for appropriate integer values within specified boundaries.
            * If the user enters a non-integer value, throws error and exits program to prevent infinite looping. 
        * Searches table for user-defined index and number of top documents using topRelevantDocs function.
            * Calculate frequency of word j in document i using: occurrences[i,k] / size of document i. 
                * Size of document i (row i) = Sum of occurrences of all of its words (sum of all columns in row i).
            * Apply partial selection sort (using temp variable) to sort frequencies.
            * Match indices with sorted frequencies.
            * Return the indices of user-specified number of top documents from search.
        * Creates a log file to store: the initial table, user inputs and search results.
        * Program completes execution. 
* The user provides no command-line arguments:
    * Similar to "Filename exists" but instead uses randomly generated values from the randomNum function.
        * Uses system time as a seed for RNG. 
        * Generates a random number in between 0 to (max - min + 1) and adds the min to offset.
