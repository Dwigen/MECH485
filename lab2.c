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

//########### Prototypes ############
void knightrider();
void delayms();

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {
  // Set Data Direction Registers
  DDRC = 0xFF;
  DDRD = 0xFF;

  // Initalize IO registers to 0
  PORTC = 0x00;
  PORTD = 0x00;



}
//########## Program Functions ##########
void knightrider() {// Creates knightrider pattern on LEDs
	PORTC = 0x03;
	delayms(delayTime, dutyCycle);
	PORTC = 0x07;
	delayms(delayTime, dutyCycle);
	PORTC = 0x0F;
	delayms(delayTime, dutyCycle);
	for (size_t i = 0; i < 3; i++) { // Left shift LEDs
		PORTC = PORTC << 1;
		delayms(delayTime, dutyCycle);
	}
	PORTC = 0xE0;
	delayms(delayTime, dutyCycle);
	PORTC = 0xC0;
	delayms(delayTime, dutyCycle);
	PORTC = 0xE0;
	delayms(delayTime, dutyCycle;
	PORTC = 0xF0;
	delayms(delayTime, dutyCycle);
	for (size_t i = 0; i < 4; i++) { // Right shift LEDs
		PORTC = PORTC >> 1;
		delayms(delayTime, dutyCycle);
	}
	PORTC = 0x07;
	delayms(delayTime, dutyCycle);
}

void delayms(int t) {
  /* code */
}
