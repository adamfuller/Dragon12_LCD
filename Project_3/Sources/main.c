#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "lcd.c"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "timer.c"


void init(void);


char output[] = "X: 0.00";
char output2[] = "Y: 0.00";

void main(void) {
  init();
  loop: // Use label to avoid "Statement always true" warning
  
    ATD0CTL5 = 0x86;// ATD 6
    ATD1CTL5 = 0x86;// ATD 14
    while(!ATD0STAT0 &ATD0STAT0_SCF_MASK); // ATD 6
    while(!ATD1STAT0 &ATD1STAT0_SCF_MASK); // ATD 14
    
    sprintf(output, "X: %d.%d", (ATD1DR0 * 10)/2046, (((ATD1DR0 * 10) % 2046)*10)/2046);
    sprintf(output2, "Y: %d.%d", (ATD0DR0 * 10)/2046, (((ATD0DR0 * 10) % 2046)*10)/2046);
    moveCursorHome();
    puts2LCD(output);
    switchLine();
    shiftCursorLeft(strlen(output));
    puts2LCD(output2);
    switchLine();
    delay_1ms(100);
    updateTimerPeriod(ATD0DR0 + ATD1DR0);
    
  goto loop;
}


void tickUpPortB(void){
  PORTB++;  
}



void init(void){
  initLCD();
  cursorBlinkOff();
  cursorOff();
  clearDisplay();
  
  // Init LEDs
  DDRJ = 0x02;
  PTJ &= 0xFD; // clear bit 1
  DDRB = 0xFF;
  PORTB = 0x00;
  DDRP = 0x7F;
  
  initInterrupt(10000, &tickUpPortB);
  
  // Init ADC
  ATD0CTL2 = 0xE0;// ATD 6
  ATD1CTL2 = 0xE0;// ATD 14
  delay_50us(4);  ATD0CTL3 = 0x22;// ATD 6
  ATD0CTL4 = 0x05;// ATD 6
  ATD1CTL3 = 0x22;// ATD 14
  ATD1CTL4 = 0x05;// ATD 14
}