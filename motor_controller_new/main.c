#include "msp430.h"
#include "functions.h"
#include "motor.h"
#include "ports.h"
#include "timers.h"
#include "serial.h"
#include "encoders.h"
/**
 * main.c
 */

extern Motor motor1, motor2, motor3, motor4, motor5, motor6;

int main(void) {
    PM5CTL0 &= ~LOCKLPM5;
    
    Init_Ports();
    Init_Clocks();
    enable_interrupts();

    Init_Serial_UCA();
    Init_Timers();

    while(1) {
        SerialProcess(); // load ring to process buffer
        HandleCommands(); // handle the serial commands
        // run the process for all the motors
        sendTrains(&motor1);
        sendTrains(&motor2);
        sendTrains(&motor3);
        sendTrains(&motor4);
        sendTrains(&motor5);
        //sendTrains(&motor6);
    }

    return 0;
}
