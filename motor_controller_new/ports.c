//==============================================================================
// File Name : ports.c
//
// Description: This file contains the Initialization for all port pins

//==============================================================================
#include "ports.h"
#include "msp430.h"

//===========================================================================

//===========================================================================
void Init_Ports(void) {
    Init_Port1();
    Init_Port2();
    Init_Port3(USE_GPIO);
    Init_Port4();
    Init_Port5();
    Init_Port6();
}

//===========================================================================

//===========================================================================
void Init_Port1() {
    P1OUT = 0x00;
    P1DIR = 0x00;

    P1SEL0 &= ~RED_LED;
    P1SEL1 &= ~RED_LED;
    P1DIR |= RED_LED;
    P1OUT &= ~RED_LED;

    P1SEL0 &= ~LASER;
    P1SEL1 &= ~LASER;
    P1DIR |= LASER; // direction to output
    P1OUT &= ~LASER; // initial value is low
    
    P1SEL0 &= ~ENCODER_CLK1;
    P1SEL1 &= ~ENCODER_CLK1;
    P1DIR |= ENCODER_CLK1; // direction to output
    P1OUT |= ENCODER_CLK1; // initial value is high

    P1SEL0 &= ~ENCODER_DATA1;
    P1SEL1 &= ~ENCODER_DATA1;
    P1DIR &= ~ENCODER_DATA1;

    P1SEL0 &= ~ENCODER_CLK2;
    P1SEL1 &= ~ENCODER_CLK2;
    //P1DIR &= ~ENCODER_CLK2;
    P1DIR |= ENCODER_CLK2; // direction to output
    P1OUT &= ~ENCODER_CLK2; // initial value is low
    
    P1SEL0 &= ~ENCODER_CLK3;
    P1SEL1 &= ~ENCODER_CLK3;
    P1DIR |= ENCODER_CLK3; // direction to output
    P1OUT |= ENCODER_CLK3; // initial value is high

    P1SEL1 &= ~UCA0RXD;
    P1SEL0 |= UCA0RXD;

    P1SEL1 &= ~UCA0TXD;
    P1SEL0 |= UCA0TXD;
}

//===========================================================================

//===========================================================================
void Init_Port2() {
    P2OUT = 0x00;
    P2DIR = 0x00;
    
    P2SEL0 &= ~MOTOR1_ENABLE_P;
    P2SEL1 &= ~MOTOR1_ENABLE_P;
    P2DIR |= MOTOR1_ENABLE_P;
    P2OUT &= ~MOTOR1_ENABLE_P;
    
    P2SEL0 &= ~MOTOR1_DIR_P;
    P2SEL1 &= ~MOTOR1_DIR_P;
    P2DIR |= MOTOR1_DIR_P;
    P2OUT |= MOTOR1_DIR_P;

    P2SEL0 &= ~ENCODER_CLK4;
    P2SEL1 &= ~ENCODER_CLK4;
    P2DIR |= ENCODER_CLK4; // direction to output
    P2OUT &= ~ENCODER_CLK4; // initial value is low

    P2SEL0 &= ~SW2;
    P2SEL1 &= ~SW2;
    P2DIR &= ~SW2;

    P2SEL0 &= ~MOTOR2_ENABLE_P;
    P2SEL1 &= ~MOTOR2_ENABLE_P;
    P2DIR |= MOTOR2_ENABLE_P;
    P2OUT &= ~MOTOR2_ENABLE_P;

    P2SEL0 &= ~MOTOR2_DIR_P;
    P2SEL1 &= ~MOTOR2_DIR_P;
    P2DIR |= MOTOR2_DIR_P;
    P2OUT |= MOTOR2_DIR_P;

    P2SEL0 &= ~LFXOUT;
    P2SEL1 |= LFXOUT;

    P2SEL0 &= ~LFXIN;
    P2SEL1 |= LFXIN;
}

//===========================================================================

//===========================================================================
void Init_Port3(char smclk) {
    P3OUT = 0x00;
    P3DIR = 0x00;

    P3SEL0 &= ~ENCODER_DATA2;
    P3SEL1 &= ~ENCODER_DATA2;
    P3DIR &= ~ENCODER_DATA2;

    P3SEL0 &= ~MOTOR5_P_COUNT;
    P3SEL1 &= ~MOTOR5_P_COUNT;
    P3OUT |= MOTOR5_P_COUNT;
    P3DIR &= ~MOTOR5_P_COUNT;
    P3REN |= MOTOR5_P_COUNT;
    P3IES |= MOTOR5_P_COUNT;
    P3IFG &= ~MOTOR5_P_COUNT; //reset interrupt flag
    P3IE |= MOTOR5_P_COUNT; //enable interrupt
    
    P3SEL0 &= ~P3_2;
    P3SEL1 &= ~P3_2;
    
    P3SEL0 &= ~MOTOR4_P_COUNT; 
    P3SEL1 &= ~MOTOR4_P_COUNT; 
    //P3DIR |= MOTOR4_P_COUNT;
    //P3OUT &= ~MOTOR4_P_COUNT;
    P3OUT |= MOTOR4_P_COUNT;
    P3DIR &= ~MOTOR4_P_COUNT;
    P3REN |= MOTOR4_P_COUNT;
    P3IES |= MOTOR4_P_COUNT;
    P3IFG &= ~MOTOR4_P_COUNT;
    P3IE |= MOTOR4_P_COUNT;
    
    P3SEL0 &= ~MOTOR3_P_COUNT;
    P3SEL1 &= ~MOTOR3_P_COUNT;
    //P3OUT |= MOTOR3_P_COUNT;
    //P3DIR |= MOTOR3_P_COUNT;
    P3OUT |= MOTOR3_P_COUNT;
    P3DIR &= ~MOTOR3_P_COUNT;
    P3REN |= MOTOR3_P_COUNT;
    P3IES |= MOTOR3_P_COUNT;
    P3IFG &= ~MOTOR3_P_COUNT;
    P3IE |= MOTOR3_P_COUNT;
    
    P3SEL0 |= DAC_CNTL;
    P3SEL1 |= DAC_CNTL;
    //P3DIR &= ~DAC_CNTL;

    P3SEL0 &= ~MOTOR1_P_COUNT;
    P3SEL1 &= ~MOTOR1_P_COUNT;
    P3OUT |= MOTOR1_P_COUNT;
    P3DIR &= ~MOTOR1_P_COUNT;
    P3REN |= MOTOR1_P_COUNT;
    P3IES |= MOTOR1_P_COUNT;//
    P3IFG &= ~MOTOR1_P_COUNT;
    P3IE |= MOTOR1_P_COUNT;

    P3SEL0 &= ~MOTOR2_P_COUNT;
    P3SEL1 &= ~MOTOR2_P_COUNT;
    P3OUT |= MOTOR2_P_COUNT;
    P3DIR &= ~MOTOR2_P_COUNT;
    P3REN |= MOTOR2_P_COUNT;
    P3IES |= MOTOR2_P_COUNT;//
    P3IFG &= ~MOTOR2_P_COUNT;
    P3IE |= MOTOR2_P_COUNT;
}

