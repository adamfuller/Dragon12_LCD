#include "derivative.h"
#include "lcd.h"

/**
 * Sets up the LCD for 4-bit data, 2 lines, 5x7 font,
 * RtL text direction, with a blinking cursor.
 **/
void initLCD(void){
	LCD_DIR = 0xFF;    // Configure LCD_DAT for output
	delay_1ms(100);    // wait for LCD to be ready;
  putCMD2LCD(FUNCTION_SET_MASK & ~DATA_LENGTH_MASK | LINE_COUNT_MASK & ~FONT_SIZE_MASK);		
	displayOn();       // turn on display, show cursor, blink cursor
	setLTR();
  clearDisplay();
}

/**
  Activates the display
*/
void displayOn(void){
  __displayState |= DISPLAY_MASK;
  putCMD2LCD(__displayState);
}


/**
  Disable showing characters on the display
  Characters can still be added and removed
  but changes will not be shown.
*/
void displayOff(void){
  __displayState &= ~DISPLAY_MASK;
  putCMD2LCD(__displayState);
}

/**
 *  Enables cursor blinking. 
 */
void cursorBlinkOn(void){
   __displayState |= CURSOR_BLINK_MASK;
  putCMD2LCD(__displayState);
}

/**
 * Disables cursor blinking.
 * Stops the whole block from blinking,
 * To hide entire cursor use with cursorOff.
 * */
void cursorBlinkOff(void){
   __displayState &= ~CURSOR_BLINK_MASK;
  putCMD2LCD(__displayState);
}

/**
 * Shows the cursor on the display.
 * The cursor is the little line on the bottom.
 **/
void cursorOn(void){
   __displayState |= CURSOR_DISPLAY_MASK;
  putCMD2LCD(__displayState);
}

/**
 * Prevents the cursor from being shown.
 * The cursor is the little line on the bottom.
 **/
void cursorOff(void){
  __displayState &= ~CURSOR_DISPLAY_MASK;
  putCMD2LCD(__displayState);
}

/**
 * Set text direction from left to right
 */
void setLTR(void){
  putCMD2LCD(0x06);// Cursor goes left to right
}

/**
 * Set text direction from right to left
 */
void setRTL(void){
  putCMD2LCD(0x04);// Cursor goes right to left
}

/**
  Shifts the active cursor on the LCD
  display to the right by 1 section.
  The cursor will wrap to the second
  row when it passes 40 lines
*/
void moveCursorRight(void){
  put2LCD(0b00010100, CMD); 
}

/**
  Shifts the active cursor on the LCD
  display to the left by 1 section.
*/
void moveCursorLeft(void){
  put2LCD(0b00010000, CMD);
}

/**
  Shifts the cursor right by n
  spaces.
*/
void shiftCursorRight(int numSpaces){
   for(;numSpaces>0;numSpaces--) moveCursorRight();
}

/**
  Shifts the cursor left by n
  spaces.
*/
void shiftCursorLeft(int numSpaces){
   for(;numSpaces>0;numSpaces--) moveCursorLeft();
}

/**
 * Togles the current line,
 * if only one line nothing should happen
 */
void switchLine(void){
  shiftCursorRight(40); 
}

/**
 * Moves the cursor back to
 * its home position, top left.
 * 
 */
void resetCursor(void){
  putCMD2LCD(0x80); 
}

/**
 * Moves the cursor back to 
 * its home position, top left.
 */
void moveCursorHome(void){
  resetCursor();
}
  
/**
 * Removes all characters from
 * the display, and moves the 
 * cursor back to the top left. 
 **/
void clearDisplay(void){
  put2LCD(0x01, CMD);
  delay_1ms(2); 
}

/**
 * Displays a string on the LCD.
 */
void puts2LCD(char *ptr){
  int i = 0;
  for (;ptr[i];i++)
    putc2LCD(ptr[i]);
  
}

/**
 * Display a single character
 * on the LCD.
 */
void putc2LCD(char c){
	LCD_DAT |= LCD_RS;
  LCD_DAT = ((c & 0xF0) / 4)|LCD_E|LCD_RS;
	LCD_DAT |= LCD_E;
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_DAT &= (~LCD_E);
  LCD_DAT = ((c & 0x0F) * 4)|LCD_E|LCD_RS;
	LCD_DAT |= LCD_E;
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_DAT &= (~LCD_E);
  delay_50us(2); 
}

void put2LCD(char c, char type){
	char c_lo, c_hi;
	
	c_hi = (c & 0xF0) >> 2;  // Snag upper 4 bits of c
	c_lo = (c & 0x0F) << 2;  // Snag lower 4 bits of c
	if (type == DATA) LCD_DAT |= LCD_RS;
	else LCD_DAT &= (~LCD_RS);
	if (type == DATA)
		LCD_DAT = c_hi|LCD_E|LCD_RS;
	else
		LCD_DAT = c_hi|LCD_E;
	LCD_DAT |= LCD_E;
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_DAT &= (~LCD_E);
	if (type == DATA)
		LCD_DAT = c_lo|LCD_E|LCD_RS;
	else
		LCD_DAT = c_lo|LCD_E;
	LCD_DAT |= LCD_E;
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_DAT &= (~LCD_E);
	delay_50us(1);
}


/**
 * Send a command to the
 * LCD controller.
 */
void putCMD2LCD(char c){
  char c_lo, c_hi;
	
	c_hi = (c & 0xF0) >> 2;  // Snag upper 4 bits of c
	c_lo = (c & 0x0F) << 2;  // Snag lower 4 bits of c

	LCD_DAT &= (~LCD_RS);    // select LCD instruction register
  LCD_DAT = c_hi | LCD_E;  // Set PORTK to data
	LCD_DAT |= LCD_E;        // Set E high
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_DAT &= (~LCD_E);    // Clear E pin
  LCD_DAT = c_lo|LCD_E;   
	LCD_DAT |= LCD_E;       // Pull E high
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_DAT &= (~LCD_E);
	delay_50us(1);
}

/**
 * Delay by n intervals of 50us
*/
void delay_50us(int n){
	volatile int c;
	for (;n>0;n--)
		for (c = 133; c>0;c--);
}

/**
 * Delay by n milliseconds.
 **/
void delay_1ms(int n){
	for (;n>0;n--) delay_50us(20);
}
