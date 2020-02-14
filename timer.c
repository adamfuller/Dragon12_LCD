#include "timer.h"

void initIterrupt(int ms, void (*func) (void)){
    stopTimer();
    asm(" MOVB #$10, RTICTL"); // RTI calls every 1024 clock cycles
    asm(" BSET CRGINT,$80");   // Set timer call flag
    __tempFunction = func;
    __callEverymS = ms;
    startTimer();
}

void updateTimerPeriod(int ms){
    __callEverymS = ms;
}

void startTimer(void){
    asm("   CLI");
}

void stopTimer(void){
    asm("   SEI");
}

void interrupt VectorNumber_Vrti IRQ_ISR(void){
    // CRGFLG = 0x80; // Reset timer call flag
    asm("   BSET CRGFLG,$80");

    // Call the function if the tick count is how many are needed
    __interruptCount++;

    if (__interruptCount == __interruptCounts[__interruptIndex]){
        // Increment the milliseconds count
        __msCount++;
        // Increment the call count
        __tickCount++;
        // Reset the interrupt count
        __interruptCount = 0;

        // If we hit the ms needed to call reset and call
        if ( !(__msCount %= __callEverymS) ){
            
            // Call the function if it isn't null
            if (__tempFunction){
                (*__tempFunction)();
            }
        }

        // Switch between high and low ticks when needed
        // Also resets the tickCount in the process
        if ( !(__tickCount %= __ticksToSwitch) ){
            // Tick up the index for the interrupt count per ms
            if (__tickCount == __callCounts[__callIndex]){
                __interruptIndex = (__interruptIndex + 1) % __callLength;
                __callIndex = (__callIndex + 1) % __callLength;
                // Change how many times the interrupt 
                // is called before ticking up the count
                __ticksToSwitch = __callCounts[__callIndex];
            }
        }
    }
}