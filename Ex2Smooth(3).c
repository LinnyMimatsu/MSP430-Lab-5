
#include <msp430.h>

int main(void)
{
  // Stop watchdog timer
  WDTCTL = WDTPW + WDTHOLD;

  // For PWM, use Timer0  
  // Using SMCLK with /8 divisor, and 300 Hz servo frequency -> ~0.0033 second period
  // Set Timer0 compare register 0 value for 300 Hz
  TA0CCR0 = 0.0033 * 1000000 / 8;
  
  // Initially set PWM target to 20%
  TA0CCR1 = 0.2 * 0.0033 * 1000000 / 8;

  // Set output mode 6 for Timer0 capture/compare register 1
  TA0CCTL1 |= OUTMOD2 | OUTMOD1;

  // Set Timer0 clock source (TASSEL1), clock divider (ID0/ID1), and Up Mode (MC0)
  TA0CTL |= TASSEL1 | ID1 | ID0 | MC0;

  // Set P2.6 appropriately (see datasheet)
  P2DIR |= BIT6;
  P2SEL |= BIT6;
  P2SEL2 &= ~BIT6;

  P1DIR &= ~BIT3;              // Input
  P1IES |= BIT3;               // Interrupt on high->low (button press)
  P1IE  |= BIT3;               // Enable interrupt

  // Set P2.7 appropriately (see datasheet)
  P2SEL &= ~BIT7;
  P2SEL2 &= ~BIT7;

  // Enable interrupts
  __bis_SR_register(GIE);


}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {

  
// Toggle direction each time button pressed

if(P1IES & BIT3) {

TA0CCR1 = 0.8 * 0.0033 * 1000000 / 8;

P1IES &= ~BIT3;

} else {

TA0CCR1 = 0.2 * 0.0033 * 1000000 / 8;

P1IES |= BIT3;


}

P1IFG = 0;

}

