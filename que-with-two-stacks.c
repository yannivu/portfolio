/* que-with-two-stacks.c
*  Abstract: This program implements a queue using two stacks.
*  Author: Yanni Vu
*  Email: yvu.nd.edu
*  Date: 02/18/2023
*/

// LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

// STACK
typedef struct sll_node{
    int data;
    struct sll_node *next;
} Sll_node;

typedef struct queue{
    struct sll_node *front;
} Queue;

// PROTOTYPES
void push(Queue* queue, int the_value);
void destructor(Sll_node* curr_ptr);
void pop(Queue* queue);
void top(Queue* queue);
int isEmpty(Queue* queue);
void enque(Queue* queue, Queue* tmp, int val);
void deque(Queue* queue);
void display(Sll_node* front);


// MAIN
int main()
{
    // Initialize queue
	Queue *queue = malloc (sizeof(Queue));
	Queue *tmp = malloc(sizeof(Queue));
	queue->front = NULL;
	tmp->front = NULL;

	// Print Menu
	printf("This is a list of operations on the queue.\n\n");
	printf("\t1. Enque\n");
	printf("\t2. Deque\n");
	printf("\t3. Display\n");

	// While loop for options
	int option, val;
	char ifCont;
	while (1) {
		// Ask for option
		printf("\nEnter your option: ");
		scanf("%d", &option);

		// Call appropriate function depending on option
		if (option == 1) {
			printf("Enter value to enque: ");
			scanf("%d", &val);
			enque(queue, tmp, val);
			display(queue->front);
		}
		else if (option == 2) {
			deque(queue);
			display(queue->front);
		}
		else if (option == 3) {
			display(queue->front);
		}

		// Ask to continue and break if N
		printf("Continue? (Y/N) ");
		scanf(" %c", &ifCont);
		if (ifCont == 'Y' || ifCont == 'y') continue;
		else if (ifCont == 'N' || ifCont == 'n') break;
	}

    // Destruct stack and free
    destructor(queue->front);
	destructor(tmp->front);
    free(queue);
	free(tmp);
    return 0;
}

// FUNCTIONS
int isEmpty(Queue* queue) {
        if (queue->front == NULL) {
                return 1;
        }
		else return 0;
}

void push( Queue* queue, int the_value ){

  // Create insert node
  Sll_node *insert_node = malloc (sizeof(Sll_node));
  insert_node->data = the_value;
  insert_node->next = NULL;
  
	// Check if the list is empty
  if (isEmpty(queue) == 1){
    queue->front = insert_node;
    //return
    return;
  }
    
	//Otherwise add insert_node to the top of the stack
  insert_node->next = queue->front;
  queue->front = insert_node;
	
}

// Pop the top node
void pop( Queue* queue ){

    // Check if the list is not empty
    if(isEmpty(queue) == 1) {
        return;
    }

    // Otherwise, move the top node to next and free old top node
    Sll_node *tmp = queue->front;
    queue->front = queue->front->next;
    free(tmp); 
	
}

void top(Queue* queue) {
        Sll_node* ptr = queue->front;
        printf("%c", ptr->data);
}	

void enque(Queue* queue, Queue* tmp, int val)
{
	while(!isEmpty(queue)) {
		push(tmp, queue->front->data); // push all values of queue into tmp backwards
		pop(queue);	// empty the queue
	}

	push(queue, val); // val is the only node pushed to the queue
	
	while (!isEmpty(tmp)) {
		push(queue, tmp->front->data); // push all the old values into queue
		pop(tmp); // empty out tmp
	}
}

void deque(Queue* queue)
{
	printf("Dequed element: %d", queue->front->data);
	pop(queue); // pop the top node
}


void display(Sll_node* front)
{
	Sll_node* countPtr = front;
	int numVals = 0;

	// traverse through whole list and count until the end
	while (countPtr != NULL) {
		numVals++;
		countPtr = countPtr->next;
	}	

	printf("\nDisplaying queue front to back:\n\t");

	Sll_node* curr_ptr = front;
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

// Free all the elements 
void destructor( Sll_node* curr_ptr ){

  // Create a tmp pointer
  Sll_node *tmp;

  // Check if curr_ptr is NULL
  while(curr_ptr != NULL) {
    
    // Set tmp to point to curr_ptr
    tmp = curr_ptr;

    //Advance curr_ptr
    curr_ptr = curr_ptr->next;

    //free tmp
    free(tmp);
  }

}