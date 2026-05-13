#include <msp430.h>

void uart_sendChar(char character) {
  while(UCA3STAT & UCBUSY);                 // wait until TX free
  UCA3TXBUF = character;                    // send char
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P8DIR |= 0xFF;                            // P1.0 output
  P8OUT = 0xFF;
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 50000;
  TA1CTL = TASSEL_2 + MC_2 + TACLR;        // SMCLK, contmode, clear TAR

  P10SEL |= BIT4;
  P10DIR |= BIT4;
  UCA3CTL1 |= UCSWRST;
  UCA3CTL0 = 0;
  UCA3CTL1 |= UCSSEL_2;
  UCA3BRW = 104;
  UCA3MCTL = UCBRS_1;
  UCA3CTL1 &= ~UCSWRST;

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  static unsigned int count = 0;
  count++;
  TA1CCR0 += 50000;

  if (count >= 20) {                       // ~1 second
    count = 0;
    P8OUT ^= 0x01;                         // toggle LED
    uart_sendChar('X');                    // send char every second
    uart_sendChar('\n');
    uart_sendChar('\r');
  }
}
