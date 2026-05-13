#include <msp430.h>
#include <stdio.h>

void uart_sendChar(char character) {
  while(UCA3STAT & UCBUSY);
  UCA3TXBUF = character;
}

void usbUart_sendString(char * str) {
  while (*str != '\0') {                    // loop until end of string
    uart_sendChar(*str);
    str++;
  }
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
  static unsigned int sec = 0;
  static unsigned int min = 0;
  static unsigned int hour = 0;
  char buf[12];

  count++;
  TA1CCR0 += 50000;

  if (count >= 20) {                       // ~1 second
    count = 0;
    sec++;
    if (sec >= 60) { sec = 0; min++; }
    if (min >= 60) { min = 0; hour++; }

    sprintf(buf, "%02u:%02u:%02u", sec, min, hour); // format time
    usbUart_sendString(buf);
    usbUart_sendString("\n\r");
    P8OUT ^= 0x01;                         // toggle LED
  }
}
