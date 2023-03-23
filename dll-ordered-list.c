/* dll-ordered-list.c
*  Abstract: This program uses a doubly linked list to store values in 
*  an ascending order and allow the user to perform operations on the list.
*  Author: Yanni Vu
*  Email: yvu.nd.edu
*  Date: 02/13/2023
*/

// LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

// STRUCTS
typedef struct dll_node{
	int data;
	struct dll_node *prev;
	struct dll_node *next;
}dll_node;

typedef struct dllist{
    struct dll_node *head;
    struct dll_node *tail;
}dllist;

// PROTOTYPES
void insertFront(dllist* list, int val);
void insertBack(dllist* list, int val);
void printList(dll_node* curr_ptr);
void destructor(dll_node* curr_ptr);
void printMenu();
void displayAll(dll_node* head, int numVals);
void displayNth(dll_node* head, int nodeNum);
void appendHeadOrTail(dllist* list, int *numVals);
void appendNthPosition(dllist* list, int *numVals);
void deleteHeadorTail(dllist* list, int *numVals);
void deleteNthNode(dllist* list, int *numVals);
void find(dllist* list, int numVals);
void getNumberOfNodes(dll_node* head);
void displayEven(dll_node* head, int numVals);
void displayOdd(dll_node* head, int numVals);

// MAIN
int main(){

	// initialize doubly linked listed
	dllist* OrderedList = malloc(sizeof(dllist));
	OrderedList->head = NULL;
	OrderedList->tail = NULL;

	//read filename
	char fname[20];
	printf("Enter an input file: ");
	scanf("%s", fname);

	// open file
	FILE* fp = fopen(fname, "r");
	if (fp == NULL) {
		printf("File does not exist!");
		return 1;
	}

	// read in first value
	int numVals;
	fscanf(fp,"%d", &numVals);

	// read in nodes
	int val;
	for (int i = 0; i < numVals; i++) {
		fscanf(fp,"%d", &val);
		insertBack(OrderedList, val);
	}

	printMenu(); // print the menu
	char cont;
	int choice, nodeNum;
	while (1) {
		// get user's option
		printf("\nEnter your option: ");
		scanf("%d", &choice);

		// conditionals to run the appropriate function
		if (choice == 1) {
			displayAll(OrderedList->head, numVals);
		}
		else if (choice == 2) {
			printf("Enter n to display nth node: ");
			scanf("%d", &nodeNum);
			printf("\nDisplaying node %d:\n\t", nodeNum);
			displayNth(OrderedList->head, nodeNum);
			printf("\n\n");
		}
		else if (choice == 3) {
			appendHeadOrTail(OrderedList, &numVals);
		}
		else if (choice == 4) {
			appendNthPosition(OrderedList, &numVals);
		}
		else if (choice == 5) {
			deleteHeadorTail(OrderedList, &numVals);
		}
		else if (choice == 6) {
			deleteNthNode(OrderedList, &numVals);
		}
		else if (choice == 7) {
			find(OrderedList, numVals);
		}
		else if (choice == 8) {
			getNumberOfNodes(OrderedList->head);
		}
		else if (choice == 9) {
			displayEven(OrderedList->head, numVals);
		}
		else if (choice == 10) {
			displayOdd(OrderedList->head, numVals);
		}
		else {
			printf("That is not an option!\n");
		}

		// while loop to keep prompting unless the user enters a valid input
		while (1) {
			printf("Continue? (Y/N): ");
			scanf(" %c", &cont);
			if ((cont == 'Y') || (cont == 'y') || (cont == 'N') || (cont == 'n')) break;
		}

		// conditionals to break out of loop
		if ((cont == 'Y') || (cont == 'y')) continue;
		else if ((cont == 'N') || (cont == 'n')) break;
	}


	// free memory
	fclose(fp);
	destructor(OrderedList->head);
	free(OrderedList);
	return 0;
}



// FUNCTIONS
void insertFront(dllist* list, int val)
{
	// initialize node to store value
	dll_node *insertNode = malloc(sizeof(dll_node));

	// add data to node
	insertNode->data = val;
	insertNode->prev = NULL;
	insertNode->next = NULL;

	// check if list is empty
	if (list->head == NULL) {
		list->head = insertNode;
		list->tail = insertNode;
		return;
	}

	// use reference pointer to insert node at the head
	dll_node *refptr = list->head;
	list->head = insertNode;
	insertNode->next = refptr;
	refptr->prev = insertNode;
}

