//------------------------------------------------------------------------------
//
//  Description: This file contains the Serial Routine - Serial Processes
//
//
//  Ibrahim Moghul
//  Mar 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//
//  Yiyang Ye
//  July 2023
//  Built with IAR Embedded Workbench Version: (8.10.2)
//------------------------------------------------------------------------------
#include "serial.h"
#include "macros.h"
#include <string.h>
#include "motor.h"
#include "msp430.h"
#include "ports.h"

// global variables
volatile unsigned int usb0_rx_wr, usb1_rx_wr;
unsigned int usb0_rx_rd, usb1_rx_rd;
volatile char USB0_Char_Rx_Ring[SMALL_RING_SIZE], USB0_Char_Rx_Process[LARGE_RING_SIZE];
volatile char USB1_Char_Rx_Ring[SMALL_RING_SIZE], USB1_Char_Rx_Process[LARGE_RING_SIZE];
volatile char USB0_Char_Tx[LARGE_RING_SIZE], USB1_Char_Tx[LARGE_RING_SIZE];
unsigned volatile int pb0_index, pb1_index;
unsigned volatile int tx0_index, tx1_index;
unsigned volatile int pb0_buffered, pb1_buffered;
extern volatile unsigned char display_changed;
extern char display_line[4][11];
unsigned volatile int serialState;
volatile char receievedFromPC = OFF;
extern Motor motor1, motor2, motor3, motor4, motor5, motor6;
extern volatile long long encoderVal_1, encoderVal_2;//
extern unsigned int encoder_en_1, encoder_en_2;
extern unsigned int encoder_1_ready, encoder_2_ready;
unsigned long int data1,data2;//
extern volatile long long encoder1_v, encoder2_v;//
char status[41] = "S0000000000000000000000000000000000000000";
extern unsigned int laser_en;
extern unsigned int heater_en;
extern unsigned long int heaterCount;
unsigned long int heaterSet;
//----------------------------------------------------------------------------
void Init_Serial_UCA(void) {
    int i;

    for(i = 0; i < SMALL_RING_SIZE; i++) {
        USB0_Char_Rx_Ring[i] = 0x00;
        USB1_Char_Rx_Ring[i] = 0x00;
    }

    usb0_rx_wr = BEGINNING;
    usb0_rx_rd = BEGINNING;
    usb1_rx_wr = BEGINNING;
    usb1_rx_rd = BEGINNING;

    for(i = 0; i < LARGE_RING_SIZE; i++) {
        USB0_Char_Tx[i] = 0x00;
        USB1_Char_Tx[i] = 0x00;
    }

    // Configure UART 0
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0BRW = 8;
    UCA0MCTLW = 0xF7A1; // 112,500 rates
    UCA0CTLW0 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
    // Configure UART 1
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1BRW = 8;
    UCA1MCTLW = 0xF7A1;
    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE |= UCRXIE;
}
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void) {
    unsigned int temp;

    switch(__even_in_range(UCA0IV, 0x08)) {
        case 0:
            break;

        case 2: // RXIFG
            temp = usb0_rx_wr++;
            USB0_Char_Rx_Ring[temp] = UCA0RXBUF;

            if (usb0_rx_wr >= (SMALL_RING_SIZE)) {
                usb0_rx_wr = BEGINNING;
            }

            if(receievedFromPC) UCA1TXBUF = USB0_Char_Rx_Ring[temp];

            break;

        case 4: // TXIFG
            //if(receievedFromPC==OFF) {
            //  UCA0IE &= ~UCTXIE;
            //  return;
            //}
            UCA0TXBUF = USB0_Char_Tx[tx0_index];
            USB0_Char_Tx[tx0_index++] = 0;

            if(USB0_Char_Tx[tx0_index] == 0) {
                UCA0IE &= ~UCTXIE;
            }

            break;

        default:
            break;
    }
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
    unsigned int temp;

    switch(__even_in_range(UCA1IV, 0x08)) {
        case 0:
            break;

        case 2: // RXIFG
            if (!receievedFromPC)// && recieved=='\n')
                receievedFromPC = ON;

            temp = usb1_rx_wr++;
            USB1_Char_Rx_Ring[temp] = UCA1RXBUF;

            if(receievedFromPC)UCA0TXBUF = USB1_Char_Rx_Ring[temp];

            if (usb1_rx_wr >= (SMALL_RING_SIZE)) {
                usb1_rx_wr = BEGINNING;
            }

            break;

        case 4: // TXIFG
            if(receievedFromPC == OFF) {
                UCA1IE &= ~UCTXIE;
                return;
            }

            UCA1TXBUF = USB1_Char_Tx[tx1_index];
            USB1_Char_Tx[tx1_index++] = 0;

            if(USB1_Char_Tx[tx1_index] == 0) {
                UCA1IE &= ~UCTXIE;
            }

            break;

        default:
            break;
    }
}

