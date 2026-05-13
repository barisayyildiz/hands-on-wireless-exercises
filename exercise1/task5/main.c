#include <msp430.h>                 // MSP430 register names

volatile unsigned char led_state = 1; // current LED value, starts at 1, updated in ISR

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;      // stop the watchdog timer

    P8DIR |= 0xFF;                  // make all P8 pins outputs
    P8OUT = ~led_state;             // show led_state on LEDs

    P2DIR &= ~0x40;                 // make P2.6 an input pin

    P2IES |= 0x40;                 // trigger interrupt on falling
    P2IFG &= ~(0x40);              // clear interrupt flags before enabling
    P2IE  |= 0x40;                  // enable interrupts for P2.6

    __bis_SR_register(GIE);         // enable global interrupts

    while (1)                       // loop forever
    {
        P8OUT = ~led_state;         // update LEDs
        __delay_cycles(10000);    // wait about 1 second
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    unsigned short vector = P2IV;
    switch (vector)
    {
    case 0x0E:                      // button S0 was pressed
        led_state++;                // increment the counter
        break;
    default:                        // unexpected interrupt, do nothing
        break;
    }
}
