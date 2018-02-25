/*
#################################################
MILESTONE: 5
PROGRAM: 1
PROJECT:
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: ADC input displayed on the 8 LEDs
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
int stepdelay = 15; //10 is too low
int stepPosition = 1;
int trayPosition = 0;
int direction = 1;
volatile unsigned char ADC_result;
volatile unsigned int ADC_result_flag;

//########## Variable Definitions ######
#define stepOne 0b00110000
#define stepTwo	0b00000110
#define stepThree 0b00101000
#define stepFour 0b00000101

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

	// Set timer one to run at CPU Clock, Use as pure timer
	TCCR1B |=_BV(CS10);

  cli(); //ensure interrupts are disabled

  //ADC Config
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADIE);
  ADMUX |= _BV(ADLAR) | _BV(REFS0);

	sei(); //enable global interrupts

	// Set Data Direction Registers
	DDRA = 0xFF;
	DDRC = 0xFF;
	PORTA = 0x00;
	PORTC = 0x00;

  ADCSRA |= _BV(ADATE); //initalize ADC, Start Auto Conversion in free run mode (Default)

  while (1) {
    if (ADC_result_flag) {
      PORTC = ADC_result;
      ADC_result_flag = 0;
    }
  }
}

ISR(ADC_vect){
  ADC_result = ADCH;
  ADC_result_flag = 1;
}


//Counter function counts milliseconds
void delayms(int count){
	int i;
	i = 0;
	TCCR1B|=_BV(WGM12);
	OCR1A = 0x03E8;
	TCNT1 = 0x0000;
	TIMSK1 = TIMSK1 |0b00000110;
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
	TIMSK3 = TIMSK3 |0b00000010;
	TIFR3 |=_BV(OCF3A);
	return;
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
	TCCR0A |= _BV(WGM01);
	TCCR0A |= _BV(WGM00);
	TIMSK0 = TIMSK0 |0b00000010;
	TCCR0A |= _BV(COM0A1);
	TCCR0B |= _BV(CS01);
	OCR0A = 127;
	DDRB |= _BV(PB7);
}

void ADC(/* arguments */) {
    /*
    set ADLAR bit in ADMUX for left adjusted allowing reading of ADCH for a 8
    bit input
    single conversion by writing logical 1 to ADSC
    default ADC input is PF0
    _BV(REFS0) sets reference voltage to AREF ## NEED EXTERNAL CAP ON AREF PIN ##
    ADMUX = 0x60 or ADMUX =| _BV(ADLAR) | _BV(REFS0)
    Last 4 bits of ADMUX choose input pin. 0000 sets ADC0 or PF0
    ADCSRA - ADC Control and Status Register A
    ADCSRA |= _BV(ADEN) enables the ADC
    ADCSRA |= _BV(ADSC) Starts ADC single conversion
    ADCSRB - ADC Control and Status Register B
    ADCSRB last 3 bits select mode 000 sets free running mode, running indefinitely?
    */
}
