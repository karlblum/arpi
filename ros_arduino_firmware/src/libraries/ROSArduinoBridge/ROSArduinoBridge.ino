#include "Arduino.h"
#include "commands.h"
#include "sensors.h"
#include <Servo.h>
#include "servos.h"
#include <Wire.h>
#include "Adafruit_BMP085.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_ADXL345.h"
#include "L3G.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include <PID_v1.h>
#include "diff_controller.h"
#include <SoftwareSerial.h>

#define BAUDRATE     9600
SoftwareSerial mySerial(9, 8);

/* PID loop parameters */
// MAX Ticks per second is about 30-50 with current encoders!!!!
#define PID_RATE           10.0     // Hz
const int PID_INTERVAL = 1000 / PID_RATE;
unsigned long nextPID = PID_INTERVAL;

/* Stop the robot if it hasn't received a movement command
 in this number of milliseconds */
#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

/* Pressure sensor */
Adafruit_BMP085 bmp;

/* Accelerometer */
Adafruit_ADXL345 accel = Adafruit_ADXL345(12345);
sensors_event_t event;

/* Gyroscope */
L3G gyro;

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
    mySerial.println(BAUDRATE);
    break;
  case ANALOG_READ:
    mySerial.println(analogRead(arg1));
    break;
  case DIGITAL_READ:
    mySerial.println(digitalRead(arg1));
    break;
  case ANALOG_WRITE:
    analogWrite(arg1, arg2);
    mySerial.println("OK"); 
    break;
  case DIGITAL_WRITE:
    if (arg2 == 0) digitalWrite(arg1, LOW);
    else if (arg2 == 1) digitalWrite(arg1, HIGH);
    mySerial.println("OK"); 
    break;
  case ENVIR_DATA: 
    mySerial.print(bmp.readTemperature());
    mySerial.print(" ");
    mySerial.print(bmp.readPressure());
    mySerial.print(" ");
    mySerial.println(bmp.readAltitude()); 
    break;
  case ACCELERATION:
    mySerial.print("X: "); 
    mySerial.print(event.acceleration.x); 
    mySerial.print("  ");
    mySerial.print("Y: "); 
    mySerial.print(event.acceleration.y); 
    mySerial.print("  ");
    mySerial.print("Z: "); 
    mySerial.print(event.acceleration.z); 
    mySerial.print("  ");
    mySerial.println("m/s^2 ");
    break;
  case GYROSCOPE:
    gyro.read();
    mySerial.print("G ");
    mySerial.print("X: ");
    mySerial.print((int)gyro.g.x);
    mySerial.print(" Y: ");
    mySerial.print((int)gyro.g.y);
    mySerial.print(" Z: ");
    mySerial.println((int)gyro.g.z);
    break;
  case PIN_MODE:
    if (arg2 == 0) pinMode(arg1, INPUT);
    else if (arg2 == 1) pinMode(arg1, OUTPUT);
    mySerial.println("OK");
    break;
  case PING:
    mySerial.println(Ping(arg1));
    break;
  case SERVO_WRITE:
    servos[arg1].write(arg2);
    mySerial.println("OK");
    break;
  case SERVO_READ:
    mySerial.println(servos[arg1].read());
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

    mySerial.println("OK"); 
    break;
  case READ_ENCODERS:
    mySerial.print(readEncoder(LEFT));
    mySerial.print(" ");
    mySerial.println(readEncoder(RIGHT));
    break;
  case RESET_ENCODERS:
    resetEncoders();
    resetPID();
    mySerial.println("OK");
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
    mySerial.println("OK");
    break;
  case IR:
    mySerial.println("OK");
    break;
  default:
    mySerial.println("Invalid Command");
    break;
  }
}


void setup() {
  pinMode(9, INPUT);
  pinMode(8, OUTPUT);
  mySerial.begin(BAUDRATE);
  Wire.begin();

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

/*
  if (!bmp.begin()) {
    Serial1.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {
    }
  }

  if(!accel.begin())
  {
    Serial1.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  if (!gyro.init())
  {
    Serial1.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();
*/
}



/* Enter the main loop.  Read and parse input from the serial port
 and run any valid commands. Run a PID calculation at the target
 interval and check for auto-stop conditions.
 */
void loop() {

  /* Accelerometer update*/
  accel.getEvent(&event);

  while (mySerial.available() > 0) {

    // Read the next character
    chr = mySerial.read();

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
    } 
    else {
      nextPID += PID_INTERVAL;
    } 
    updatePID();
  }

  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
    ;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}




