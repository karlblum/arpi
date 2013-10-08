#include <Arduino.h>

class ArpiMotors
{
  public:  
    ArpiMotors(); // Default pin selection.
    ArpiMotors(unsigned char M1DIR, unsigned char M1PWM, unsigned char M2DIR, unsigned char M2PWM); // User-defined pin selection. 
    
    void init();
    void setM1Speed(int speed);
    void setM2Speed(int speed);
    void setSpeeds(int m1Speed, int m2Speed);
    
  private:
    unsigned char _M1DIR;
    unsigned char _M2DIR;
    unsigned char _M1PWM;
    unsigned char _M2PWM;

};

