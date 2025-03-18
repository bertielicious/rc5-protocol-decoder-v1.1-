#include "constVar.h"
#include "enablePulseLCD.h"
/* send 0x8X for line 1 text
   send 0xCX for line 2 text*/
void lcdCommand(uint8_t command)
{
     //write position to start writing on the LCD screen
    //write high nibble 0x80
    RS = LO;
    D7 = (command & 0b10000000)>>7;
    D6 = (command & 0b01000000)>>6;// if first line, HI if second line of display
    D5 = (command & 0b00100000)>>5;
    D4 =(command & 0b00010000)>>4;
     __delay_us(1000);
     enablePulseLCD();

  
     
    
    RS = LO;
    D7 = (command & 0b00001000)>>3;
    D6 = (command & 0b00000100)>>2;
    D5 = (command & 0b00000010)>>1;
    D4 = (command & 0b00000001);
     __delay_us(1000);
     enablePulseLCD();



}
