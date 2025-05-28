#define TB0CCR0_INTERVAL                (40)          //  10us
#define TB0CCR1_INTERVAL                (20)           //  us
#define TB0CCR2_INTERVAL                (50000)         // 12.5 ms

#define TB1CCR0_INTERVAL                (20)         // ms
#define TB1CCR1_INTERVAL                (50000)
#define TB1CCR2_INTERVAL                (25000)         // ms


#define MOTOR1_P                        (TB3CCR1)
#define MOTOR2_P                        (TB3CCR2)
#define MOTOR3_P                        (TB3CCR3)
#define MOTOR4_P                        (TB3CCR4)
#define MOTOR5_P                        (TB3CCR5)
//
//#define MOTOR6_P                        (TB1CCR1)

#define PULSE_PERIOD_SLOW                    (20000)//
#define PULSE_PERIOD_MED                     (20000)//
#define PULSE_PERIOD_FAST                    (2000)//
#define PULSE_PERIOD_400K                    (40)// for laser

void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B3(void);