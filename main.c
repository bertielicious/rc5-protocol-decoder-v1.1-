/*
 * File:   main.c
 * Author: User
 *
 * Created on 08 February 2025, 12:30
 */


#include "constVar.h"
#include "deviceConfigBits.h"
#include "configOsc.h"
#include "configPins.h"
#include "putch.h"
#include "configEUSART.h"
#include "configIOC.h"
#include "resetRC5.h"
#include "stateMachine.h"
#include "LCD1602init.h"
#include "enablePulseLCD.h"
#include "byteToNib.h"
#include "nibToBin.h"
#include "lcdCommand.h"
#include "clearLCD.h"
#include "LCDwriteData.h"
#include "LCDwriteNum.h"
//#include <stdbool.h>


void main(void) 
{
    //RW = LO;    //always write to LCD
    LED = LO;
    configOsc();
    configPins();
    configEUSART();
    configIOC();
    enablePulseLCD();
    LCD1602init();
    
 // uint8_t D4L, D5L, D6L, D7L, D4H, D5H, D6H, D7H;
    // splash screen "RC5 protocol"
    //                  "decoder"
    printf("RC5 protocol\n");
    // define the strings to be displayed on LCD
    uint8_t string[13] = "RC5 protocol";
    uint8_t string2[8] = "decoder";
    uint8_t device[7] = "Device";
    uint8_t tog[] = "Tog";
    uint8_t command[] = "Command";
    uint8_t hex[] = "hex";
    
    // variable declaration for extraction of RC5 data
    
    uint16_t toggleMask = 0x0800;
    uint16_t addressMask = 0x07c0;
    uint16_t commandMask = 0x003f;
    uint16_t temp = 0;
    uint16_t tempTog =  0;
   
    
    
    
    
     //define pointer to strings
    uint8_t *ascii;
    ascii = string;
    uint8_t lowN, highN = 0;
     
    fourBit binary;
    fourBit *ptr;
    ptr = &binary;
    
   lcdCommand(0x82);
    while(*ascii != 0)
    {  
        LCDwriteData(*ascii);
        ascii= ascii + 1;
    }
   
   ascii = string2;
   lcdCommand(0xc5);
   while(*ascii != 0)
    {  
        LCDwriteData(*ascii);   
        ascii= ascii + 1;
    }
   
   __delay_ms(1500);
   clearLCD();
   
  //print 'Device:' on LCD at position 0x80
   lcdCommand(0x80);
   ascii = device;  
   
   while(*ascii != 0)
    {  
        LCDwriteData(*ascii);   
        ascii= ascii + 1;
    }
   
   //print 'hex:' on LCD at position 0x0b
  // lcdCommand(0x80|0x0a);
   //ascii = hex;  
   
  // while(*ascii != 0)
  //  {  
   //     LCDwriteData(*ascii);   
  //      ascii= ascii + 1;
   // }
   
   //print 'Command:' on LCD at position 0x40
   lcdCommand(0x80|0x40);
   ascii = command;  
   
   while(*ascii != 0)
    {  
        LCDwriteData(*ascii);   
        ascii= ascii + 1;
    }
   
   //print 'Tog:' on LCD at position 0x40
   lcdCommand(0x80|0x4b);
   ascii = tog;  
   
   while(*ascii != 0)
    {  
        LCDwriteData(*ascii);   
        ascii= ascii + 1;
    }
   
    
    while(1)
    {
       while(IR == HI && has_run !=1)
           ;    // wait for first button press on RC5 handset
            while(IR == LO && has_run !=1)
                ;
            if(IR == HI && has_run !=1)
            {
                bits--;
                has_run = 1;                // this block only runs once until system is reset
                INTCONbits.IOCIE = 0;       // master switch disable for interrupt on change   
                RC5_code = RC5_code | 1<<bits; // generates initial RC5 code S1 of 0x2000
                bits--;
                RC5_code = RC5_code | 1<<bits; // generates initial RC5 code S2 of 0x1000
                bits--;                        // keep track of how many of the 14 RC5 bits have been consumed
                state = MID1;
                TMR1H = 0x00;
                TMR1L = 0x00;
                INTCONbits.IOCIE = 1;       // master switch enable for interrupt on change     
                IOCBPbits.IOCBP5 = 0;       // disable interrupt on rising edge
                IOCBNbits.IOCBN5 = 1;       // enable interrupt on falling edge
            }   
            
            else if(IOCIF !=1 && (bits + 1) == 0)  // if no interrupt and all 14 RC5 bits have been consumed by state machine
            {
                //printf("RC5 code = %x\n", RC5_code);
                
               
                //extract toggle bit from RC5 code, toggle bit is bit 3
               tempTog = (RC5_code & toggleMask) >> 11;
               //printf("tog %d\n", temp);
               lcdCommand(0x80|0x4f); 
               LCDwriteNum(0x30, tempTog);
               
               //extract 5-bit address        ;
               temp = (RC5_code & addressMask)>>6;
               //printf("device %d\n", temp);
               lcdCommand(0x87); 
               LCDwriteNum(0x30,temp/10);// tens digit
               
               lcdCommand(0x88); 
               LCDwriteNum(0x30,(temp%10));// units digit
               
               //extract 6-bit command
               temp = (RC5_code & commandMask);
              // printf("command %d\n", temp);
               lcdCommand(0x80|0x48); 
               LCDwriteNum(0x30,temp/10);// tens digit
               
               lcdCommand(0x80|0x49); 
               LCDwriteNum(0x30,temp%10);// units digit
               
               
               
                //extract RC5 code 
               
               
               
               //extract two start bits
               lcdCommand(0x80|0x0c);
               LCDwriteNum(0x30,(RC5_code>>12) & 0x0f);
               printf("%x\n", (RC5_code>>12) & 0x0f);
               
               //extract toggle bit
               tempTog = ((RC5_code >> 11) & 0x01);
               printf(" tempTog %x\n", tempTog);
               
               //extract
               uint8_t type = 0;
               uint8_t data = 0;
               if(tempTog == 1)
               {
                   type = 0x60;
                   data = ((RC5_code >> 8)& 0x0f) - 9;
               }
               else
               {
                   type = 0x30;
                   data = ((RC5_code >> 8  & 0x0f));
               }
               lcdCommand(0x80|0x0d);
               LCDwriteNum(type, data);
               
               printf("%x\n",(RC5_code >> 8)& 0x0f);
               
               lcdCommand(0x80|0x0e);
               LCDwriteNum(0x30,(RC5_code >> 4) & 0x0f);
               printf("%x\n", (RC5_code >> 4) & 0x0f);
               
               lcdCommand(0x80|0x0f);
               LCDwriteNum(0x30,(RC5_code) & 0x0f);
               printf("%x\n", (RC5_code>>0) & 0x0f);
               
               
               
               
               
               resetRC5();
              
                T1CONbits.TMR1ON = 0;
            
        }
       /*Test LCD IO all good
       LATAbits.LATA4 = ~LATAbits.LATA4;
       LATAbits.LATA5 = ~LATAbits.LATA5;
       LATBbits.LATB6 = ~LATBbits.LATB6;
       LATCbits.LATC3 = ~LATCbits.LATC3;
       LATCbits.LATC6 = ~LATCbits.LATC6;
       LATCbits.LATC2 = ~LATCbits.LATC2;
       LATBbits.LATB4 = ~LATBbits.LATB4;
       __delay_ms(1000);*/
       
        
    }
                
    return;
}

