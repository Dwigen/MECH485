/*
#################################################
MILESTONE: 1
PROGRAM: 1
PROJECT: MyFirstProject
GROUP: X
NAME 1: Daniel, Wigen, V00797593
NAME 2: Nicola, Watts, V00
DESC: This program does
DATA
REVISED
#################################################
*/

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay_basic.h>

//################ MAIN ROUTINE ##############
int main(int argc, char const *argv[]) {
  DDRD = 0xFF;
  DDRC = 0xFF;
  PORTD = 0xA0; //Lab 1 part one. Turn on LEDs
  PORTC = 0x00;
  //knightrider(); //lab 1 Part 2, comment out for part one
  return 0;
}

void delaynus(int n) {
  int k;
  for(k=0; k<n, k++)
    _delay_loop_1(1);
}

void delaynms(int n) {
  int k;
  for (k = 0; k < n; k++)
    delaynus(1000);
}

void out_in() {
  PORTC = 0x18;
  delaynms(delayTime);
  PORTC = 0x24;
  delaynms(delayTime);
  PORTC = 0x42;
  delaynms(delayTime);
  PORTC = 0x81;
  delaynms(delayTime);
  PORTC = 0x42;
  delaynms(delayTime);
  PORTC = 0x24;
  delaynms(delayTime);
  PORTC = 0x18;
  delaynms(delayTime);
  }
}

void back_forth() {
  PORTC = 0x01;
  for (size_t i = 0; i < 7; i++) {
    delaynms(delayTime)
    PORTC = PORTC << 1;
  }
  for (size_t i = 0; i < 7; i++) {
    delaynms(delayTime)
    PORTC = PORTC >> 1;
  }
  delaynms(delayTime);
}
void knightrider() {
  delayTime = 50; //Delay time between led changes in ms
  out_in();
  out_in();
  back_forth();
  back_forth();
  PORTC = 0x00;
  PORTD = 0x00;
}