void clearProcessBuff(volatile char* pb, volatile unsigned int* pb_index, volatile unsigned int* pb_buffered) {
    for(int i = 0; i < LARGE_RING_SIZE; ++i)pb[i] = 0;

    *pb_index = 0;
    *pb_buffered = 0;
}
void clearProcessBuff_0(void) {
    clearProcessBuff(USB0_Char_Rx_Process, &pb0_index, &pb0_buffered);
}
void clearProcessBuff_1(void) {
    clearProcessBuff(USB1_Char_Rx_Process, &pb1_index, &pb1_buffered);
}

/*void out_character(char character) {
    //------------------------------------------------------------------------------
    // The while loop will stall as long as the Flag is not set [port is busy]
    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?

    UCA0TXBUF = character;
    //------------------------------------------------------------------------------
}*/

void USCI_A0_transmit(void) {
    tx0_index = 0;
    UCA0IE |= UCTXIE;
}

void USCI_A1_transmit(void) {
    tx1_index = 0;
    UCA1IE |= UCTXIE;
}

void loadRingtoPB(volatile unsigned int* rx_wr, unsigned int* rx_rd, volatile char* Rx_Process, volatile char* Rx_Ring, volatile unsigned int* pb_index, volatile unsigned int* pb_buffered) {
    if(*pb_buffered) return;
  
    if(*rx_wr != *rx_rd) {
        Rx_Process[*pb_index] = Rx_Ring[*rx_rd];

        if(++(*rx_rd) > SMALL_RING_SIZE - 1) *rx_rd = BEGINNING;

        if(++(*pb_index) > LARGE_RING_SIZE - 1) *pb_index = BEGINNING;
    }

    if(*pb_index >= 2 && Rx_Process[(*pb_index) - 1] == '\n' && Rx_Process[(*pb_index) - 2] == '\r') {
        *pb_buffered = 1;
        *pb_index = BEGINNING;
    }
}

void loadRingtoPB_0(void) {
    loadRingtoPB(&usb0_rx_wr, &usb0_rx_rd, USB0_Char_Rx_Process, USB0_Char_Rx_Ring, &pb0_index, &pb0_buffered);
}
void loadRingtoPB_1(void) {
    loadRingtoPB(&usb1_rx_wr, &usb1_rx_rd, USB1_Char_Rx_Process, USB1_Char_Rx_Ring, &pb1_index, &pb1_buffered);
}


void SerialProcess(void) {
    /*if(!pb0_buffered)*/loadRingtoPB_0();
    /*if(!pb1_buffered)*/loadRingtoPB_1();
}

