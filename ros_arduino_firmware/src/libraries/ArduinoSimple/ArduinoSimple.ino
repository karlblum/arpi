#include "ArpiMotors.h"

ArpiMotors drive;
boolean moving = false;
boolean turning = false;
int pingInterval = 100;
long nextPing = 0;
int ping;
int range = 0;
int rangeL = 0;
int rangeR = 0;

void setup() {
  drive.init();
  Serial.begin(9600);
}

void loop() {
  unsigned long time;
  time = millis();

  if (time > nextPing) {
    rangeL = PingAVG(9);
    rangeR = PingAVG(8);
    Serial.print("RL: ");
    Serial.print(rangeL);
    Serial.print(" - RR: ");
    Serial.println(rangeR);

    if (rangeR == 0 && rangeL == 0) {
      range = 32000;
    } else {
      range = min(rangeL, rangeR);
    }

    nextPing = time + pingInterval;
  }

  if (range > 10 && !moving) {
    drive.stop();
    drive.mvForward();
    moving = true;
    turning = false;
    Serial.println("Started moving");
  } else if (range <= 10 && !turning) {
    drive.stop();
    drive.mvRight();
    moving = false;
    turning = true;
    Serial.println("Started turning");
  }

}

long Ping(int pin) {
  long duration, range, r;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH, 6000); // Timeout in microseconds, sound travel time = 340m/s ==> 1cm/29,4 microseconds
  
  if (duration == 0) {
    r = 100000;
  } else {
    r = duration;
  }
  range = r / 58;

  return (range);
}

long PingAVG(int pin) {
  long r1, r2, r3, range;
  r1 = Ping(pin);
  r2 = Ping(pin);
  r3 = Ping(pin);
  range = (r1 + r2 + r3) / 3;
  return range;
}
