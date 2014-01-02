#include "ArpiMotors.h"

ArpiMotors drive;
boolean moving = false;
boolean turning = false;
int pingInterval = 500;
long nextPing = 0;
int ping;
int range = 0;

void setup() {
  drive.init();
  Serial.begin(9600);
}

void loop() {
  unsigned long time;
  time = millis();

  if (time > nextPing) {
    range = Ping(10);
    Serial.println(range);
    nextPing = time + pingInterval;
  }

  if (range > 30 && !moving) {
    drive.stop();
    delayMicroseconds(1000);
    drive.mvForward();
    moving = true;
    turning = false;
    Serial.println("Started moving");
  } else if (range <= 30 && !turning) {
    drive.stop();
    delayMicroseconds(1000);
    drive.mvLeft();
    moving = false;
    turning = true;
    Serial.println("Started turning");
  }

}

long Ping(int pin) {
  long duration, range;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH, 23200); //Lets set timeout to 4m sound travel time
  // convert the time into meters
  range = duration / 58;

  return (range);
}
