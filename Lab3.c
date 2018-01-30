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

//########### Prototypes ############
void addItem(int input);
void delayms(int count);

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

  // Set timer one to run at CPU Clock, Use as pure timer
  TCCR1B |=_BV(CS10);

  // Set Data Direction Registers
  DDRA = 0x00;
  DDRD = 0xFF;

  // Initalize IO registers to 0
  PORTD = 0x00;

  //Create function to poll (or use interrupts) to look for PINA bit 3
  //to be pulled low. Software debounce and count to 4,
  for (size_t i = 0; i < 4; i++) {
    //Take input and remove any input other than the first 2 bits
    //send that to addItem
    while (PINA & 0x04 == 0x04){
      
    }
    char tempin = PINA;
    char inA = tempin & 0x03;
    addItem(inA, i);
  }
}

//Add input to List
void addItem(int input, int count) {
  char outLights;
  switch (input){
    case 0x00:
      outLights = 0x00;
      break;
    case 0x01:
      outLights = 0x01;
      break;
    case 0x02:
      outLights = 0x02;
      break;
    case 0x03:
      outlights = 0x03;
      break;
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
