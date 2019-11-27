#define LCD_DAT PORTK   // PORTK is used for data to the LCD
#define LCD_DIR DDRK    // Direction of LCD port
#define LCD_E 0x02      // LCD E signal
#define LCD_RS 0x01     // LCD Register Select signal

#define CMD 0   // Command type for put2lcd 
#define DATA 1  // Data type for put2lcd

#define CURSOR_DISPLAY_MASK 0b00000010
#define CURSOR_BLINK_MASK 0b00000001
#define DISPLAY_MASK 0b00000100
#define LINE_COUNT_MASK 0b00001000  // high - 2 line, low - 1 line
#define DATA_LENGTH_MASK 0b00010000 // high - 8-bit, low - 4-bit
#define FONT_SIZE_MASK 0b00000100   // high - 5x10, low - 5x7
#define FUNCTION_SET_MASK 0b00100000

char __displayState = 0b00001111;

void initLCD(void);
void displayOn(void);
void displayOff(void);
void cursorBlinkOn(void);
void cursorBlinkOff(void);
void cursorOn(void);
void cursorOff(void);
void setLTR(void);
void setRTL(void);
void moveCursorRight(void);
void moveCursorLeft(void);
void shiftCursorRight(int numSpaces);
void shiftCursorLeft(int numSpaces);
void switchLine(void);
void resetCursor(void);
void moveCursorHome(void);
void clearDisplay(void);
void puts2LCD (char *ptr);
void putc2LCD(char c);
void put2LCD(char c, char type);
void putCMD2LCD(char c);
void delay_50us(int n);
void delay_1ms(int n);