//===========================================================================

//===========================================================================
void Init_Port4() {
    P4OUT = 0x00;
    P4DIR = 0x00;

    P4SEL0 &= ~MOTOR5_ENABLE_P;
    P4SEL1 &= ~MOTOR5_ENABLE_P;
    P4DIR |= MOTOR5_ENABLE_P;
    P4OUT &= ~MOTOR5_ENABLE_P;

    P4SEL0 &= ~SW1;
    P4SEL1 &= ~SW1;
    P4DIR &= ~SW1;


    P4SEL0 |= UCA1TXD;
    P4SEL1 &= ~UCA1TXD;

    P4SEL0 |= UCA1RXD;
    P4SEL1 &= ~UCA1RXD;

    P4SEL0 &= ~LED1;
    P4SEL1 &= ~LED1;
    P4OUT &= ~LED1;
    //P4OUT |= LED1;
    P4DIR |= LED1;

    P4SEL0 |= UCB1CLK;
    P4SEL1 &= ~UCB1CLK;

    P4SEL0 &= ~LED2;
    P4SEL1 &= ~LED2;
    P4OUT &= ~LED2;
    //P4OUT |= LED2;
    P4DIR |= LED2;

    P4SEL0 &= ~LED3;
    P4SEL1 &= ~LED3;
    P4OUT &= ~LED3;
    //P4OUT |= LED3;
    P4DIR |= LED3;
    
}

//===========================================================================

//===========================================================================
void Init_Port5() {
    P5OUT = 0x00;
    P5DIR = 0x00;

    P5SEL0 &= ~MOTOR3_ENABLE_P;
    P5SEL1 &= ~MOTOR3_ENABLE_P;
    P5DIR |= MOTOR3_ENABLE_P;
    P5OUT &= ~MOTOR3_ENABLE_P;

    P5SEL0 &= ~MOTOR3_DIR_P;
    P5SEL1 &= ~MOTOR3_DIR_P;
    P5DIR |= MOTOR3_DIR_P;
    P5OUT |= MOTOR3_DIR_P;

    P5SEL0 &= ~MOTOR4_ENABLE_P;
    P5SEL1 &= ~MOTOR4_ENABLE_P;
    P5DIR |= MOTOR4_ENABLE_P;
    P5OUT &= ~MOTOR4_ENABLE_P;

    P5SEL0 &= ~MOTOR4_DIR_P;
    P5SEL1 &= ~MOTOR4_DIR_P;
    P5DIR |= MOTOR4_DIR_P;
    P5OUT |= MOTOR4_DIR_P;

    P5SEL0 &= ~MOTOR5_DIR_P;
    P5SEL1 &= ~MOTOR5_DIR_P;
    P5DIR |= MOTOR5_DIR_P;
    P5OUT |= MOTOR5_DIR_P;

}

//===========================================================================

//===========================================================================
void Init_Port6() {
    P6OUT = 0x00;
    P6DIR = 0x00;

    P6SEL0 |= MOTOR1_PWM_PIN;
    P6SEL1 &= ~MOTOR1_PWM_PIN;
    P6DIR |= MOTOR1_PWM_PIN;

    P6SEL0 |= MOTOR2_PWM_PIN;
    P6SEL1 &= ~MOTOR2_PWM_PIN;
    P6DIR |= MOTOR2_PWM_PIN;

    P6SEL0 |= MOTOR3_PWM_PIN;
    P6SEL1 &= ~MOTOR3_PWM_PIN;
    P6DIR |= MOTOR3_PWM_PIN;

    P6SEL0 |= MOTOR4_PWM_PIN;
    P6SEL1 &= ~MOTOR4_PWM_PIN;
    P6DIR |= MOTOR4_PWM_PIN;

    P6SEL0 |= MOTOR5_PWM_PIN;
    P6SEL1 &= ~MOTOR5_PWM_PIN;
    P6DIR |= MOTOR5_PWM_PIN;

    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6DIR &= ~P6_5;

    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6DIR |= GRN_LED;
    P6OUT &= ~GRN_LED;
}