/* For controlling a 16x2 display with the HITACHI HD44780U (LCD-II) display
driver, using an AT90USBKEY2 AVR micro controller.
*/

#include <mydisplay.h>


void display_Init() {
  disp_Delay(200);
  write_Data(0x38);//Sets 8-bit operation and 5x8 character font
  disp_Delay(50);
  write_Data(0x0C);//turn on display
  write_Data(0x06);//set to increment cursor on char write
  write_Data(0x01);//clear Display
  write_Data(0x02);//Set cursor to home
}

void display_In_Tray(char alu, char stl, char blk, char wht) {
  write_Char(0x49);//I
  write_Char(0x4E);//N
  write_Data(0xC0);//move cursor to start of 2nd line
  write_Char(0x56);//T
  write_Char(0x52);//R
  write_Char(0x41);//A
  write_Char(0x59);//Y
  write_Data(0x85);
  write_Char(0x42);//B
  write_Char(0x4B);//K
  write_Char(0x3A);//:
  integer_Convert(blk);
  write_Char(0x20);//" "
  write_Char(0x57);//W
  write_Char(0x56);//T
  write_Char(0x3A);//:
  integer_Convert(wht);
  write_Data(0xC5);
  write_Char(0x41);//A
  write_Char(0x4C);//L
  write_Char(0x3A);//:
  integer_Convert(alu);
  write_Char(0x20);//" "
  write_Char(0x53);//S
  write_Char(0x56);//T
  write_Char(0x3A);//:
  integer_Convert(stl);
}

void display_On_Belt(char alu, char stl, char blk, char wht) {
  write_Char(0x4F);//O
  write_Char(0x4E);//N
  write_Data(0xC0);
  write_Char(0x42);//B
  write_Char(0x45);//E
  write_Char(0x4C);//L
  write_Char(0x56);//T
  write_Data(0x88);
  integer_Convert(blk);
  write_Data(0x8E);
  integer_Convert(wht);
  write_Data(0xC8);
  integer_Convert(alu);
  write_Data(0xCE);
  integer_Convert(stl);
}

void display_Calibration(int count, int max, int min, int avg){
  write_Char(0x43);//C
  write_Char(0x4E);//N
  write_Char(0x56);//T
  write_Char(0x3A);//:
  integer_Convert(count);
  write_Data(0x88);
  write_Char(0x4D);//M
  write_Char(0x41);//A
  write_Char(0x58);//X
  write_Char(0x3A);//:
  integer_Convert(max);
  write_Data(0xC0);
  write_Char(0x41);//A
  write_Char(0x56);//V
  write_Char(0x47);//G
  write_Char(0x3A);//:
  integer_Convert(avg);
  write_Data(0xC8);
  write_Char(0x4D);//M
  write_Char(0x49);//I
  write_Char(0x4E);//N
  write_Char(0x3A);//:
  integer_Convert(min);
}

void update_Calibration(int count, int max, int min, int avg){
  write_Data(0x84);
  integer_Convert(count);
  write_Data(0x8C);
  integer_Convert(max);
  write_Data(0xC4);
  integer_Convert(avg);
  write_Data(0xCC);
  integer_Convert(min);
}

void integer_Convert(int in){
  int count = count(num);
  int intArray[count];
  while(count--){
    arr[count] = in % 10;
    in /= 10;
  }
  for (size_t i = 0; i < count; i++) {
    switch (intArray[i]) {
      case 0:
        write_Char(0x30);
        break;
      case 1:
        write_Char(0x31);
        break;
      case 2:
        write_Char(0x32);
        break;
      case 3:
        write_Char(0x33);
        break;
      case 4:
        write_Char(0x34);
        break;
      case 5:
        write_Char(0x35);
        break;
      case 6:
        write_Char(0x36);
        break;
      case 7:
        write_Char(0x37);
        break;
      case 8:
        write_Char(0x38);
        break;
      case 9:
        write_Char(0x39);
        break;
      default:
        break;
    }
  }
  if(count == 1){
    write_Char(0x20);//" "
  }
}

void write_Data(char in) {
  controlPort &= INVRS;
  dataPort = in;
  controlPort |= EN;
  disp_Delay(datadelay);
  controlPort ^= EN;
}

void write_Char(char in) {
  controlPort |= RS;
  dataPort = in;
  controlPort |= EN;
  disp_Delay(charDelay);
  controlPort ^= EN;
}

int count(int in){
  int cnt = 1;
  while (in /= 10) {
    cnt++;
  }
  return cnt;
}

void disp_Delay(unsigned long waitTime){
  unsigned long tempTime = (waitTime + timeCount);
  while(tempTime > timeCount){
    //delay for waitTime x .1ms
  }
}

ISR(TIMER3_COMPA_vect){
  timeCount++;
}

void init_usCounter(){
	TCCR3B|= (_BV(WGM12) | _BV(CS30));
	OCR3A = 0x0064; //interrupt every .1 ms @ 100MHz clk
	TCNT3 = 0x0000;
	TIMSK3 = TIMSK3 |0b00000010;
	TIFR1 |=_BV(OCF3A);
	return;
}
