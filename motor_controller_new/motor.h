typedef struct {
    volatile unsigned short * PWM;
    volatile unsigned int pulseCount;
    volatile unsigned int pulseTrainsSent;
    volatile unsigned int pulsesToSend;
    unsigned int trainsToSend;
    unsigned int trainsDir;
    unsigned int trainsSent;
    unsigned int trainsFinished;
    void (*setMotor)(int);
} Motor;


int sendTrains(Motor*);
void sendPulseTrains( Motor* m);
void setMotor2(int);
void setMotor1(int);
void setMotor3(int);
void setMotor4(int);
void setMotor5(int);
void setMotor6(int);

