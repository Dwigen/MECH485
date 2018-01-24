/*
#################################################
MILESTONE: 1
PROGRAM: 1
PROJECT: MyFirstProject
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
#include <util/delay_basic.h>

//########### Prototypes ############
void delaynms(int n);
void delaynus(int n);
void knightrider();
void lightup();

int delayTime = 10; //Delay time between led changes in ms

//################ MAIN ROUTINE ##############
int main(int argc, char const *argv[]) {
	//Set port D and C pins to output
	DDRD = 0xFF;
	DDRC = 0xFF;

	PORTD = 0xA0; //Lab 1 part one. Turn on LEDs
	PORTC = 0x00;
	while(1){		//Runs knightrider function infinitely
		knightrider();
	}
	return 0;
}

//########## Program Functions ##########
void delaynus(int n) { //function to delay one microsecond
	int k;
	for(k=0; k<n; k++){
		_delay_loop_1(1);
	}
}

void delaynms(int n) { //function to delay one millisecond
	int k;
	for (k = 0; k < n; k++){
		delaynus(1000);
	}
}

void knightrider() {// Creates knightrider pattern on LEDs
	PORTC = 0x03;
	delaynms(delayTime);
	PORTC = 0x07;
	delaynms(delayTime);
	PORTC = 0x0F;
	delaynms(delayTime);
	for (size_t i = 0; i < 3; i++) { // Left shift LEDs
		PORTC = PORTC << 1;
		delaynms(delayTime);
	}
	PORTC = 0xE0;
	delaynms(delayTime);
	PORTC = 0xC0;
	delaynms(delayTime);
	PORTC = 0xE0;
	delaynms(delayTime);
	PORTC = 0xF0;
	delaynms(delayTime);
	for (size_t i = 0; i < 4; i++) { // Right shift LEDs
		PORTC = PORTC >> 1;
		delaynms(delayTime);
	}
	PORTC = 0x07;
	delaynms(delayTime);
}
