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

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)

// Function Prototypes
void swDelay(char numLoops);

//Global Variables
unsigned long int timer;
char CircIndex = 255;
unsigned int currentTemp = 0;
unsigned int swVolt = 0;
unsigned int tempArray[256];
unsigned int scrollArray[256];
int CheckADC = -1;
volatile float temperatureDegC;

#pragma vector = TIMER2_A0_VECTOR

__interrupt void A2_ISRs() {
    timer++;
    CircIndex++;
    ADC12CTL0 |= ADC12ENC | ADC12SC;
}
void A2_timer() {
    TA2CTL = TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 32767;   //it means we have 80 periods of 32 Khz each
    TA2CCTL0 = CCIE;
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR()
{
    currentTemp = ADC12MEM0;
    swVoltage = ADC12MEM1;
    CheckADC = 1;
}



// Main
void main(void)

{
    while(1) {
        if (sample == 1) {
            tempArray[circIndex] = currentTemp;
            ScrollArray[circIndex] = swVolt;
            sample = 0;
            if (circIndex == 255) {
                int average = tempArray[251] + tempArray[252] + tempArray[253] + tempArray[254] + tempArray[255];
            }
            temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit +30.0;

            Graphics_clearDisplay(&g_sContext);
            Graphics_drawStringCentered(&g_sContext, "HI", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
        }
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

    unsigned int somethingFun = 0x2121;

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    // Some utterly useless instructions -- Step through them
    // What size does the Code Composer MSP430 Compiler use for the
    // following variable types? A float, an int, a long integer and a char?
    flt = flt*tst;
    X = tst+X;
    tst = tst-myGrade;    // A number minus a letter?? What's actually going on here?
                          // What value stored in myGrade (i.e. what's the ASCII code for "A")?
                          // Thus, what is the new value of tst? Explain?

    // Useful code starts here
    initLeds();

    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Lab3", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);

    // Draw a box around everything
    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);

    dispThree[0] = ' ';
    dispThree[2] = ' ';

    while (1)    // Forever loop
    {
        // Check if any keys have been pressed on the 3x4 keypad
        currKey = getKey();
        if ((currKey >= '0') && (currKey <= '9'))
            setLeds(currKey - 0x30);
        if (currKey == '*')
            BuzzerOn();
        if (currKey == '#')
            BuzzerOff();

        if (currKey)
        {
            dispThree[1] = currKey;
            // Draw the new character to the display
            Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 48, 55, OPAQUE_TEXT);

            // Refresh the display so it shows the new data
            Graphics_flushBuffer(&g_sContext);

            // wait awhile before clearing LEDs
            swDelay(1);
            setLeds(0);
        }


    }  // end while (1)
}


void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i,j;	// volatile to prevent optimization
			                            // by compiler

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}

char stringConverter(float tempNum) {
    char str[4] = tempNum;
}
