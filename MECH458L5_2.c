/*
#################################################
MILESTONE: 5
PROGRAM: 1
PROJECT:
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: Controls motor speed and button interrupt changes direction
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
void beltMotor(int beltDir);
void PWM();
void delayms(int count);

//########## GLOBAL VARIABLES #######
int stepdelay = 15; //10 is too low
int stepPosition = 1;
int trayPosition = 0;
int direction;
int directionChange = 0;
volatile unsigned int ADC_result;
volatile unsigned int direction_flag;
volatile unsigned int direction_change_flag;

//########## Variable Definitions ######
#define stepOne 0b00110000
#define stepTwo	0b00000110
#define stepThree 0b00101000
#define stepFour 0b00000101
#define forward	1
#define backward 0
#define brake 2

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

	// Set timer one to run at CPU Clock, Use as pure timer
	TCCR1B |=_BV(CS10);

	PWM();

	cli(); //ensure interrupts are disabled

	EIMSK |= (_BV(INT5));
	EICRB |= (_BV(ISC51) | _BV(ISC50));
	//ADC Config
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADIE);
	ADMUX |= _BV(ADLAR) | _BV(REFS0) | _BV(MUX0);
	// Set Data Direction Registers
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	DDRE = 0x00;
	PORTA = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;

	sei(); //enable global interrupts

	ADCSRA |= _BV(ADSC);
	beltMotor(1);
	direction_flag = 1;

	while (1) {
		if(direction_change_flag==1){
			direction_change_flag = 0;
			beltMotor(2);
			delayms(50);
			if(direction_flag == 1){
				beltMotor(0);
				direction_flag = 0;
			}
			else{
				beltMotor(1);
				direction_flag = 1;
			}
		}
  }
}//end main

ISR(ADC_vect){
  ADC_result = ADCH;
  OCR0A = ADC_result;
  PORTC = ADC_result;
  ADCSRA |= _BV(ADSC);
}

ISR(INT5_vect){
	direction_change_flag = 1;
}

//Counter function counts milliseconds
void delayms(int count){
	int i;
	i = 0;
	TCCR1B|=_BV(WGM12);
	OCR1A = 0x03E8;
	TCNT1 = 0x0000;
	TIFR1 |=_BV(OCF1A);
	while (i<count) {
		if((TIFR1 & 0x02) == 0x02){
			TIFR1 |=_BV(OCF1A);
			i++;
		}
	}
	return;
}

void trayTimer(){
	TCCR3B|=_BV(WGM12);
	OCR3A = 0x3A98; //run for 15ms Max is 65ms
	TCNT3 = 0x0000;
	TIFR3 |=_BV(OCF3A);
	return;
}

void beltMotor(int beltDir) {
	if (beltDir == 0) {
		PORTA = 0b00001000;
	}
	else if (beltDir == 1){
		PORTA = 0b00000100;
	}
	else if (beltDir == 2){
		PORTA = 0b00000000;
	}
	return;
}

void PWM() {
	TCCR0B |= _BV(CS01);
	TCCR0A |= _BV(WGM01);
	TCCR0A |= _BV(WGM00);
	TCCR0A |= _BV(COM0A1);
	OCR0A = 127;
	DDRB |= _BV(PB7);
}
