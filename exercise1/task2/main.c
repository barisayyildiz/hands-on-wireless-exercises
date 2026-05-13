#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // stop the watchdog timer
  P8DIR |= 0xFF;                            // make all P8 pins outputs

  int counter = 0xFF;                       // start counter at 255
  P8OUT = counter;

  while (1)
  {
      P8OUT = counter;                      // write current counter value to LEDs
      counter--;                            // decrement counter by 1
      if(counter < 0) {                     // if counter went below 0, wrap back to 255
          counter = 0xFF;
      }
      __delay_cycles(1000000);              // wait about 1 second
  }
}
