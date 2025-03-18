#include "constVar.h"
#include "enablePulseLCD.h"
void LCDwriteData(uint8_t data)
{
   
    
    //send high byte to LCD
    RS = HI;
    D7 = (data & 0x80)>>7;
    D6 = (data & 0x40)>>6;
    D5 = (data & 0x20)>>5;
    D4= (data & 0x10)>>4;
    __delay_ms(1);
    enablePulseLCD();
    
    //send low byte to LCD
    RS = HI;
    D7= (data & 0x08)>>3;
    D6 = (data & 0x04)>>2;
    D5 = (data & 0x02)>>1;
    D4 = (data & 0x01);
    __delay_ms(5);
    enablePulseLCD();
    
    
}
