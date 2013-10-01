float volts;
float distance;
int IRpin=A0;

void setup() {
  pinMode(IRpin, INPUT);
  Serial.begin(9600);
}

void loop() {
  volts=analogRead(IRpin)*(5.0 / 1023.0);
  distance = 65*pow(volts, -1.10);

  Serial.print("Sensor value: ");
  Serial.print(volts,3);
  Serial.print("V, ");
  Serial.print(distance,3);
  Serial.println("cm");
  delay(1000);
}



