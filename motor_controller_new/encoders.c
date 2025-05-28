//==============================================================================
// File Name : encorder.c
//
// Description: This file contains codes for the encoder
//
// Author: Yiyang Ye
// Date: May 2023
// Compiler: Built with IAR Embedded Workbench Version: 8.10.2
//==============================================================================

#include "msp430.h"
#include "encoders.h"
#include "ports.h"
#include "timers.h"
#include "encoders.h"

volatile long long encoderVal_1, encoderVal_2;//
volatile long long encoder1_v, encoder2_v;//
extern long data1,data2;//
volatile unsigned long long encoderValBuffer;
char encoderCounter, encoderRisingEdge;
unsigned int count1, count2;
unsigned int encoder_en_2;//
unsigned int encoder_en_1;//
unsigned int encoder_1_ready, encoder_2_ready;

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) {
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    
    // for encoder 1:
    if (encoder_en_1) {
      P1OUT ^= (ENCODER_CLK1+ENCODER_CLK2); // Output clocks to the encoder
      encoderRisingEdge = !encoderRisingEdge; // generate a clock to capture bits from encoder
      if (count1++<=encoder_count1) { 
        if(!encoderRisingEdge){ // store values at edge    
          encoderValBuffer = encoderValBuffer<<1;
          if ((P1IN&ENCODER_DATA1)==8) {
            encoderValBuffer+=1; // if input is one, add 1
          }
          if(++encoderCounter>=18){
            encoderCounter = 0;
            encoderVal_1 = encoderValBuffer & 0x03FFFF; // capture the last 18 bits
            encoder1_v = encoderVal_1>>2; // capture the first 16 bits
          }
        }
      }
      else {
        count1 =0;
        encoderRisingEdge=0;
        encoderValBuffer=0;
        encoder_en_1 = 0;
        encoder_1_ready = 1;
      }
    }
    // for encoder 2:
    if (encoder_en_2) {
      P1OUT ^= ENCODER_CLK3; // Output clocks to the encoder
      P2OUT ^= ENCODER_CLK4;
      encoderRisingEdge = !encoderRisingEdge; // generate a clock to capture bits from encoder
      if (count2++<=encoder_count1) { 
        if(!encoderRisingEdge){ // store values at edge    
          encoderValBuffer = encoderValBuffer<<1;
          if ((P3IN&ENCODER_DATA2)==1) {
            encoderValBuffer+=1; // if input is one, add 1
          }
          if(++encoderCounter>=18){
            encoderCounter = 0;
            encoderVal_2 = encoderValBuffer & 0x03FFFF; // capture the last 18 bits
            encoder2_v = encoderVal_2>>2; // capture the first 16 bits
          }
        }
      }
      else {
        count2 =0;
        encoderRisingEdge=0;
        encoderValBuffer=0;
        encoder_en_2 = 0;
        encoder_2_ready = 1;
      }
    }
    //if (laser_en) {
      
    //}
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TB0CCR0
    //----------------------------------------------------------------------------
}
/*
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void) {
 // switch(__even_in_range(TB0IV,14)) {  
   // case 2: // CCR1
      if (laser_en) {
        P1OUT ^= LASER;
      }
      else P1OUT &= ~LASER; // SET TO LOW
      TB1CCR0 += TB1CCR0_INTERVAL; // Add Offset to TB0CCR1 
      //break;
 // }
}
*/









