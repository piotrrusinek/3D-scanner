#ifndef stepper_h
#define stepper_h

void stepperInitialize(void);
void stepperPulse(void);
void stepperFaster(void);
void stepperSlower(void);
void stepperDir(void);
void stepperPulseSecond(void);
void  updateLCD(void);


#endif