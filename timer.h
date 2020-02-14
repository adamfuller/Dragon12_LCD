#define INTERRUPTS_HIGH 0x08 // 8 calls offset by 0.024ms each time
#define INTERRUPTS_LOW 0x07  // 7 calls offset by 0.104 each time
#define CALLS_HIGH 13        // 13 calls at high interrupt count
#define CALLS_LOW 3          // 3 calls at low interrupt count
/**
 * To equalize uneven timing:
 * 13 calls at 8 ticks per call
 * 3 calls at 7 ticks per call
 **/

int __interruptCount = 0;
int __interruptsNeeded = INTERRUPTS_HIGH;
int __ticksCount = 0;
int __ticksToSwitch = CALLS_HIGH;
int __msCount = 0;
int __callEverymS = 0;

// Split up the calls so the offset is more even
int __callCounts[5] = {5, 1, 4, 2, 4};
unsigned char __callIndex = 0;
const unsigned char __callLength = 5;
// Interrupts per call
int __interruptCounts[5] = {8, 7, 8, 7, 8};
unsigned char __interruptIndex = 0;


void interrupt VectorNumber_Vrti IRQ_ISR(void);
void initIterrupt(int ms, void (*func) (void));
void (*__tempFunction)(void);
void startTimer(void);
void stopTimer(void);
void updateTimerPeriod(int ms);