void insertBack(dllist* list, int val)
{
	// initialize node to store value
	dll_node* insertNode = malloc(sizeof(dll_node));

	// add data to node
	insertNode->data = val;
	insertNode->prev = NULL;
	insertNode->next = NULL;

	// check if list is empty
	if (list->tail == NULL) {
		list->head = insertNode;
		list->tail = insertNode;
		return;
	}

	// use reference pointer to insert node at the tail
	dll_node *refptr = list->tail;
	list->tail = insertNode;
	insertNode->prev = refptr;
	refptr->next = insertNode;
}

void destructor(dll_node* curr_ptr)
{
	// destructor to free nodes
	dll_node* tmp;

	while(curr_ptr != NULL) {
    	tmp = curr_ptr;
    	curr_ptr = curr_ptr->next;
    	free (tmp);
	}
}

void printMenu()
{
	// menu options
	printf("1. Display all nodes\n");
	printf("2. Display nth node\n");
	printf("3. Append a new node - Head or Tail? (H/T)\n");
	printf("4. Append a new node in the Nth position\n");
	printf("5. Delete a node - Head or Tail? (H/T)\n");
	printf("6. Delete Nth node\n");
	printf("7. Find a node\n");
	printf("8. Get number of nodes\n");
 	printf("9. Display even numbered nodes\n");
	printf("10. Display odd numbered nodes\n");
}

void displayAll(dll_node* head, int numVals)
{
	printf("\nDisplaying all nodes:\n\t");

	// node pointing to the head
	dll_node* curr_ptr = head;

	int count = 0;

	// traverse through each node and print
	while (curr_ptr != NULL) {
		printf("%d ", curr_ptr->data);
		if (count != (numVals-1)) printf("-> ");
		curr_ptr = curr_ptr->next;
		count++;
	}
	printf("\n\n");
}

void displayNth(dll_node* head, int nodeNum)
{
	// traverse through nodes until the desired node is reached
	dll_node* curr_ptr = head;
	for (int i = 0; i < nodeNum-1; i++) {
		curr_ptr = curr_ptr->next;
	}

	// print the data from the node
	fprintf( stdout, "%d", curr_ptr->data );
}

void appendHeadOrTail(dllist* list, int *numVals)
{
	// ask user to append to head or tail
	printf("Append to head or tail? (H/T): ");
	char HorT;
	int append;
	scanf(" %c", &HorT);

	// conditional to append at head or tail
	if (HorT == 'H') {
		dll_node* curr_ptr = list->head; // node pointing to head
		printf("Node at the head of the list is %d.\n", curr_ptr->data);
			// loop asking for value to append until the user enters an appropriate value
			while (1) {
				printf("Enter an integer less than %d to append to list: ", curr_ptr->data);
				scanf("%d", &append);
				if (append >= curr_ptr->data) continue;
				else break;
			}
		insertFront(list, append); // call function to append to head
        *numVals = *numVals+1; // update numVals
		displayAll(list->head, *numVals); // display all nodes
	}
	else if (HorT == 'T') {
		dll_node* curr_ptr = list->tail; // node pointing to tail
		printf("Node at the tail of the list is %d.\n", curr_ptr->data);
		// loop asking for value to append until the user enters an appropriate value
		while (1) {
			printf("Enter an integer greater than %d to append to list: ", curr_ptr->data);
			scanf("%d", &append);
			if (append <= curr_ptr->data) continue;
			else break;
		}
		insertBack(list, append); // call function to append to tail
		*numVals = *numVals+1; // update numVals
		displayAll(list->head, *numVals); // display all nodes
	}
}

void appendNthPosition(dllist* list, int *numVals)
{
	// ask user for position to append
	int npos;
	printf("Append node at which position?: ");
	scanf("%d", &npos);

	// traverse to that position
	dll_node* curr_ptr = list->head;
	for (int i = 0; i < npos-1; i++) {
		curr_ptr = curr_ptr->next;
	}
	
	// display n-1 node
	printf("Node at position %d: ", npos-1);
	displayNth(list->head, npos-1);
	printf("\n");

	// display n node
	printf("Node at position %d: ", npos);
	displayNth(list->head, npos);
	printf("\n");

	// loop until user enters an appropriate value to place between nodes
	int append;
	while (1) {
		printf("Enter an integer between ");
		displayNth(list->head, npos-1);
		printf(" and ");
		displayNth(list->head, npos);
		printf(": ");
		scanf("%d", &append);
		if ((append > curr_ptr->prev->data) && (append < curr_ptr->data)) break;
		else continue;
	}

	dll_node *insertNode = malloc(sizeof(dll_node)); // allocate memory for insert node

	// add data to node
	insertNode->data = append;
	insertNode->prev = NULL;
	insertNode->next = NULL;

	//check if list is null
    if (list->head == NULL) {
        list->head = insertNode;
        list->tail = insertNode;
        return;
    }

	// insert the node between two values
	insertNode->next = curr_ptr;
    insertNode->prev = curr_ptr->prev;
    if (curr_ptr->prev != NULL) {
        curr_ptr->prev->next = insertNode;
    } else {
        list->head = insertNode;
    }
    curr_ptr->prev = insertNode;

	*numVals = *numVals+1; // update numVals
	
	displayAll(list->head, *numVals); // display all nodes
}

