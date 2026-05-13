#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // stop the watchdog timer
  P8DIR |= 0xFF;                            // make all P8 pins output
  P8OUT = 0xFF;                             // all LEDs off at start

  unsigned int i = 0;                       // index of the current LED

  while (1)
  {
      P8OUT = ~(1 << i);                    // turn on only LED i
      i++;                                  // move to the next LED
      if(i > 7) {                           // if we passed the last LED, go back to the first
          i = 0;
      }
      __delay_cycles(1000000);              // wait about 1 second
  }
}
