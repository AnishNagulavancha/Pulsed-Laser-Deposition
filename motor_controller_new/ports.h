//==============================================================================
// File Name : ports.h
//
// Description: This file contains macros and function declarations for ports.c
//
// Author: Ibrahim Moghul
// Date: Feb 2022
// Compiler: Built with IAR Embedded Workbench Version: 7.21.1
//==============================================================================

#define USE_GPIO (0)
#define USE_SMCLK (1)

// Port 1 Pins
#define RED_LED (0x01)  // 0 
#define LASER (0x02) // 1 
#define ENCODER_CLK1 (0x04)     // 2 
#define ENCODER_DATA1 (0x08)     // 3 
#define ENCODER_CLK2 (0x10)     // 4 
#define ENCODER_CLK3 (0x20)     // 5 
#define UCA0RXD (0x40)  // 6 Back Channel UCA0RXD
#define UCA0TXD (0x80)  // 7 Back Channel UCA0TXD
// Port 2 Pins
#define MOTOR1_ENABLE_P (0x01)   // 0 
#define MOTOR1_DIR_P (0x02)   // 1 
#define ENCODER_CLK4 (0x04)   // 2
#define SW2 (0x08)    // 3 SW2
#define MOTOR2_ENABLE_P (0x10)   // 4 
#define MOTOR2_DIR_P (0x20)   // 5 
#define LFXOUT (0x40) // 6 XOUTR
#define LFXIN (0x80)  // 7 XINR
// Port 3 Pins
#define ENCODER_DATA2 (0x01) // 0 
#define MOTOR5_P_COUNT (0x02)       // 1 
#define P3_2 (0x04)       // 2
#define MOTOR4_P_COUNT (0x08)       // 3
#define MOTOR3_P_COUNT (0x10) // 4
#define DAC_CNTL (0x20)       // 5
#define MOTOR1_P_COUNT (0x40)       // 6
#define MOTOR2_P_COUNT (0x80)       // 7
// Port 4 Pins
#define MOTOR5_ENABLE_P (0x01)        // 0
#define SW1 (0x02)         // 1 SW1
#define UCA1RXD (0x04)     // 2 Back Channel UCA1RXD 
#define UCA1TXD (0x08)     // 3 Back Channel UCA1TXD
#define LED1 (0x10) // 4 LED1
#define UCB1CLK (0x20)     // 5 SPI mode -
#define LED2 (0x40)    // 6 LED2
#define LED3 (0x80)    // 7 LED3
// Port 5 Pins
#define MOTOR3_ENABLE_P (0x01)
#define MOTOR3_DIR_P (0x02)
#define MOTOR4_ENABLE_P (0x04)
#define MOTOR4_DIR_P (0x08)
#define MOTOR5_DIR_P (0x10)
// Port 6 Pins
#define MOTOR1_PWM_PIN (0x01)   // 0
#define MOTOR2_PWM_PIN (0x02)   // 1
#define MOTOR3_PWM_PIN (0x04)   // 2
#define MOTOR4_PWM_PIN (0x08)   // 3
#define MOTOR5_PWM_PIN (0x10)   // 4
#define P6_5 (0x20)    // 5   (NEED A FLY WIRE TO USE)
#define GRN_LED (0x40) // 6

// shortcuts

#define ENABLE1_P_ON {P2OUT |= MOTOR1_ENABLE_P;}
#define ENABLE1_P_OFF {P2OUT &= ~MOTOR1_ENABLE_P;}
#define DIR1_P_ON {P2OUT |= MOTOR1_DIR_P;}
#define DIR1_P_OFF {P2OUT &= ~MOTOR1_DIR_P;}

#define ENABLE2_P_ON {P2OUT |= MOTOR2_ENABLE_P;}
#define ENABLE2_P_OFF {P2OUT &= ~MOTOR2_ENABLE_P;}
#define DIR2_P_ON {P2OUT |= MOTOR2_DIR_P;}
#define DIR2_P_OFF {P2OUT &= ~MOTOR2_DIR_P;}

#define ENABLE3_P_ON {P5OUT |= MOTOR3_ENABLE_P;}
#define ENABLE3_P_OFF {P5OUT &= ~MOTOR3_ENABLE_P;}
#define DIR3_P_ON {P5OUT |= MOTOR3_DIR_P;}
#define DIR3_P_OFF {P5OUT &= ~MOTOR3_DIR_P;}

#define ENABLE4_P_ON {P5OUT |= MOTOR4_ENABLE_P;}
#define ENABLE4_P_OFF {P5OUT &= ~MOTOR4_ENABLE_P;}
#define DIR4_P_ON {P5OUT |= MOTOR4_DIR_P;}
#define DIR4_P_OFF {P5OUT &= ~MOTOR4_DIR_P;}

#define ENABLE5_P_ON {P4OUT |= MOTOR5_ENABLE_P;}
#define ENABLE5_P_OFF {P4OUT &= ~MOTOR5_ENABLE_P;}
#define DIR5_P_ON {P5OUT |= MOTOR5_DIR_P;}
#define DIR5_P_OFF {P5OUT &= ~MOTOR5_DIR_P;}

void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(char smclk);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);