// Motor 1: 1 revs
// ex: 1 10
// Motor 2: 2 dir
// ex: 2 1
void HandleCommands(void) {
    if(pb1_buffered) {
//        int len = strlen((char*)USB1_Char_Rx_Process) - 2;
//        USB1_Char_Rx_Process[len] = 0;
//        USB1_Char_Rx_Process[len + 1] = 0;
//
//        Motor* motor;
//
//        if(USB1_Char_Rx_Process[0] == '1') {
//            motor = &motor1;
//        } else if (USB1_Char_Rx_Process[0] == '2') {
//            motor = &motor2;
//        } else if (USB1_Char_Rx_Process[0] == '3') {
//            motor = &motor3;
//        } else if (USB1_Char_Rx_Process[0] == '4') {
//            motor = &motor4;
//        } else if (USB1_Char_Rx_Process[0] == '5') {
//            motor = &motor5;
//        }
//
//        unsigned int dir = USB1_Char_Rx_Process[2] - '0';
//
//        // make sure trains have finished transmitting and this is not a halt command
//        if(!dir) {
//            motor->trainsToSend = 0;
//            motor->trainsDir = 0;
//            motor->pulseTrainsSent = 1;
//        } else if(motor->trainsFinished) {
//            motor->trainsFinished = 0;
//            motor->trainsToSend = stoi((char*)(USB1_Char_Rx_Process + 4), len - 4);
//            motor->trainsDir = dir;
//            motor->pulseTrainsSent = 1;
//        }
        performCommand(USB1_Char_Rx_Process, USB1_Char_Tx, USCI_A1_transmit);
        clearProcessBuff_1();
    }

    if(pb0_buffered) {
        performCommand(USB0_Char_Rx_Process, USB0_Char_Tx, USCI_A0_transmit);
        clearProcessBuff_0();
    }
}


