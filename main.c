/************** ECE2049 DEMO CODE ******************/
/**************  20 August 2016   ******************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)

// Function Prototypes
void swDelay(char numLoops);
char* stringConverter(float printNum);
char* stringConverter(float printNum);
void A2_timer();


//Global Variables
unsigned long int timer;
char circIndex = 255;
unsigned int currentTemp = 0;
unsigned int swVolt = 0;
unsigned int tempArray[256];
unsigned int scrollArray[256];
int checkADC = -1;

#pragma vector = TIMER2_A0_VECTOR
__interrupt void A2_ISRs() {
    timer++;
    circIndex++; //circular index
    ADC12CTL0 |= ADC12ENC | ADC12SC; //enables and starts conversion
}
void A2_timer() {
    TA2CTL = TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 32767;   //Every Hz is 32767
    TA2CCTL0 = CCIE;    //enabling interrupt
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR()
{
    currentTemp = ADC12MEM0;
    swVolt = ADC12MEM1;
    checkADC = 1;
}


// Main
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer
       _BIS_SR(GIE);

    volatile float temperatureDegF;
    volatile float temperatureDegC;
    volatile float degC_per_bit;
    volatile unsigned int bits30, bits85;
    degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));

    initLeds();
    configDisplay();
    initADC();
    A2_timer();
    setLeds(15);
    swDelay(5);
    ledOff();

    while(1) {
        if (checkADC == 1) {
            tempArray[circIndex] = currentTemp;
            scrollArray[circIndex] = swVolt;
            checkADC = 0;
            int average = 0;
            if (circIndex >= 5) {
                average = (tempArray[circIndex-4] + tempArray[circIndex-3] + tempArray[circIndex-2] + tempArray[circIndex-1] + tempArray[circIndex])/5;
            }
            temperatureDegC = (float)((long)average - CALADC12_15V_30C) * degC_per_bit +30.0;
            temperatureDegF = temperatureDegC * 1.8 + 32;

            //char celsius[] = stringconverter(temperatureDegC);
            //char* farenheit[] = stringconverter(temperatureDegF);
            //char celciusDeg[] = {celsius[0],celsius[1],celsius[2],celsius[3],celsius[4],'C'};
            if ((circIndex % 5) == 0){
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "Celsius:", AUTO_STRING_LENGTH, 38, 35, TRANSPARENT_TEXT);
                //Graphics_drawStringCentered(&g_sContext, celsius, AUTO_STRING_LENGTH, 58, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Farenheit:", AUTO_STRING_LENGTH, 38, 55, TRANSPARENT_TEXT);
                //Graphics_drawStringCentered(&g_sContext, stringConverter(temperatureDegF), AUTO_STRING_LENGTH, 58, 55, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
            }
        }
        dateConverter(timer);
        BuzzerOnNote(swVolt);
    }
    //unsigned char ret_val = 0x0F;
    unsigned char currKey=0, dispSz = 3;
    unsigned char dispThree[3];

    // Define some local variables
    float flt = 190.68;
    int  tst = 0x0400, i=0;   // In C prefix 0x means the number that follows is in hex
    long unsigned X= 200022;   // No prefix so number is assumed to be in decimal
    unsigned char myGrade='A';
    unsigned char initial='J';
    //unsigned char name[14] = "Your Name Here"; // What happens when you change the array length?
                                        // What should it be?

}


void swDelay(char numLoops)
{
	volatile unsigned int i,j;	// volatile to prevent optimization			                            // by compiler

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}

char* stringConverter(float printNum) {
    char str[5];
    itoa(printNum, str[4], 10);
}

void dateConverter(long int total) {
    char* month;
    int dayOf = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int days = total/86400;
    if (days < 31) {
        month = "Jan";
        dayOf = days + 1;
    }
    else if (days < 59) {
        month = "Feb";
        dayOf = days - 30;
    }
    else if (days < 90) {
        month = "Mar";
        dayOf = days - 58;
    }
    else if (days < 120) {
        month = "Apr";
        dayOf = days - 89;
    }
    else if (days < 151) {
        month = "May";
        dayOf = days - 119;
    }
    else if (days < 181) {
        month = "Jun";
        dayOf = days - 150;
    }
    else if (days < 212) {
        month = "Jul";
        dayOf = days - 180;
    }
    else if (days < 243) {
        month = "Aug";
        dayOf = days - 211;
    }
    else if (days < 273) {
        month = "Sep";
        dayOf = days - 242;
    }
    else if (days < 304) {
        month = "Oct";
        dayOf = days - 272;
    }
    else if (days < 334) {
        month = "Nov";
        dayOf = days - 303;
    }
    else if (days < 365) {
        month = "Dec";
        dayOf = days - 333;
    }
    hour = (total - ((long)days * 86400))/3600;
    minute = (total - ((long)days * 86400)-(hour * 3600))/60;
    second = total - ((long)days * 86400)-(hour * 3600)-(minute * 60);
    //int date[] = {}
    Graphics_clearDisplay(&g_sContext);
    //Graphics_drawStringCentered(&g_sContext, date, AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}
