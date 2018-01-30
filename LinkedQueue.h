/* LinkedQueue.h */

/* Type definitions */
typedef struct {
	char itemCode; 	/* stores a number describing the element */
	char stage; 	/* 0: part is built, 1: part not built, 2: part is shipped */
} element;

typedef struct link{
	element		e;
	struct link *next;
} link;

/* Constant declations */
/* 	E.g. #define STAGE0 0 ... sets STAGE0 = 0 

	Constant declarations can make you code much more readable, and will make life easier when required
	you need to enter in a constant value through many places in your code such as PI, or the GOLDEN NUMBER
	etc.
*/


/* Subroutine headers */
/* 	List the top line of your subroutine here. 
	WARNING: Make sure you put a semi-colon after each line, if you fail to do this it will make your life
	miserable to try and figure out where your bug is
*/
//MAKE sure there are semi colons at the end of these if you change them!!!

void	initLink	(link **newLink);
void 	setup		(link **h, link **t);
void 	clearQueue	(link **h, link **t);
void 	enqueue		(link **h, link **t, link **nL);
void 	dequeue		(link **h, link **deQueuedLink);
element firstValue	(link **h);
char 	isEmpty		(link **h);
int 	size		(link **h, link **t);

