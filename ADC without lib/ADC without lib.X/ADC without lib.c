/* 
 * File:   ADC without lib.c
 * Author: Obiano Miracol
 *
 * Created on April 15, 2019, 11:19 PM
 */
#include <stdio.h>
#include "config.c"
#include <xlcd.h>
#include <string.h>

/*
 * 
 */
int ADCResult=0;
float voltage;
char ResultString[20];

void initialize_XLCD(void);     //initializing the LCD
unsigned int ADCcalc(int channel);
void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);

void main() {
    OSCCON=0X77;
    TRISA=0xff;
    ADCON0=0X49;
    ADCON1=0XC2;
    
    initialize_XLCD();
    while(BusyXLCD());
    putrsXLCD("ADC(WITHOUT LIB)");
    SetDDRamAddr(0x40);
    putrsXLCD("DISPLAY VOLTAGE");
    WriteCmdXLCD(0x0C);
    for(int x=0; x<=20; x++)__delay_ms(100);     //2 seconds delay
    WriteCmdXLCD(0x01);
    while(1){
        ADCResult=0;
        ADCResult=ADCcalc(1);       //Select channel used
        voltage=((ADCResult*5.0)/1023);
        SetDDRamAddr(0x00);
        putrsXLCD("VOLTAGE=");
        sprintf(ResultString,"%0.5f",voltage);
        putrsXLCD(ResultString);
        putrsXLCD("V");
        putrsXLCD("  ");
    }
}
void initialize_XLCD(void){
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    WriteCmdXLCD(0x06);
    WriteCmdXLCD(0x0F);
}
unsigned int ADCcalc(int channel){
    ADON=1;
    ADCS0=0;
    ADCS1=1;
    ADCS2=1;
    ADCON0bits.CHS=channel;
    ADFM=1;
    __delay_ms(1);
    GO_DONE=1;
    while(ADCON0bits.GO_DONE==1);
    unsigned int adval = (ADRESH<<8)+ADRESL;
    return adval;
}
//Delay for 18 cycles
void DelayFor18TCY(void){
    Delay10TCYx(20);
}
//Delay for 15ms
void DelayPORXLCD(){
    Delay1KTCYx(30);
}
//Delay for 5ms
void DelayXLCD(void){
    Delay1KTCYx(10);
}