void deleteHeadorTail(dllist* list, int *numVals)
{
	// ask user what to delete
	char HorT;
	while (1) {
		printf("Delete from head or tail? (H/T): ");
		scanf(" %c", &HorT);
		if((HorT == 'H') || (HorT == 'h') || (HorT == 'T') || (HorT == 't')) break;
	}

	// use temp node to hold value and then free appropriate node
	if (HorT == 'H') {
		dll_node* tmp = list->head->next;
		free(list->head);
    	list->head = tmp;
	}
	else if (HorT == 'T') {
		dll_node* tmp = list->tail->prev;
		tmp->next = NULL;
		free(list->tail);
    	list->tail = tmp;
	}

	*numVals = *numVals-1; // update numVals

	displayAll(list->head, *numVals); // display all nodes
}

void deleteNthNode(dllist* list, int *numVals)
{
	// ask user what to delete
	int npos;
	while (1) {
		printf("Delete node at which position?: ");
		scanf("%d", &npos);
		if ((npos > 0) && (npos < *numVals)) break;
	}

	if (npos == 1) {
		// deleting the head node
		dll_node* old_head = list->head;
		list->head = old_head->next;
		if (list->head != NULL) {
			list->head->prev = NULL;
		} else {
			list->tail = NULL;
		}
		free(old_head);
	} else if (npos == *numVals) {
		// deleting the tail node
		dll_node* old_tail = list->tail;
		list->tail = old_tail->prev;
		if (list->tail != NULL) {
			list->tail->next = NULL;
		} else {
			list->head = NULL;
		}
		free(old_tail);
	} else {
		// deleting a node in the middle of the list
		dll_node* curr = list->head;
		for (int i = 1; i < npos; i++) {
			curr = curr->next;
		}
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		free(curr);
	}

	*numVals = *numVals - 1; // update numVals

	displayAll(list->head, *numVals); // display head node
}

void find(dllist* list, int numVals)
{
	// ask user what to find
	int findInt;
	printf("Enter an integer to find in the list: ");
	scanf("%d", &findInt);

	// traverse through list until node is found
	int npos = 1;
	dll_node* curr_ptr = list->head;
	for (int i = 0; i < numVals; i++) {
		if (curr_ptr->data == findInt) break;
		curr_ptr = curr_ptr->next;
		npos++;
	}

	// print values
	if (npos == 1) {
		printf("\n%d found at position %d in the list.\n", curr_ptr->data, npos);
		printf("Next node of %d is %d.", curr_ptr->data, curr_ptr->next->data);
	}
	else if (npos == numVals) {
		printf("\n%d found at position %d in the list.\n", curr_ptr->data, npos);
		printf("Previous node of %d is %d.", curr_ptr->data, curr_ptr->prev->data);
	}
	else {
		printf("\n%d found at position %d in the list.\n", curr_ptr->data, npos);
		printf("Previous node of %d is %d.\n", curr_ptr->data, curr_ptr->prev->data);
		printf("Next node of %d is %d.", curr_ptr->data, curr_ptr->next->data);
	}
	
	displayAll(list->head, numVals); // display all nodes
}

void getNumberOfNodes(dll_node* head)
{
	dll_node* curr_ptr = head;
	int count = 0;

	// traverse through whole list and count until the end
	while (curr_ptr != NULL) {
		count++;
		curr_ptr = curr_ptr->next;
	}	
	printf("Number of nodes in the list: %d", count); // display count
	displayAll(head, count);
}

void displayEven(dll_node* head, int numVals)
{
	// call display function to display every other node starting at 2
	printf("Displaying even numbered nodes: \n\t");
	for (int i = 2; i <= numVals; i=i+2) {
		displayNth(head, i);
		printf(" ");
	}
	displayAll(head, numVals);
}

void displayOdd(dll_node* head, int numVals)
{
	// call display function to display every other node starting at 1
	printf("Displaying odd numbered nodes: \n\t");
	for (int i = 1; i <= numVals; i=i+2) {
		displayNth(head, i);
		printf(" ");
	}
	displayAll(head, numVals);
}

