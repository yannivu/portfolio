// crossfunc.c
// functions file for crossword.c
// Yanni Vu

#include "crossfunc.h"


void upper(char word[maxLetters])
{
    for (int i = 0; i < maxLetters; i++) {
        if (word[i] >= 'a' && word[i] <= 'z') {     // if they enter the whole name then make the first letter uppercase so it is matches
            word[i] = word[i] - 32;
        }
    }
}

int readData(Words words[])
{
    printf("Enter a list of words: \n");

    char word[256];
    int count = 0;
    int nonletter = 0;
    while(1) {
        scanf("%s", &word); // scan words in
        for (int i = 0; i < strlen(word); i++) {
            if ((isdigit(word[i]) != 0) || (isalpha(word[i])==0)){ 
                nonletter = 1;  // flag if the word contains a nonletter
            }
        }
        if ((strlen(word) <= maxLetters) && (strcmp(word,".") != 0) && (nonletter != 1) && (strlen(word) >= 2)) {
            upper(word);    // convert the word into all uppercase
            strcpy(words[count].word, word); // copy word
            count++;
        }
        else if (strcmp(word, ".") == 0 || count == maxWords)
            break; // stop when user enters period
        else {
            printf("INVALID WORD. Enter another: "); // show error and allow for user to enter another word
        }
        nonletter = 0;
    }

    return count;
}

int fileRead(Words words[], char fname[])
{
    FILE *fp = fopen(fname, "r");   // open file for reading
    if(fp == NULL) {
        perror("Error in opening file");    // return error if file does not exist
    }
    char word[256];
    int count = 0;
    int nonletter = 0;
    while(1) {
        fscanf(fp, "%s", &word); // scan words
        for (int i = 0; i < strlen(word); i++) {
            if ((isdigit(word[i]) != 0) || (isalpha(word[i])==0)){ // check if not a letter
                nonletter = 1;
            }
        }
        if ((strlen(word) <= maxLetters) && (strcmp(word,".") != 0) && (nonletter != 1) && (strlen(word) >= 2)) { // copy word if parameters hold
            upper(word);
            strcpy(words[count].word, word);
            count++;
        }
        nonletter = 0;
        if (strcmp(word, ".") == 0 || count == maxWords)
            break; // break if the last entry is a period
        if (feof(fp)) break; // break at the end of the file
    }
    fclose(fp);
    return count;
}

void initSoln(char board[][szBoard]) // function to initialize the board
{
    int i,j;
    for (i = 0; i < szBoard; i++) {
        for (j = 0; j < szBoard; j++) {
            board[i][j] = '.'; // set all spots to a blank space
        }
    }
}

void initPuzzle(char board[][szBoard]) // function to initialize the board
{
    int i,j;
    for (i = 0; i < szBoard; i++) {
        for (j = 0; j < szBoard; j++) {
            board[i][j] = '#'; // set all spots to a blank space
        }
    }
}

void initClues(Words words[], int numWords) // initialize so that startCol and startRow print as 0
{
    for (int i = 0; i < numWords; i++) {
        words[i].startCol = -1;
        words[i].startRow = -1;
        strcpy(words[i].orientation, "Can\'t Place"); // show cant place for direction if it was not placed
    }
}

void sortLength(Words words[], int numWords)
{
    char temp[maxLetters];
 
    // sorting strings using bubble sort
    for (int i=0; i < numWords-1; i++)
    {
        for (int j=0; j<numWords-1-i; j++)
        {
            if (strlen(words[j].word) < strlen(words[j+1].word))
            {
                strcpy(temp, words[j].word);
                strcpy(words[j].word, words[j+1].word);
                strcpy(words[j+1].word, temp);
            }
        }
    }
    for (int i=0; i < numWords-1; i++) {
        words[i].length = strlen(words[i].word);
    }

}

void incommon(Words words[], char incommon[maxLetters], int word1, int word2) // find letters in common
{
    int letternum = 0;
    for (int i = 0; i < words[word1].length; i++) {
        for (int j = 0; j < words[word2].length; j++) {
            if ((words[word1].word[i] == words[word2].word[j])) {
                incommon[letternum] = words[word1].word[i]; // add to array if letters are in common
                letternum++;
            }
        }
    }
    incommon[letternum] = '\0';
}

