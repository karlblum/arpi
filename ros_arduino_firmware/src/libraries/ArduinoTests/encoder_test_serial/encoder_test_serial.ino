int LEFT_WHEEL = 0; //Interrupt 0, input pin 2
int RIGHT_WHEEL = 1; //Interrupt 1, input pin 3
float ENC_DIST_COEF = 1.05; //cm per encoder interrupt

long wheelEncoder[2] = {
  0,0};

void setup(){ 
  Serial.begin(9600);                            
  attachInterrupt(LEFT_WHEEL, LwheelEnc, CHANGE);    
  attachInterrupt(RIGHT_WHEEL, RwheelEnc, CHANGE);    
}

void loop(){
  Serial.print("Coder value: ");
  Serial.print(wheelEncoder[LEFT_WHEEL]);
  Serial.print("[L] ");
  Serial.print(wheelEncoder[RIGHT_WHEEL]);
  Serial.print("[R] (");
  float distance = wheelEncoder[RIGHT_WHEEL]*ENC_DIST_COEF;
  Serial.print(distance,3);
  Serial.println("cm)");
  delay(1000);
}


void LwheelEnc(){
  wheelEncoder[LEFT_WHEEL] ++;  //count the left wheel encoder interrupts
}


void RwheelEnc(){
  wheelEncoder[RIGHT_WHEEL] ++; //count the right wheel encoder interrupts
}

