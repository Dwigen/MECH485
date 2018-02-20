/*
#################################################
MILESTONE: 4
PROGRAM: 1
PROJECT:
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: Stepper motor forward/reverse control, and DC motor PWM control
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
void beltMotor(int dutyCycle, int beltDir);
void PWM();
void delayms(int count);

//########## GLOBAL VARIABLES #######
int stepdelay = 20;
int stepPosition = 1;

//########## Variable Definitions ######
#define stepOne 0b00001100
#define stepTwo	0b01100000
#define stepThree 0b00010100
#define stepFour 0b10100000

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

  // Set timer one to run at CPU Clock, Use as pure timer
  TCCR1B |=_BV(CS10);

	sei();

  // Set Data Direction Registers
  DDRA = 0xFF;
  DDRC = 0xFF;
  PORTA = 0x00;
	PORTC = 0x00;
	rotatePlatform(0,101);
	delayms(100);
	rotatePlatform(0,17);
	delayms(100);
	rotatePlatform(0,33);
	delayms(100);
	rotatePlatform(0,100);
	delayms(100);
	rotatePlatform(1,17);
	delayms(100);
	rotatePlatform(1,33);
	delayms(100);
	rotatePlatform(1,100);
	delayms(100);
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
			switch (stepPosition) {
				case 1:
					PORTC = stepTwo;
					stepPosition++;
					break;
				case 2:
					PORTC = stepThree;
					stepPosition++;
					break;
				case 3:
					PORTC = stepFour;
					stepPosition++;
					break;
				case 4:
					PORTC = stepOne;
					stepPosition = 1;
					break;
			}
		}
	}
	else {
		for (size_t i = 0; i < (distance); i++) {
			switch (stepPosition) {
				case 1:
					PORTC = stepFour;
					stepPosition = 4;
					break;
				case 2:
					PORTC = stepOne;
					stepPosition--;
					break;
				case 3:
					PORTC = stepTwo;
					stepPosition--;
					break;
				case 4:
					PORTC = stepThree;
					stepPosition--;
					break;
			}
		}
	}
}

void beltMotor(int dutyCycle, int beltDir) {
	PWM();
	OCR0A = dutyCycle;
	if (beltDir == 0) {
		PORTB = 0b00101100;
	}
	else{
		PORTB = 0b01001100;
	}
	return;
}

void PWM() {
	DDRB |= _BV(PB7);
	TCCR0A |= _BV(WGM01);
	TCCR0A |= _BV(WGM00);
	IMSK0 = TIMSK0 |0b00000010;
	TCCR0A |= _BV(COM0A1);
	TCCR0B |= _BV(CS02);
	TCCR0B |= _BV(CS00);
	OCR0A = 127;
}
