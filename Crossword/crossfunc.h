// crossfunc.h
// header file for crossword.c
// Yanni Vu

#define _GNU_SOURCE // to use strfry
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define maxLetters 15+1 // +1 for the null character
#define maxWords 20
#define szBoard 15

// Words struct containing necessary information
typedef struct {
    char word[maxLetters];
    char shuffled[maxLetters];
    char orientation[11];
    int length;
    int startRow;
    int startCol;
} Words;

// function prototypes
void upper(char word[maxLetters]);
int readData(Words words[]);
int fileRead(Words words[], char fname[]);
void initSoln(char board[][szBoard]);
void initPuzzle(char board[][szBoard]);
void initClues(Words words[], int numWords);
void sortLength(Words words[], int numWords);
void incommon(Words words[], char incommon[], int word1, int word2);
void placeWords(Words words[], char soln[][szBoard], char puzzle[][szBoard], int numWords);
void placeError(FILE *fp, Words words[], int numWords);
void shuffle(Words words[], int numWords);
void dispBoard(char board[][szBoard]);
void clues(Words words[], int numWords);
void fClues(FILE *fp, Words words[], int numWords);
void printFile(FILE *fp, char board[][szBoard]);
