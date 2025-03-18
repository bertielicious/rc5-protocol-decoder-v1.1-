#include "constVar.h"
#include "enablePulseLCD.h"
void LCDwriteNum(uint16_t upperNib, uint16_t lowerNib)
{
    //send high byte to LCD  upperNib is always 0x30
    RS = HI;
    D7 = (upperNib & 0x80)>>7;
    D6 = (upperNib & 0x40)>>6;
    D5 = (upperNib & 0x20)>>5;
    D4 = (upperNib & 0x10)>>4;
    __delay_ms(1);
    enablePulseLCD();
    printf("UNib D7 %d\n", D7);
    printf("UNib D6 %d\n", D6);
    printf("UNib D5 %d\n", D5);
    printf("UNib D4 %d\n", D4);
    
    //send low byte to LCD   lower nib is the number we want ie 0 - 9
    RS = HI;
    D7= (lowerNib & 0x08)>>3;
    D6 = (lowerNib &0x04)>>2;
    D5 = (lowerNib & 0x02)>>1;
    D4 = (lowerNib & 0x01);
    __delay_ms(1);
    enablePulseLCD();
    printf("LNib D7 %d\n", D7);
    printf("LNib D6 %d\n", D6);
    printf("LNib D5 %d\n", D5);
    printf("LNib D4 %d\n", D4);
}
