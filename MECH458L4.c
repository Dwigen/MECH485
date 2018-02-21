/*
#################################################
MILESTONE: 4
PROGRAM: 1
PROJECT:
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: Stepper motor forward/reverse control, and PWM Signal Generation
DATA
REVISED
#################################################
*/

//####### Include Files #######
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//########### Prototypes ############
void rotatePlatform(int direction, int distance);
void PWM();
void delayms(int count);

//########## GLOBAL VARIABLES #######
int stepdelay = 15; //10 is too low
int stepPosition = 1;

//########## Variable Definitions ######
#define stepOne 0b00110000
#define stepTwo	0b00000110
#define stepThree 0b00101000
#define stepFour 0b00000101
#define forward	0
#define backward 1
#define clkwise 0
#define cclkwise 1
#define black	0
#define steel 50
#define white 100
#define aluminum 150


//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

	// Set timer one to run at CPU Clock, Use as pure timer
	TCCR1B |=_BV(CS10);

	sei(); //enable global interrupts

	// Set Data Direction Registers
	DDRA = 0xFF;
	DDRC = 0xFF;
	PORTA = 0x00;
	PORTC = 0x00;
	PWM();
	rotatePlatform(clkwise,101);
	delayms(500);
	rotatePlatform(clkwise,17);
	delayms(500);
	rotatePlatform(clkwise,33);
	delayms(500);
	rotatePlatform(clkwise,100);
	delayms(500);
	rotatePlatform(cclkwise,17);
	delayms(500);
	rotatePlatform(cclkwise,33);
	delayms(500);
	rotatePlatform(cclkwise,100);
	delayms(500);
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

void rotatePlatform(int direction, int distance){
	if (direction == 0) {
		for (size_t i = 0; i < (distance); i++) {
			delayms(stepdelay);
			switch (stepPosition) {
				case 1:
					PORTA = stepTwo;
					stepPosition++;
					break;
				case 2:
					PORTA = stepThree;
					stepPosition++;
					break;
				case 3:
					PORTA = stepFour;
					stepPosition++;
					break;
				case 4:
					PORTA = stepOne;
					stepPosition = 1;
					break;
			}
		}
	}
	else {
		for (size_t i = 0; i < (distance); i++) {
			delayms(stepdelay);
			switch (stepPosition) {
				case 1:
					PORTA = stepFour;
					stepPosition = 4;
					break;
				case 2:
					PORTA = stepOne;
					stepPosition--;
					break;
				case 3:
					PORTA = stepTwo;
					stepPosition--;
					break;
				case 4:
					PORTA = stepThree;
					stepPosition--;
					break;
			}
		}
	}
}

void PWM() {
	TCCR0A |= _BV(WGM01);
	TCCR0A |= _BV(WGM00);
	TIMSK0 = TIMSK0 |0b00000010;
	TCCR0A |= _BV(COM0A1);
	TCCR0B |= _BV(CS01);
	OCR0A = 127;
	DDRB |= _BV(PB7);
}
