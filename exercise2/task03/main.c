#include <msp430.h>

void uart_sendChar(char character) {
  while(UCA3STAT & UCBUSY);
  UCA3TXBUF = character;
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P8DIR |= 0xFF;                            // P1.0 output
  P8OUT = 0xFF;
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 50000;
  TA1CTL = TASSEL_2 + MC_2 + TACLR;         // SMCLK, contmode, clear TAR


  P10SEL |= BIT4;
  P10DIR |= BIT4;
  UCA3CTL1 |= UCSWRST; //Reset State
  UCA3CTL0 = 0; /* no parity, 8 bit, LSB first, 1 stop bit, asynchronous UART */
  UCA3CTL1 |= UCSSEL_2;
  UCA3BRW = 104;
  UCA3MCTL = UCBRS_1;
  UCA3CTL1 &= ~UCSWRST; //release reset

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  static count = 0;
  count++;

  if(count >= 20) {
    uart_sendChar('0' + (255 - total)); // Send digit
    uart_sendChar('\n');                // Send New Line
    P8OUT = total--;
    count = 0;
  }
}

