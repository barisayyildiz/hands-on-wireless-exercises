#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;      // stop the watchdog timer

    P8DIR |= 0x01;                  // make P8.0 an output pin
    P8OUT |= 0x01;                  // LED0 off at start
    
    P2DIR &= ~0x40;                 // make P2.6 an input pin

    while (1)                       // check the button forever
    {
        if (!(P2IN & 0x40))         // button is being pressed
            P8OUT &= 0x00;         // turn LED0 on
        else                        // button is not pressed
            P8OUT |= 0x01;          // turn LED0 off
    }
}

