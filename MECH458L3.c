/*
#################################################
MILESTONE: 3
PROGRAM: 1
PROJECT:
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: This program uses a FIFO queue to store input values from port A and
      then reads out the list to display them on port D.
DATA
REVISED
#################################################
*/

//####### Include Files #######
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LinkedQueue.h" //or include "LinkedQueue.c" unsure

//########### Prototypes ############
void readIn(/* arguments */);
void readOut(/* arguments */);
void delayms(int count);

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

	link *head;			/* The ptr to the head of the queue */
	link *tail;			/* The ptr to the tail of the queue */
	link *newLink;		/* A ptr to a link aggregate data type (struct) */
	link *rtnLink;		/* same as the above */
	element eTest;		/* A variable to hold the aggregate data type known as element */

	DDRB = 0xFF; 		/* Used for debugging purposes only */
				

	rtnLink = NULL;
	newLink = NULL;

	setup(&head, &tail);


  // Set timer one to run at CPU Clock, Use as pure timer
  TCCR1B |=_BV(CS10);

  // Set Data Direction Registers
  DDRA = 0x00;
  DDRC = 0xFF;
  PORTC = 0x00;

  while (1) {
    //Create function to poll (or use interrupts) to look for PINA bit 3
  	//to be pulled low. Software debounce and count to 4,

	link *head;			/* The ptr to the head of the queue */
	link *tail;			/* The ptr to the tail of the queue */
	link *newLink;		/* A ptr to a link aggregate data type (struct) */
	link *rtnLink;		/* same as the above */
	element eTest;		/* A variable to hold the aggregate data type known as element */

	DDRB = 0xFF; 		/* Used for debugging purposes only */
				

	rtnLink = NULL;
	newLink = NULL;

	setup(&head, &tail);

  	for (size_t i = 0; i < 4; i++) {

    	//Take input and remove any input other than the first 2 bits
    	//send that to addItem
    	while ((PINA & 0x04) == 0x04){
    	}

    	delayms(20); // software debouncing the input switch.

    	while ((PINA & 0x04 )!= 0x04){
    	}

    	delayms(20);
    	initLink(&newLink);
    	newLink->e.itemCode = i;
    	newLink->e.stage = (PINA & 0x03);
    	enqueue(&head, &tail, &newLink);
  	}

   	int *delete = 0;
    dequeue(&head, &delete);
    free(delete);

    for (size_t i = 0; i < 3; i++) {
     	if (i == 0) {
       		PORTC = head->e.stage;
      	}

	  	if(i == 1){
        	PORTC = PORTC | (head->e.stage << 2);
      }

	  	if(i == 2){
        	PORTC = PORTC | (head->e.stage << 4);
      }

      delayms(2000);
      dequeue(&head, &delete);
      free(delete);
    }
  }

}


//Counter function counts milliseconds
void delayms(int count){
  int i;
  i = 0;
  TCCR1B|=_BV(WGM12);
  OCR1A = 0x03E8;
  TCNT1 = 0x0000;
  TIMSK1 = TIMSK1 |0b00000010;
  TIFR1 |=_BV(OCF1A);
  while (i<count) {
    if((TIFR1 & 0x02) == 0x02){
      TIFR1 |=_BV(OCF1A);
      i++;
    }
  }
  return;
}




/**************************************************************************************
* DESC: initializes the linked queue to 'NULL' status
* INPUT: the head and tail pointers by reference
*/

void setup(link **h,link **t){
	*h = NULL;		/* Point the head to NOTHING (NULL) */
	*t = NULL;		/* Point the tail to NOTHING (NULL) */
	return;
}/*setup*/




/**************************************************************************************
* DESC: This initializes a link and returns the pointer to the new link or NULL if error 
* INPUT: the head and tail pointers by reference
*/
void initLink(link **newLink){
	//link *l;
	*newLink = malloc(sizeof(link));
	(*newLink)->next = NULL;
	return;
}/*initLink*/




/****************************************************************************************
*  DESC: Accepts as input a new link by reference, and assigns the head and tail		
*  of the queue accordingly				
*  INPUT: the head and tail pointers, and a pointer to the new link that was created 
*/
/* will put an item at the tail of the queue */
void enqueue(link **h, link **t, link **nL){

	if (*t != NULL){
		/* Not an empty queue */
		(*t)->next = *nL;
		*t = *nL; //(*t)->next;
	}/*if*/
	else{
		/* It's an empty Queue */
		//(*h)->next = *nL;
		//should be this
		*h = *nL;
		*t = *nL;
	}/* else */
	return;
}/*enqueue*/




/**************************************************************************************
* DESC : Removes the link from the head of the list and assigns it to deQueuedLink
* INPUT: The head and tail pointers, and a ptr 'deQueuedLink' 
* 		 which the removed link will be assigned to
*/
/* This will remove the link and element within the link from the head of the queue */
void dequeue(link **h, link **deQueuedLink){
	/* ENTER YOUR CODE HERE */
	*deQueuedLink = *h;	// Will set to NULL if Head points to NULL
	/* Ensure it is not an empty queue */
	if (*h != NULL){
		*h = (*h)->next;
	}/*if*/
	
	return;
}/*dequeue*/




/**************************************************************************************
* DESC: Peeks at the first element in the list
* INPUT: The head pointer
* RETURNS: The element contained within the queue
*/
/* This simply allows you to peek at the head element of the queue and returns a NULL pointer if empty */
element firstValue(link **h){
	return((*h)->e);
}/*firstValue*/





/**************************************************************************************
* DESC: deallocates (frees) all the memory consumed by the Queue
* INPUT: the pointers to the head and the tail
*/
/* This clears the queue */
void clearQueue(link **h, link **t){

	link *temp;

	while (*h != NULL){
		temp = *h;
		*h=(*h)->next;
		free(temp);
	}/*while*/
	
	/* Last but not least set the tail to NULL */
	*t = NULL;		

	return;
}/*clearQueue*/





/**************************************************************************************
* DESC: Checks to see whether the queue is empty or not
* INPUT: The head pointer
* RETURNS: 1:if the queue is empty, and 0:if the queue is NOT empty
*/
/* Check to see if the queue is empty */
char isEmpty(link **h){
	/* ENTER YOUR CODE HERE */
	return(*h == NULL);
}/*isEmpty*/





/**************************************************************************************
* DESC: Obtains the number of links in the queue
* INPUT: The head and tail pointer
* RETURNS: An integer with the number of links in the queue
*/
/* returns the size of the queue*/
int size(link **h, link **t){

	link 	*temp;			/* will store the link while traversing the queue */
	int 	numElements;

	numElements = 0;

	temp = *h;			/* point to the first item in the list */

	while(temp != NULL){
		numElements++;
		temp = temp->next;
	}/*while*/
	
	return(numElements);
}/*size*/

