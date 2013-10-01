#ifndef ArpiMotors_h
#define ArpiMotors_h

#include <Arduino.h>

class ArpiMotors
{
  public:  
    // CONSTRUCTORS
    ArpiMotors(); // Default pin selection.
    ArpiMotors(unsigned char M1DIR, unsigned char M1PWM, unsigned char M2DIR, unsigned char M2PWM); // User-defined pin selection. 
    
    // PUBLIC METHODS
    void init(); // Initialize TIMER 1, set the PWM to 20kHZ. 
    void setM1Speed(int speed); // Set speed for M1.
    void setM2Speed(int speed); // Set speed for M2.
    void setSpeeds(int m1Speed, int m2Speed); // Set speed for both M1 and M2.
    
  private:
    unsigned char _M1DIR;
    unsigned char _M2DIR;
    unsigned char _M1PWM;
    unsigned char _M2PWM;

};

#endif
