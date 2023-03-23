// crossword.c
// This program generates a crossword from user-given words and 
// is capable of outputting this game to the screen or a file.
// Yanni Vu

#include "crossfunc.h"

int main(int argc, char *argv[])
{
    // initialize solution, puzzle, number of words, and Word struct
    char soln[szBoard][szBoard], puzzle[szBoard][szBoard];
    Words words[maxWords];
    int numWords;

    // if the user does not enter a filename to read from or write to
    if (argc == 1) {
    numWords = readData(words); // read words in and count the number of words
    sortLength(words, numWords);    // sort using bubble sort
    initPuzzle(puzzle); // initialize puzzle board
    initSoln(soln); // initialize solution board
    initClues(words, numWords); // initialize clues
    placeWords(words, soln, puzzle, numWords);  // place words function
    placeError(NULL, words, numWords);  // display error messages 
    shuffle(words, numWords);   // shuffle the words using strfry
    printf("SOLUTION:\n");
    dispBoard(soln);    // display the board solution board
    printf("PUZZLE:\n");
    dispBoard(puzzle);  // display the puzzle board
    clues(words, numWords); // display the clues
    }

    // if the user only enters a filename to read from
    // all functions same as above except fileRead
    else if (argc == 2) {
    numWords = fileRead(words, argv[1]); // read words from file
    sortLength(words, numWords);
    initPuzzle(puzzle);
    initSoln(soln);
    initClues(words, numWords);
    placeWords(words, soln, puzzle, numWords);
    placeError(NULL, words, numWords);
    shuffle(words, numWords);
    printf("SOLUTION:\n");
    dispBoard(soln);
    printf("PUZZLE:\n");
    dispBoard(puzzle);
    clues(words, numWords);
    }

    // if the user enters two filenames for read and write
    // all functions same as above except file printing
    else if (argc == 3) {
    FILE *fp = fopen(argv[2], "w"); // open file for writing
    numWords = fileRead(words, argv[1]);
    sortLength(words, numWords);
    initPuzzle(puzzle);
    initSoln(soln);
    initClues(words, numWords);
    placeWords(words, soln, puzzle, numWords);
    placeError(fp, words, numWords);
    shuffle(words, numWords);
    fprintf(fp, "SOLUTION:\n");
    printFile(fp, soln);
    fprintf(fp, "PUZZLE:\n");
    printFile(fp, puzzle);
    fClues(fp, words, numWords);
    fclose(fp);
    }


    return 0;
}