void performCommand(volatile char* rx_process, volatile char * tx, void (*send)(void)){
  
  int len = strlen((char*)rx_process) - 2;
        rx_process[len] = 0;
        rx_process[len + 1] = 0;
        
        Motor* motor;
        volatile char temp1[17], temp2[17];
        unsigned int dir = 1;//default is clockwise
        
        if(rx_process[0] == 'E') { // if the first character of the input is an 'E' send the encoder value
            if (rx_process[1] == '1') { 
              encoder_en_1 = 1; // enable the encoder value storage
              while(!encoder_1_ready); // wait until encoder value is ready
              encoder_1_ready=0;
              itoa(encoder1_v,tx);
              int len = strlen((char*)tx);
              tx[0] = 'E';
              tx[len] = '\r';
              tx[len+1] = '\n';
              send();
              
            }
            if (rx_process[1] == '2') { 
              encoder_en_2 = 1; // enable the encoder value storage
              while(!encoder_2_ready); // wait until encoder value is ready
              encoder_2_ready=0;
              itoa(encoder2_v,tx);
              int len = strlen((char*)tx);
              tx[0] = 'E';
              tx[len] = '\r';
              tx[len+1] = '\n';
              send();
            }
            return;
        }
        
        else if (rx_process[0] == 'S' && rx_process[1] == 'R') {
          //send status:T1 T2 C1 C2 H L 0 0 en1(16bit) en2(16bit)
          encoder_en_1 = 1; // enable the encoder value storage
          while(!encoder_1_ready); // wait until encoder value is ready
          encoder_1_ready = 0;
          itoa(encoder1_v,temp1);
          int j = 9;
          for (int i=1; i<17; i++) status[j++] = temp1[i];//update the en1 16 bits
          encoder_en_2 = 1; // enable the encoder value storage
          while(!encoder_2_ready); // wait until encoder value is ready
          encoder_2_ready = 0;
          itoa(encoder2_v,temp2);
          j = 25;
          for (int i=1; i<17; i++) status[j++] = temp2[i];//update the en2 16 bits
          for (int i=0;i<41;i++) tx[i] = status[i];
          tx[41] = '\r';
          tx[42] = '\n';
          send();
          return;
        }
        // set which motor to control based on the first two characters
        // for the new protocal
        else if (rx_process[0] == 'C') {
          if (rx_process[1] == '1') {
            motor = &motor1;
            //store the desired location
            dir = 1;
            encoder_en_1 = 1; // enable the encoder value storage
            encoder_1_ready = 0;
            while(!encoder_1_ready); // wait until encoder value is ready
            encoder_1_ready = 0;
            data1 = 256*(unsigned int)(rx_process[2])+(unsigned int)(rx_process[3]);
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
          if (rx_process[1] == '2') {
            motor = &motor3;
            //store the desired location
            dir = 1;
            encoder_en_2 = 1; // enable the encoder value storage
            encoder_2_ready = 0;
            while(!encoder_2_ready); // wait until encoder value is ready
            encoder_2_ready = 0;
            data2 = 256*(unsigned int)(rx_process[2])+(unsigned int)(rx_process[3]);
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
        }
        
        else if (rx_process[0] == 'T') {
          if (rx_process[1] == '1') {
            motor = &motor2;
            if (rx_process[3] == 'F') {
              dir = 0;
              status[1] = '0';
            }
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
          if (rx_process[1] == '2') {
            motor = &motor4;
            if (rx_process[3] == 'F') {
              dir = 0;
              status[2] = '0';
            }
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
        }
        
        else if (rx_process[0] == 'L') {
          if (rx_process[2] == 'N') {
            laser_en = 1; //enable the laser
            P1OUT |= LASER; // turn on the laser pin
            //motor = &motor6;
            status[6] = '1';
            dir = 1;
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
          if (rx_process[2] == 'F') {
            laser_en = 0;
            P1OUT &= ~LASER;
            //motor = &motor6;
            status[6] = '0';
            dir = 0;
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
        }
        
        else if (rx_process[0] == 'h') {
          motor = &motor5;
          heater_en = 0;
          heaterCount = 0;
          heaterSet = 0;
          if (rx_process[1] == 'O') {
             status[5] = 0;
             dir = 0;
             tx[0] = 'O';
             tx[1] = 'K';
             tx[2] = '\r';
             tx[3] = '\n';
             send();
          }
        }
        else if (rx_process[0] == 'H') {
          motor = &motor5;
          heater_en = 1;
          heaterSet = 256*(unsigned int)(rx_process[1])+(unsigned int)(rx_process[2]);;
          tx[0] = 'O';
          tx[1] = 'K';
          tx[2] = '\r';
          tx[3] = '\n';
          send();
        }
        
        /* for the old protocal
        if(rx_process[0] == '1') {
            motor = &motor1;
        } else if (rx_process[0] == '2') {
            motor = &motor2;
        } else if (rx_process[0] == '3') {
            motor = &motor3;
        } else if (rx_process[0] == '4') {
            motor = &motor4;
        } else if (rx_process[0] == '5') {
            motor = &motor5;
        }
        */
        else if (rx_process[0] == 'c') {
          if (rx_process[1] == '1') {
            motor = &motor1;
            status[3] = '0';
            dir=0;
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
          if (rx_process[1] == '2') {
            motor = &motor3;
            status[4] = '0';
            dir=0;
            tx[0] = 'O';
            tx[1] = 'K';
            tx[2] = '\r';
            tx[3] = '\n';
            send();
          }
        } 
        
        // make sure trains have finished transmitting and this is not a halt command
        if(!dir) { // dir=0, which means to stop the motor
            motor->trainsToSend = 0;
            motor->trainsDir = 0;
            motor->pulseTrainsSent = 1;
        } else if(motor->trainsFinished) {
            motor->trainsFinished = 0;
            //motor->trainsToSend = stoi((char*)(rx_process + 4), len - 4);
            motor->trainsToSend=1;
            motor->trainsDir = dir;
            motor->pulseTrainsSent = 1;
        }
  
}

int stoi(char* str, int len) {
    int num = 0;
    
    for(int i = 0; i < len/* && str[i] >= '0' && str[i] <= '9'*/; ++i)
        num = num * 10 + (int)(str[i] - '0');
    
    return num;
}

//convert encoder value to 16 bit binary and send back
void itoa(volatile unsigned long long n, volatile char s[]) 
 {
     unsigned long long i;
     
     //if ((sign = n) < 0)  /* record sign */
         //n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */ 
       s[i++] = n % 2 + '0';   /* get next digit */
       n /= 2;  
     } while (i<16);     /* delete it */
     s[i] = '0';
     //s[i+1] = '\0';
     //if (sign < 0)
         //s[i++] = '-';
     //s[i] = '\0';
     reverse(s);
}  

 /* reverse:  reverse string s in place */
 void reverse(volatile char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen((char const*)s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  