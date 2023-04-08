#include <msp430.h> 

void LEDSetup();
void ButtonSetup();
void TimerB0Setup();
void TimerB1Setup();

void main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	LEDSetup(); // Initialize our LEDS
	ButtonSetup();  // Initialize our button
	TimerB0Setup(); // Initialize Timer0
	TimerB1Setup(); // Initialize Timer1
	 PM5CTL0 &= ~LOCKLPM5;

	__bis_SR_register(LPM0_bits + GIE);
	__no_operation();
}
void LEDSetup()
{
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P6DIR |= BIT6;
    P6OUT &= ~BIT6;
}
void ButtonSetup ()
{
    P4DIR &= ~BIT1;
    P4OUT |= BIT1;
    P4REN |= BIT1;
    P4IE  |= BIT1;
    P4IES &= ~BIT1;


    P2DIR &= ~BIT3;
    P2OUT |= BIT3;
    P2REN |= BIT3;
    P2IES &= ~BIT3;
    P2IE |= BIT3;


}
void TimerB0Setup()
{
    TB0CTL = TBSSEL__SMCLK | MC__UP | TBIE;
    TB0CCTL1 |= CCIE;
    TB0CCR0 = 1000;
    TB0CCR1 = 1;
}
void TimerB1Setup()
{
    TB1CTL = TBSSEL__SMCLK | MC__UP | TBIE;
    TB1CCTL1 |= CCIE;
    TB1CCR0 = 1000;
    TB1CCR1 = 1;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;
    if (TB0CCR1 >= 999)
        TB0CCR1 = 1;
    else
        TB0CCR1 += 100;
    }

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;
    if (TB1CCR1 >= 999)
        TB1CCR1 = 1;
    else
        TB1CCR1 += 100;

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt
        case TB0IV_TBCCR1:
            P1OUT &= ~BIT0;
            break;                               // CCR1 Set the pin to a 0
        case TB0IV_TBCCR2:
            break;                               // CCR2 not used
        case TB0IV_TBIFG:
            P1OUT |= BIT0;                       // overflow Set the pin to a 1
            break;
        default:
            break;
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B1_VECTOR))) TIMER1_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB1IV,TB1IV_TBIFG))
    {
        case TB1IV_NONE:
            break;                               // No interrupt
        case TB1IV_TBCCR1:
            P6OUT &= ~BIT6;
            break;                               // CCR1 Set the pin to a 0
        case TB1IV_TBCCR2:
            break;                               // CCR2 not used
        case TB1IV_TBIFG:
            P6OUT |= BIT6;                       // overflow Set the pin to a 1
            break;
        default:
            break;
    }
}

