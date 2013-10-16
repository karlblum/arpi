#include "Arduino.h"
#include "commands.h"
#include "sensors.h"
#include <Servo.h>
#include "servos.h"
#include <Wire.h>
#include <BMP085.h>
#include <motor_driver.h>
#include "encoder_driver.h"
#include <PID_v1.h>
#include "diff_controller.h"

#define BAUDRATE     57600 /* Serial port baud rate */

/* Run the PID loop at 10 times per second */
// MAX Ticks per second is about 30-50 with current encoders!!!!
#define PID_RATE           10.0     // Hz

/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;

/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;

/* Stop the robot if it hasn't received a movement command
 in this number of milliseconds */
#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

/* Init pressure and temeprature sensor */
BMP085 dps = BMP085();



// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index = 0;
char chr; // Variable to hold an input character
char cmd; // Variable to hold the current single-character command

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;

/* Clear the current command parameters */
void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

/* Run a command.  Commands are defined in commands.h */
int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch(cmd) {
  case GET_BAUDRATE:
    Serial.println(BAUDRATE);
    break;
  case ANALOG_READ:
    Serial.println(analogRead(arg1));
    break;
  case DIGITAL_READ:
    Serial.println(digitalRead(arg1));
    break;
  case ANALOG_WRITE:
    analogWrite(arg1, arg2);
    Serial.println("OK"); 
    break;
  case DIGITAL_WRITE:
    if (arg2 == 0) digitalWrite(arg1, LOW);
    else if (arg2 == 1) digitalWrite(arg1, HIGH);
    Serial.println("OK"); 
    break;
  case PIN_MODE:
    if (arg2 == 0) pinMode(arg1, INPUT);
    else if (arg2 == 1) pinMode(arg1, OUTPUT);
    Serial.println("OK");
    break;
  case PING:
    Serial.println(Ping(arg1));
    break;
  case SERVO_WRITE:
    servos[arg1].write(arg2);
    Serial.println("OK");
    break;
  case SERVO_READ:
    Serial.println(servos[arg1].read());
    break;
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0) {
      setMotorSpeeds(0, 0);
      moving = 0;
    }
    else moving = 1;
    //Set target encoder ticks per frame
     if(arg1<0) {
       leftPID.f = -1;    
     }
     else { 
       leftPID.f = 1;
     }
     leftPID.SetpointTicks = arg1/PID_RATE * leftPID.f;
   
     if(arg2<0) {
       rightPID.f = -1;
     }
     else { 
       rightPID.f = 1; 
     }
     rightPID.SetpointTicks = arg2/PID_RATE * rightPID.f;
     
    Serial.println("OK"); 
    break;
  case READ_ENCODERS:
    Serial.print(readEncoder(LEFT));
    Serial.print(" ");
    Serial.println(readEncoder(RIGHT));
    break;
  case RESET_ENCODERS:
    resetEncoders();
    resetPID();
    Serial.println("OK");
    break;
  case UPDATE_PID:
    while ((str = strtok_r(p, ":", &p)) != '\0') {
       pid_args[i] = atoi(str);
       i++;
    }
    //Kp = pid_args[0];
    //Kd = pid_args[1];
    //Ki = pid_args[2];
    //Ko = pid_args[3];
    Serial.println("OK");
    break;
  default:
    Serial.println("Invalid Command");
    break;
  }
}


void setup() {
  Serial.begin(BAUDRATE);

  initMotorController();
  initEncoders();

  /* Attach servos if used */
  int i;
  for (i = 0; i < N_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
  }
  servos[0].write(0);
  servos[1].write(90);
  
  myPIDL.SetMode(AUTOMATIC);
  myPIDR.SetMode(AUTOMATIC);
  myPIDL.SetOutputLimits(0,255);
  myPIDR.SetOutputLimits(0,255);
}

/* Enter the main loop.  Read and parse input from the serial port
 and run any valid commands. Run a PID calculation at the target
 interval and check for auto-stop conditions.
 */
void loop() {
  while (Serial.available() > 0) {

    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      runCommand();
      resetCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }
  
  // Execute motor commands
  if (millis() > nextPID) {
    if (!moving){
        leftPID.SetpointTicks = 0.0;
        rightPID.SetpointTicks = 0.0;
    } else {
      nextPID += PID_INTERVAL;
    } 
    updatePID();
  }
  
  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}


