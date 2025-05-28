#include "motor.h"
#include "msp430.h"
#include "timers.h"
#include "ports.h"

extern unsigned long int data1,data2;
extern volatile long long encoder1_v, encoder2_v;
extern unsigned int encoder_en_2;//
extern unsigned int encoder_en_1;//
extern unsigned int encoder_1_ready, encoder_2_ready;
unsigned int finish;
extern char status[];
unsigned int fast;
unsigned int laser_en;
unsigned long int heaterCount;
extern unsigned long int heaterSet;
unsigned int heaterFinish;
unsigned int heater_en;

Motor motor1 = {
    .PWM = &MOTOR1_P,
    .pulseCount = 0,
    .pulseTrainsSent = 0,
    .pulsesToSend = 0,
    .trainsToSend = 0,
    .trainsDir = 0,
    .trainsSent = 0,
    .trainsFinished = 0,
    .setMotor = setMotor1
};
Motor motor2 = {
    .PWM = &MOTOR2_P,
    .pulseCount = 0,
    .pulseTrainsSent = 0,
    .pulsesToSend = 0,
    .trainsToSend = 0,
    .trainsDir = 0,
    .trainsSent = 0,
    .trainsFinished = 0,
    .setMotor = setMotor2
};
Motor motor3 = {
    .PWM = &MOTOR3_P,
    .pulseCount = 0,
    .pulseTrainsSent = 0,
    .pulsesToSend = 0,
    .trainsToSend = 0,
    .trainsDir = 0,
    .trainsSent = 0,
    .trainsFinished = 0,
    .setMotor = setMotor3
};
Motor motor4 = {
    .PWM = &MOTOR4_P,
    .pulseCount = 0,
    .pulseTrainsSent = 0,
    .pulsesToSend = 0,
    .trainsToSend = 0,
    .trainsDir = 0,
    .trainsSent = 0,
    .trainsFinished = 0,
    .setMotor = setMotor4
};

Motor motor5 = {
    .PWM = &MOTOR5_P,
    .pulseCount = 0,
    .pulseTrainsSent = 0,
    .pulsesToSend = 0,
    .trainsToSend = 0,
    .trainsDir = 0,
    .trainsSent = 0,
    .trainsFinished = 0,
    .setMotor = setMotor5
};
/*
Motor motor6 = {
    .PWM = &MOTOR6_P,
    .pulseCount = 0,
    .pulseTrainsSent = 0,
    .pulsesToSend = 0,
    .trainsToSend = 0,
    .trainsDir = 0,
    .trainsSent = 0,
    .trainsFinished = 0,
    .setMotor = setMotor6
};
*/

