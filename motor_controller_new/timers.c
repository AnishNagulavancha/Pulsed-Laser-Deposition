#include "msp430.h"
#include "timers.h"
#include "ports.h"
#include "macros.h"


void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B3();
    //Init_Timer_B1();
}

void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__2; // Divide clock by 2
  
    
    TB0EX0 = TBIDEX__2; // Divide clock by an additional 2
    
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt
    
    TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    //TB0CCTL1 |= CCIE; // CCR1 enable interrupt
    
    TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
    //TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    
    TB0CTL &= ~TBIE & ~TBIFG; // Disable Overflow Interrupt
    //TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
/*
void Init_Timer_B1(void) {
    TB1CTL = TBSSEL__SMCLK; // SMCLK source
    TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB1CTL |= MC__UP; //  up
    TB1CTL |= ID__1; // Divide clock by 1
    
    
    TB1EX0 = TBIDEX__1; // Divide clock by an additional 1

    TB1CCR0 = PULSE_PERIOD_400K; //pwm period = 40
    
    TB1CCTL1 = OUTMOD_7;
    MOTOR6_P = 0; //tb1ccr1 = 0 
    
    //TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
    //TB1CCTL0 |= CCIE; // CCR0 enable interrupt
}
*/
void Init_Timer_B3(void) {
    TB3CTL = TBSSEL__SMCLK;//
    TB3CTL |= MC__UP;
    TB3CTL |= TBCLR;
    TB3CTL |= ID__8; // Divide clock by 8
    
    TB3EX0 = TBIDEX__1; // Divide clock by an additional 1
    
    TB3CCR0 = PULSE_PERIOD_SLOW; //pwm period = 20000
    
    TB3CCTL1 = OUTMOD_7;
    MOTOR1_P = 0;

    TB3CCTL2 = OUTMOD_7;
    MOTOR2_P = 0;

    TB3CCTL3 = OUTMOD_7;
    MOTOR3_P = 0;

    TB3CCTL4 = OUTMOD_7;
    MOTOR4_P = 0;

    TB3CCTL5 = OUTMOD_7;
    MOTOR5_P = 0;
    
}




