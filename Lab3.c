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

  // Set timer one to run at CPU Clock, Use as pure timer
  TCCR1B |=_BV(CS10);

  // Set Data Direction Registers
  DDRA = 0x00;
  DDRC = 0xFF;

  while (1) {
    PORTC = 0x00;
    readIn();
    readOut();
  }
}

//Add input to List
void readIn() {
  //Create function to poll (or use interrupts) to look for PINA bit 3
  //to be pulled low. Software debounce and count to 4,
  for (size_t i = 0; i < 4; i++) {

    //Take input and remove any input other than the first 2 bits
    //send that to addItem
    while (PINA & 0xFB != 0x04){
    }
    delayms(20); // software debouncing the input switch.
    while (PINA & 0x04 != 0x04){
    }
    delayms(20);
    initLink(&newLink);
    newLink->e.itemCode = i;
    newLink->e.stage = (PINA & 0x03);
    enqueue(&head, &tail, &newLink);
  }
}

void readOut(){
    int *delete = 0;
    dequeue(&head, &tail, &delete);
    free(delete);
    for (size_t i = 0; i < 3; i++) {
      if (i == 0) {
        PORTC = head->e.stage;
      }else{
        PORTC = PORTC | (head->e.stage << 2^i);
      }
      delayms(2000);
      dequeue(&head, &tail, &delete);
      free(delete);
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
