#include <Timer1.h>

void timer1InterruptHandler() {
    // Do something on interrupt
}

int main() {
    // Most timers has API as below, now covered timers 0...5

    // Set clock source
    Timer1.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);

    // Set count mode
    Timer1.setCountMode(TIMER_16BIT_COUNT_NORMAL);

    // Set output mode for channels A/B/C
    Timer1.setOutputModeA(TIMER_OUTPUT_TOGGLE);

    // Set interrupt handler as pointer to function
    Timer1.setInterruptHandler(TIMER_INTERRUPT_OVERFLOW, timer1InterruptHandler);

    // Set interrupt handler as lambda function
    Timer1.setInterruptHandler(TIMER_INTERRUPT_COMPARE_MATCH_A, [](){
        // Do something on compare match
    });

    // Set initial counter value
    *Timer1.TCNTn = 0x0F;

    // Set compare match value
    *Timer1.OCRnA = 0xF0;

    // Enable interrupts
    Timer1.setInterruptEnabled(TIMER_INTERRUPT_OVERFLOW, true);
    Timer1.setInterruptEnabled(TIMER_INTERRUPT_COMPARE_MATCH_A, true);
}