int findCol(Words words[], char soln[][szBoard], char common[maxLetters], char commonCol[], int word1, int word2) // find columns of respective letters in common
{
    incommon(words, common, word1, word2);
    int colNum = 0;
    for (int i = 0; i < szBoard; i++) {
        for (int j = 0; common[j] != '\0'; j++) {
            if (soln[words[word1].startRow][i] == common[j]) {
                commonCol[colNum] = i; // add common columns to array
                colNum++;
            }
        }
    }
    return colNum; // return number of common columns
}

void placeVert(Words words[], char soln[][szBoard], char puzzle[][szBoard], int index) // place words vertically
{
    for (int i = words[index].startRow; i < words[index].length+words[index].startRow; i++) {
        soln[i][words[index].startCol] = words[index].word[i-words[index].startRow]; // place in solution board
        puzzle[i][words[index].startCol] = ' '; // place blank in puzzle board
        }
}

void placeHoriz(Words words[], char soln[][szBoard], char puzzle[][szBoard], int index)
{
    for (int k = words[index].startCol; k < words[index].length+words[index].startCol; k++) {
        soln[words[index].startRow][k] = words[index].word[k-words[index].startCol]; // place in solution board
        puzzle[words[index].startRow][k] = ' '; // place in solution board
    }
}

void placeFirst(Words words[], char soln[][szBoard], char puzzle[][szBoard]) // place the first word
{
    words[0].startCol = 7-(words[0].length/2); // subtract from center to find starting
    words[0].startRow = 7;  // 7 is center
    placeHoriz(words, soln, puzzle, 0); // place it horizontally
    strcpy(words[0].orientation, "Across"); // mark as across
}

void placeSecond(Words words[], char soln[][szBoard], char puzzle[][szBoard]) // place second word
{
    char common[maxLetters];
    char commonCol[maxLetters];
    int numCol = findCol(words, soln, common, commonCol, 0, 1);
    if (numCol == 0) {
        printf("ERROR: Word \"%s\" could not be placed. Moving on.\n", words[1].word); // show error message if word cannot be placed
    }
    else {
        for (int i = 0; i < words[1].length; i++) {
            for (int k = 0; k < numCol; k++) {
                if ((words[1].word[i] == common[k]) &&  ((words[0].startRow - i) >= 0)){ // check for common character and make sure its in bounds
                    words[1].startRow = words[0].startRow - i;
                    words[1].startCol = commonCol[k];
                    strcpy(words[1].orientation, "Down"); // mark as down
                    break;
                }
            }
        }
    }
    placeVert(words, soln, puzzle, 1);
}

int checkHoriz(Words words[], char soln[][szBoard], int index, int i, int x, int y) // check if the word can be placed horizontally
{
    int location = y - i;

    if ((location < 0) || ((location+words[index].length) > (szBoard - 1)) || soln[x][location-1] != '.') // check if word fits and if there is a blank space to place it
        return 0;

    for (int j = 0; j < words[index].length; j++) {
        if (soln[x][location] != '.' && (words[index].word[j] != soln[x][location])) // make sure word doesnt touch other words
            return 0;
        if ((x-1 >= 0) && (location != y) && (soln[x-1][location] != '.'))  // make sure word not on top of another
            return 0;
        if ((x+1) <= (szBoard - 1) && (location != y) && (soln[x+1][location] != '.'))
            return 0;
        location++;
    }

    words[index].startCol = location - words[index].length; // copy column
    words[index].startRow = x;  //copy row
    strcpy(words[index].orientation, "Across"); // copy direction
    return 1;
}

int checkVert(Words words[], char soln[][szBoard], int index, int i, int x, int y) // same concept as checkHoriz but checks if it can fit vertically
{
    int location = x - i;
    int letter = 0;

    if ((location < 0) || ((location+words[index].length) > (szBoard - 1)) || soln[x][location-1] != '.')
        return 0;

    for (int j = 0; j < words[index].length; j++) {
        if (soln[location][y] != '.' && (words[index].word[j] != soln[location][y]))
            return 0;
        if ((y-1 >= 0) && (location != x) && (soln[location][y-1] != '.'))
            return 0;
        if ((y+1) <= (szBoard - 1) && (location != x) && (soln[location][y+1] != '.'))
            return 0;
        location++;
    }

    words[index].startCol = y;
    words[index].startRow = location - words[index].length;
    strcpy(words[index].orientation, "Down");
    return 1;
}

