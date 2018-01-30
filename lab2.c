/*
#################################################
MILESTONE: 2
PROGRAM: 2
PROJECT: knightrider, self made timer
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: This program lights up the on board LEDs on the AT90USBKEY2, and creates
	  the knightrider LED display on the LEDs connected to Port D.
DATA
REVISED
#################################################
*/

//####### Include Files #######
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//########### Prototypes ############
void knightrider(int delaytime);
void delayms(int count);

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

  // Set timer one to run at CPU Clock, Use as pure timer
  TCCR1B |=_BV(CS10);

  // Set Data Direction Registers
  DDRC = 0xFF;
  DDRD = 0xFF;

  // Initalize IO registers to 0
  PORTC = 0x00;
  PORTD = 0x00;
  while (1) {
    knightrider(70);
  }
}

//########## Program Functions ##########
void knightrider(int delaytime) {// Creates knightrider pattern on LEDs
	PORTC = 0x03;
	delayms(delaytime);
	PORTC = 0x07;
	delayms(delaytime);
	PORTC = 0x0F;
	delayms(delaytime);
	for (size_t i = 0; i < 3; i++) { // Left shift LEDs
		PORTC = PORTC << 1;
		delayms(delaytime);
	}
	PORTC = 0xE0;
	delayms(delaytime);
	PORTC = 0xC0;
	delayms(delaytime);
	PORTC = 0xE0;
	delayms(delaytime);
	PORTC = 0xF0;
	delayms(delaytime);
	for (size_t i = 0; i < 4; i++) { // Right shift LEDs
		PORTC = PORTC >> 1;
		delayms(delaytime);
	}
	PORTC = 0x07;
	delayms(delaytime);
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
