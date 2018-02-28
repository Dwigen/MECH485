/*
#################################################
MILESTONE: 5
PROGRAM: 2
PROJECT:
GROUP: 2
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00822833
DESC: ADC input Controlling the pulse width of the DC motor running the belt
DATA
REVISED
#################################################
*/

//####### Include Files #######
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//########### Prototypes ############
void trayTimerInit();
void counterTimerInit();
void beltMotor(int beltDir);
void PWMInit();
void delayms(int count);

//########## Variable Definitions ######
#define stepOne 0b00110000
#define stepTwo	0b00000110
#define stepThree 0b00101000
#define stepFour 0b00000101
#define backward 0b00001000
#define forward	0b00000100
#define brake 0b00000000
#define clkwise 0
#define cclkwise 1
#define stop 2
#define black	0
#define steel 50
#define white 100
#define aluminum 150

//########## GLOBAL VARIABLES #######
volatile unsigned char stepPosition = 1;
volatile unsigned char trayPosition = 0;
volatile unsigned char trayDirection = stop;//Keeps track of desired tray rotation direction
volatile unsigned long timeCounter = 0;//keeps track of time since start in ms
volatile unsigned char ADC_result;
volatile unsigned char ADC_result_flag;
volatile unsigned char direction_flag;
volatile unsigned char direction_change_flag;

//########## MAIN ROUTINE ###########
int main(int argc, char const *argv[]) {

	link *head;			/* The ptr to the head of the queue */
	link *tail;			/* The ptr to the tail of the queue */
	link *newLink;		/* A ptr to a link aggregate data type (struct) */
	link *rtnLink;		/* same as the above */
	element eTest;		/* A variable to hold the aggregate data type known as element */
	rtnLink = NULL;
	newLink = NULL;

	// Set timers to run at CPU Clock, Use as pure timer
	TCCR0B |=_BV(CS10);
	TCCR1B |=_BV(CS10);
	TCCR3B |=_BV(CS10);

	cli(); //ensure interrupts are disabled

	//enable EXTERNAL interrupts
	EIMSK |= (_BV(INT5)); //enable EXTERNAL interrupt 5
	EICRB |= (_BV(ISC51) | _BV(ISC50)); //set INT5 to Rising Edge

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
	PORTB = 0x00;

	sei(); //enable global interrupts

	counterTimerInit();
	PWMInit();

	ADCSRA |= _BV(ADSC); //initalize ADC, Start Auto Conversion in free run mode (Default)

	while (1) {
		char tray_ready = false;
		 if(abs(trayPosition - head->e.material) < 15){
			 tray_ready = TRUE;
		 }

		if(direction_change_flag==TRUE){
			direction_change_flag = FALSE;
			beltMotor(brake);
			delayms(50);
			if(direction_flag == forward){
				beltMotor(backward);
				direction_flag = backward;
			}
			else{
				beltMotor(forward);
				direction_flag = forward;
			}
		}//End Direction Change if else
	}//End Infinite While
}//End Main

//Updated the register controlling DC motor dutyCycle evey time a conversion occurs
ISR(ADC_vect){
	OCR0A = ADCH;
	PORTC = ADC_result;
	ADCSRA |= _BV(ADSC); //initalize ADC, Start Auto Conversion in free run mode (Default)
}//ADC ISR

ISR(INT5_vect){
	direction_change_flag = TRUE;
}//External Interrupt 5 ISR

ISR(INT6_vect){
	tray_home_flag = TRUE;
}//External Interrupt 6 ISR

//rotates the tray 1 step every time timer 1 A interrupt is triggered
ISR(TIMER1_COMPA_vect){
	if (trayDirection == clkwise) {
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
		if(trayPosition == 199){
			trayPosition = 0;
		}
		else{
			trayPosition++;
		}
	}
	else if (trayDirection == cclkwise){
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
		if(trayPosition == 0){
			trayPosition = 199;
		}
		else{
			trayPosition--;
		}
	}
	else{
		//do NOTHING
	}
}//Timer 1 (rotate tray) ISR

//Increments the time counter every millisecond
ISR(TIMER3_COMPA_vect){
	timeCounter++;
}

//Delays for specified number of milliseconds
void delayms(int count){
	int startTime = timeCounter;
	while (timeCounter - startTime != count) {
	}
	return;
}

//updates the direction of the belt motor
void beltMotor(int beltDir) {
	PORTB = beltDir;
	return;
}

void PWMInit() {
	TCCR0A |= _BV(WGM01);
	TCCR0A |= _BV(WGM00);
	//TIMSK0 = TIMSK0 |0b00000010;
	TCCR0A |= _BV(COM0A1);
	TCCR0B |= _BV(CS01);
	OCR0A = 127;
	DDRB |= _BV(PB7);
}

void trayTimerInit(){
	TCCR1B|=_BV(WGM12);
	OCR1A = 0x3A98; //Trigger Interrupt A every 15ms Max is 65ms
	TCNT1 = 0x0000;
	TIMSK1 = TIMSK1 |0b00000010;
	TIFR1 |=_BV(OCF1A);
	return;
}

void counterTimerInit(){
	TCCR3B|=_BV(WGM12);
	OCR3A = 0x03E8; //Trigger Interrupt A every 1ms
	TCNT3 = 0x0000;
	TIMSK3 = TIMSK3 |0b00000010;
	TIFR3 |=_BV(OCF3A);
	return;
}
//ADC Notes
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
