#include <mps430.h>

void LedSetup();
void TimerSetup();

int count = 0;
int color = 0;

void main()
{
    WDTCTL = WDTPW | WDTHOLD;
    WDTCL = WDT_AD:Y_1000;
    LEDSetup();
    PM5CTL0 &= ~LOCKLPM5;
    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();
}

void LedSetup()
{
    P6DIR |= BIT0;
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;
    P6DIR |= BIT1;
    P6SEL0 |= BIT1;
    P6SEL1 &= ~BIT1;
    P6DIR |= BIT2;
    P6SEL0 |= BIT2;
    P6SEL1 &= ~BIT2;
}

void TimerB0Setup()
{
        TB3CCTL0 |= CCIE;
        TB3CCR0 = 0xFFFF;
        TB3CCTL1 = OUTMOD_7;
        TB3CCR1 = 800;
        TB3CTL = TBSSEL__SMCLK | ID_3 | MC__UP | TBCLR;

}

#pragma vector=TIMER3_B0VECTOR
__interrupt void TIMER3_B0_ISR(void)
{
   count ++;
    if (count == 2)
       {
       if (color == 0)
       {
            color = 1;
            P6DIR |= BIT1;
            P6SEL0 |= BIT1;
            P6SEL1 &= ~BIT0;
            count = 0;

            /
       }
       else if (color == 1)
       {
           P6DIR &= ~ BIT0;
           P6SEL0 |= BIT0;
           P6SEL1 &= ~BIT0;
            color = 2;
            count = 0;
        }
       else if (color == 2)
       {
           P6DIR |= BIT2;
           P6SEL0 |= BIT2;
           P6SEL1 &= ~BIT0;



           color = 3;
           count = 0;

       }
       else if (color == 3)
       {
           P6DIR &= ~ BIT1;
                                 P6SEL0 |= BIT0;
                                 P6SEL1 &= ~BIT0;                                                     color = 4;
                                 count = 0;
       }
       else if (color == 4)
       {
                    P6DIR |= BIT0;
                    P6SEL0 |= BIT0;
                    P6SEL1 &= ~BIT0;
                    color = 5;
                    count = 0;
       }
       else if (color ==5)
       {
                      P6DIR &= ~BIT2;
                      P6SEL0 |= BIT0;
                      P6SEL1 &= ~BIT0;
                       color = 0;
                       count = 0;
       }

        }

}