int sendTrains(Motor* motor) {
    
  //if (motor == &motor1) {
  
  //}
    if(motor->pulseTrainsSent && motor->trainsToSend) { 
        motor->trainsFinished = 0;
        //
        if (motor == &motor3) {
          encoder_en_2 = 1; // enable the encoder value storage
          while(!encoder_2_ready); // wait until encoder value is ready
          if ((signed long)(encoder2_v-data2)<-2048) {
            status[4] = '1';
            TB3CCR0 = PULSE_PERIOD_FAST;
            fast = 1;
            motor->trainsDir = 2; 
          }
          else if ((signed long)(encoder2_v-data2)>2048) {
            status[4] = '1';
            TB3CCR0 = PULSE_PERIOD_FAST;
            fast = 1;
            motor->trainsDir = 1;
          }
          else if ((signed long)(encoder2_v-data2)<-64) {
            status[4] = '1';
            TB3CCR0 = PULSE_PERIOD_SLOW;
            fast = 0;
            motor->trainsDir = 2; 
          }
          else if ((signed long)(encoder2_v-data2)>64) {
            status[4] = '1';
            TB3CCR0 = PULSE_PERIOD_SLOW;
            motor->trainsDir = 1;
            fast = 0;
          }
          else {
            status[4] = '0';
            TB3CCR0 = PULSE_PERIOD_SLOW;
            motor->trainsDir = 0; // stop
            fast = 0;
            finish = 1;
          }
        }//
        if (motor == &motor1) {
          encoder_en_1 = 1; // enable the encoder value storage
          while(!encoder_1_ready); // wait until encoder value is ready
          if ((signed long)(encoder1_v-data1)<-2048) {
            status[3] = '1';
            TB3CCR0 = PULSE_PERIOD_FAST;
            fast = 1;
            motor->trainsDir = 2; 
          }
          else if ((signed long)(encoder1_v-data1)>2048) {
            status[3] = '1';
            TB3CCR0 = PULSE_PERIOD_FAST;
            fast = 1;
            motor->trainsDir = 1;
          }
          else if ((signed long)(encoder1_v-data1)<-64) {
            status[3] = '1';
            TB3CCR0 = PULSE_PERIOD_SLOW;
            fast = 0;
            motor->trainsDir = 2; 
          }
          else if ((signed long)(encoder1_v-data1)>64) {
            status[3] = '1';
            TB3CCR0 = PULSE_PERIOD_SLOW;
            motor->trainsDir = 1;
            fast = 0;
          }
          else {
            status[3] = '0';
            TB3CCR0 = PULSE_PERIOD_SLOW;
            motor->trainsDir = 0; // stop
            fast = 0;
            finish = 1;
          }
        }
        if (motor == &motor2) {
          if (motor->trainsDir) status[1] = '1';
          TB3CCR0 = PULSE_PERIOD_MED;
        }
        if (motor == &motor4) {
          if (motor->trainsDir) status[2] = '1';
        }
        if (motor == &motor5) {
          if (motor->trainsDir) status[5] = '1';
          if (heaterFinish) {
            motor->trainsDir = 0;
            heaterFinish = 0;
            heaterCount = 0;
            heaterSet = 0;
            heater_en  = 0;
            finish = 1;
          }
        }
        /*
        if (motor == &motor6) {
          if (motor->trainsDir) status[6] = '1';
          else status[6] = '0';
          TB1CCR0 = PULSE_PERIOD_400K;
        }
        */
        sendPulseTrains(motor);
        
        if(finish) { // finish sending,stop
            finish = 0;
            motor->trainsDir = 0;
            motor->trainsSent = 0;
            motor->trainsToSend = 0;
            motor->trainsFinished = 1;
            //encoder_1_ready = 0;
            //encoder_2_ready = 0;
        }

        return 1;
    } else if(!motor->trainsToSend) { // when no command is received
        motor->trainsSent = 0;
        motor->trainsToSend = 0;
        motor->pulsesToSend = 0;
        motor->pulseTrainsSent = 0;
        motor->trainsFinished = 1;
    }

    return 0;
}

void sendPulseTrains(Motor* m) {
    m->pulseTrainsSent = 0;
    m->pulsesToSend = 1; // this value controls the running time 
    m->setMotor(m->trainsDir);
}

// dir:
// 0: stop
// 1: cw
// 2: ccw
void setMotor2(int dir) {
    if(!dir) {
        MOTOR2_P = 0;
        TB3CCR0 = PULSE_PERIOD_SLOW;
        return;
    }

    if(dir == 2) {
        DIR2_P_ON;
    } else if (dir == 1) {
        DIR2_P_OFF;
    }

    MOTOR2_P = PULSE_PERIOD_MED >> 1; // 50% duty cycle
}

void setMotor3(int dir) {
    if(!dir) {
        MOTOR3_P = 0;
        return;
    }

    if(dir == 2) {
        DIR3_P_ON;
    } else if (dir == 1) {
        DIR3_P_OFF;
    }
    if (fast) {
      MOTOR3_P = PULSE_PERIOD_FAST >> 1;
    }
    else {
      MOTOR3_P = PULSE_PERIOD_SLOW >> 1;
    }
}

