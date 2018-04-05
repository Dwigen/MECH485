/* For controlling a 16x2 display with the HITACHI HD44780U (LCD-II) display
driver, using an AT90USBKEY2 AVR micro controller.
*/

#include <mydisplay.h>

//initalizes display using correct sequence from datasheet
void display_Init() {
  disp_Delay(20000);
  write_Data(0x30);
  disp_Delay(5000);
  write_Data(0x30);
  disp_Delay(200);
  write_Data(0x30);
  disp_Delay(10);
  write_Data(0x38);//Sets 8-bit operation and 5x8 character font
  write_Data(0x0C);//turn on display
  write_Data(0x06);//set to increment cursor on char write
  write_Data(0x01);//clear Display
  disp_Delay(2500);
  write_Data(0x02);//Set cursor to home
  disp_Delay(2500);
}

//displays number of items sorted into tray
void display_In_Tray(char alu, char stl, char blk, char wht) {
	write_Data(0x02);//Set cursor to home
	disp_Delay(2500);
  write_Char(0x49);//I
  write_Char(0x4E);//N
  write_Data(0xC0);//move cursor to start of 2nd line
  write_Char(0x54);//T
  write_Char(0x52);//R
  write_Char(0x41);//A
  write_Char(0x59);//Y
  write_Data(0x85);
  write_Char(0x42);//B
  write_Char(0x4B);//K
  write_Char(0x3A);//:
  integer_Convert(blk);
  write_Data(0x8B);
  write_Char(0x57);//W
  write_Char(0x54);//T
  write_Char(0x3A);//:
  integer_Convert(wht);
  write_Data(0xC5);
  write_Char(0x41);//A
  write_Char(0x4C);//L
  write_Char(0x3A);//:
  integer_Convert(alu);
  write_Data(0xCB);
  write_Char(0x53);//S
  write_Char(0x54);//T
  write_Char(0x3A);//:
  integer_Convert(stl);
}

//displays items detected but still on the belt, run after display_In_Tray
void display_On_Belt(char alu, char stl, char blk, char wht) {
	write_Data(0x02);//Set cursor to home
	disp_Delay(2500);
  write_Char(0x4F);//O
  write_Char(0x4E);//N
  write_Data(0xC0);
  write_Char(0x42);//B
  write_Char(0x45);//E
  write_Char(0x4C);//L
  write_Char(0x54);//T
  write_Data(0x88);
  integer_Convert(blk);
  write_Data(0x8E);
  integer_Convert(wht);
  write_Data(0xC8);
  integer_Convert(alu);
  write_Data(0xCE);
  integer_Convert(stl);
}

//sets up diplay for calibration
void display_Calibration(int count, int max, int min, int avg){
	write_Data(0x02);//Set cursor to home
	disp_Delay(2500);
  write_Char(0x43);//C
  write_Char(0x4E);//N
  write_Char(0x54);//T
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

//updates calibration values
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

//converts integers into single digits and prints them
void integer_Convert(int in){
  int cnt = int_count(in);
  int intArray[cnt];
  int othercnt = cnt;
  while(cnt >= 0){
    intArray[cnt-1] = in % 10;
    in /= 10;
	cnt--;
  }
  int i = 0;
  for (i = 0; i < othercnt; i++) {
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
  while(i<3){
    write_Char(0x20);//" "
	i++;
  }
}

//writes config instruction to display driver
void write_Data(char in) {
  controlPort &= INVRS;
  dataPort = in;
  controlPort |= EN;
  disp_Delay(1);
  controlPort ^= EN;
  disp_Delay(1);
  controlPort |= RS;
  disp_Delay(100);
}

//writes character to display takes hex value of char
void write_Char(char in) {
  controlPort |= RS;
  disp_Delay(1);
  dataPort = in;
  controlPort |= EN;
  disp_Delay(1);
  controlPort ^= EN;
  disp_Delay(1);
  controlPort &= INVRS;
}

//count number of digits in integer
int int_count(int in){
  int cnt = 1;
  while (in /= 10) {
    cnt++;
  }
  return cnt;
}

// microsecond delay function
void disp_Delay(int count){
	TCCR3B|= (_BV(WGM32) | _BV(CS30));
	OCR3A = 0x0001; //one clock cycle timer.
	TCNT3 = 0x0000;
	TIFR1 |=_BV(OCF3A);
	int i = 0;
	while (i<count) {
		if((TIFR3 & 0x02) == 0x02){
			TIFR3 |=_BV(OCF3A);
			i++;
		}
	}
	return;
}
