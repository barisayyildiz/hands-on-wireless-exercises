#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop the watchdog timer
  P8DIR |= 0x01;                            // make P8.0 an output pin

  while (1)                                 // loop forever
  {
      P8OUT ^= 0x01;                        // flip P8.0 between 0 and 1 using XOR
      __delay_cycles(1000000);              // wait about 1 second
  }
}
