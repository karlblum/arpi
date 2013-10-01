#include "ArpiMotors.h"

// Constructors ////////////////////////////////////////////////////////////////

ArpiMotors::ArpiMotors()
{
  //Pin map
  _M1DIR = 4;
  _M2DIR = 7;
  _M1PWM = 5;
  _M2PWM = 6;
}

ArpiMotors::ArpiMotors(unsigned char M1DIR, unsigned char M1PWM, unsigned char M2DIR, unsigned char M2PWM)
{
  //Pin map
  _M1DIR = M1DIR;
  _M2DIR = M2DIR;
  _M1PWM = M1PWM; 
  _M2PWM = M2PWM;
}

// Public Methods //////////////////////////////////////////////////////////////
void ArpiMotors::init()
{
// Define pinMode for the pins

  pinMode(_M1DIR,OUTPUT);
  pinMode(_M1PWM,OUTPUT);
  pinMode(_M2DIR,OUTPUT);
  pinMode(_M2PWM,OUTPUT);

}

void ArpiMotors::setM1Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 255)  // Max PWM dutycycle
    speed = 255;
    
  analogWrite(_M1PWM,speed);
  
  if (reverse)
    digitalWrite(_M1DIR,HIGH);
  else
    digitalWrite(_M1DIR,LOW);
}

// Set speed for motor 2, speed is a number betwenn -400 and 400
void ArpiMotors::setM2Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 255)  // Max PWM dutycycle
    speed = 255;
  
  analogWrite(_M2PWM,speed);  
    
  if (reverse)
    digitalWrite(_M2DIR,HIGH);
  else
    digitalWrite(_M2DIR,LOW);
}

// Set speed for motor 1 and 2
void ArpiMotors::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}