void place3Plus(Words words[], char soln[][szBoard], char puzzle[][szBoard], int index) // place 3rd word and all words after
{
    int valid;

    for (int i = 0; i < words[index].length; i++) {
        for (int x = 0; x < szBoard; x++) {
            for (int y = 0; y < szBoard; y++) {
                if (words[index].word[i] == soln[x][y]) {
                    valid = checkHoriz(words, soln, index, i, x, y); // return 1 if able to place horizontally
                    if (valid == 1) {
                        placeHoriz(words, soln, puzzle, index);
                        return;
                    }
                    else if (valid == 0) {
                        valid = checkVert(words, soln, index, i, x, y); // return 1 if able to place vertically
                        if (valid == 1) {
                            placeVert(words, soln, puzzle, index);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void placeWords(Words words[], char soln[][szBoard], char puzzle[][szBoard], int numWords) // place the words
{
    // place first word
    placeFirst(words, soln, puzzle);

    // place second word
    placeSecond(words, soln, puzzle);

    // place the rest of the words
    for (int i = 2; i < numWords-1; i++)
        place3Plus(words, soln, puzzle, i);
    
}

void placeError(FILE *fp, Words words[], int numWords)
{
    // error message for not being able to place
    for (int i = 0; i < numWords; i++) {
        if ((strcmp(words[i].orientation, "Can\'t Place") == 0) && (fp == NULL))
            printf("Sorry, \"%s\" could not be placed.\n", words[i].word);
        else if ((strcmp(words[i].orientation, "Can\'t Place") == 0) && (fp != NULL))
            fprintf(fp, "Sorry, \"%s\" could not be placed.\n", words[i].word);
    }
}

void shuffle(Words words[], int numWords) // shuffle words using strfry
{
    for (int i = 0; i < numWords; i++) {
        strcpy(words[i].shuffled, words[i].word);
        strfry(words[i].shuffled);
    }
}


void dispBoard(char board[][szBoard]) // display the board
{
    // top line
    for (int i = 0; i < szBoard+2; i++) {
        printf("-");
    }
    printf(" \n");

    // board
    for (int i = 0; i < szBoard; i++) {
        printf("|");
        for (int j = 0; j < szBoard; j++) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }

    // bottom line
    for (int i = 0; i < szBoard+2; i++) {
        printf("-");
    }
    printf(" \n");
}

void clues(Words words[], int numWords) // print clues
{
    printf("CLUES:\n");
    for (int i = 0; i < numWords; i++) {
        printf("%2d,%2d | %11s | %16s\n", words[i].startRow+1, words[i].startCol+1, // +1 because indexing starts at 0 but the user should be able to start at 1
        words[i].orientation, words[i].shuffled);
    }
}

void fClues(FILE *fp, Words words[], int numWords) // print clues int file
{
    fprintf(fp, "CLUES:\n");
    for (int i = 0; i < numWords; i++) {
        fprintf(fp, "%2d,%2d | %11s | %16s\n", words[i].startRow+1, words[i].startCol+1, // +1 because indexing starts at 0 but the user should be able to start at 1
        words[i].orientation, words[i].shuffled);
    }
}

void printFile(FILE *fp, char board[][szBoard]) // print board into file
{
    // top line
    for (int i = 0; i < szBoard+2; i++) {
        fprintf(fp, "-");
    }
    fprintf(fp, " \n");

    // board
    for (int i = 0; i < szBoard; i++) {
        fprintf(fp, "|");
        for (int j = 0; j < szBoard; j++) {
            fprintf(fp, "%c", board[i][j]);
        }
        fprintf(fp, "|\n");
    }

    // bottom line
    for (int i = 0; i < szBoard+2; i++) {
        fprintf(fp, "-");
    }
    fprintf(fp, " \n");
}