void setMotor5(int dir) {
    if(!dir) {
        MOTOR5_P = 0;
        return;
    }
    if(dir == 2) {
        DIR5_P_ON;
    } else if (dir == 1) {
        DIR5_P_OFF;
    }

    MOTOR5_P = PULSE_PERIOD_SLOW >> 1;
}

void setMotor4(int dir) {
    if(!dir) {
        MOTOR4_P = 0;
        return;
    }

    if(dir == 2) {
        DIR4_P_ON;
    } else if (dir == 1) {
        DIR4_P_OFF;
    }

    MOTOR4_P = PULSE_PERIOD_SLOW >> 1;
}

void setMotor1(int dir) {
    if(!dir) {
        MOTOR1_P = 0;
        return;
    }

    if(dir == 2) {
        DIR1_P_ON
    } else if (dir == 1) {
        DIR1_P_OFF
    }

    if (fast) {
      MOTOR1_P = PULSE_PERIOD_FAST >> 1;
    }
    else {
      MOTOR1_P = PULSE_PERIOD_SLOW >> 1;
    }
}
/*
void setMotor6(int dir) {
    if(!dir) {
        MOTOR6_P = 0;
        return;
    }
    MOTOR6_P = PULSE_PERIOD_400K>>1;
}

#pragma vector=PORT1_VECTOR
__interrupt void Laser_interrupt(void) {
    if(((P1IFG & LASER) && *(motor6.PWM))) {
        if(++motor6.pulseCount >= motor6.pulsesToSend) {
            *(motor6.PWM) = 0;
            motor6.pulseTrainsSent = 1;
            motor6.pulseCount = 0;
            motor6.pulsesToSend = 0;
        }
    }
    P1IFG &= ~LASER;
}
*/
#pragma vector=PORT3_VECTOR
__interrupt void motorCount_interrupt(void) {
    if(((P3IFG & MOTOR1_P_COUNT) && *(motor1.PWM))) {
        if(++motor1.pulseCount >= motor1.pulsesToSend) {
            *(motor1.PWM) = 0; // same as motor_p = 0
            motor1.pulseTrainsSent = 1;
            motor1.pulseCount = 0;
            motor1.pulsesToSend = 0;
        }
    }

    if(((P3IFG & MOTOR2_P_COUNT) && *(motor2.PWM))) {
        if(++motor2.pulseCount >= motor2.pulsesToSend) {
            *(motor2.PWM) = 0;
            motor2.pulseTrainsSent = 1;
            motor2.pulseCount = 0;
            motor2.pulsesToSend = 0;
        }
    }

    if(((P3IFG & MOTOR3_P_COUNT) && *(motor3.PWM))) {
        if(++motor3.pulseCount >= motor3.pulsesToSend) {
            *(motor3.PWM) = 0;
            motor3.pulseTrainsSent = 1;
            motor3.pulseCount = 0;
            motor3.pulsesToSend = 0;
        }
    }

    if(((P3IFG & MOTOR4_P_COUNT) && *(motor4.PWM))) {
        if(++motor4.pulseCount >= motor4.pulsesToSend) {
            *(motor4.PWM) = 0;
            motor4.pulseTrainsSent = 1;
            motor4.pulseCount = 0;
            motor4.pulsesToSend = 0;
        }
    }

    if(((P3IFG & MOTOR5_P_COUNT) && *(motor5.PWM))) {
        if (heater_en) heaterCount++;
        if (heaterCount>heaterSet) {
            heaterFinish = 1;
        }
        if(++motor5.pulseCount >= motor5.pulsesToSend) {
            *(motor5.PWM) = 0;
            motor5.pulseTrainsSent = 1;
            motor5.pulseCount = 0;
            motor5.pulsesToSend = 0;
        }
    }
    
    P3IFG &= ~MOTOR1_P_COUNT; // reset interrupt flags
    P3IFG &= ~MOTOR2_P_COUNT;
    P3IFG &= ~MOTOR3_P_COUNT;
    P3IFG &= ~MOTOR4_P_COUNT;
    P3IFG &= ~MOTOR5_P_COUNT;
}