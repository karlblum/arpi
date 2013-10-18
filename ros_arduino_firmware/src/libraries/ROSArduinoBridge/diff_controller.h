typedef struct {
  double SetpointTicks;
  double Input;  
  double PreviousInput;  
  double Output;
  double Encoder; 
  double PreviousEncoder;
  double f;
}
PidInfo;

PidInfo leftPID, rightPID;

PID myPIDL(&leftPID.Input, &leftPID.Output, &leftPID.SetpointTicks,4,100,1, DIRECT);
PID myPIDR(&rightPID.Input, &rightPID.Output, &rightPID.SetpointTicks,4,100,1, DIRECT);

unsigned char moving = 0; // is the base in motion?

void resetPID(){
   leftPID.Input = 0;
   leftPID.PreviousInput = 0;
   leftPID.Output = 0;
   leftPID.SetpointTicks = 0;
   leftPID.Encoder = 0;
   leftPID.PreviousEncoder = 0;
   leftPID.f = 1;
   
   rightPID.Input = 0;
   rightPID.PreviousInput = 0;
   rightPID.Output = 0;
   rightPID.SetpointTicks = 0;
   rightPID.Encoder = 0;
   rightPID.PreviousEncoder = 0;
   rightPID.f = 1;
}


void updatePID() {
    leftPID.Encoder = readEncoder(0);
    rightPID.Encoder = readEncoder(1);
  
  if (!moving){
    if (leftPID.PreviousInput != 0 || rightPID.PreviousInput != 0){
      resetPID();
    }
    return;
  }
    leftPID.Input = leftPID.Encoder - leftPID.PreviousEncoder;
    myPIDL.Compute();
    leftPID.PreviousEncoder = leftPID.Encoder;
    leftPID.PreviousInput = leftPID.Input;
    
    /*
    Serial.print("LEFT Setpoint: ");
    Serial.print(leftPID.SetpointTicks);
    Serial.print(" | Encoder: ");
    Serial.print(leftPID.Encoder);
    Serial.print(" | Input: ");
    Serial.print(leftPID.Input);
    Serial.print(" | Output: ");
    Serial.println(leftPID.Output);    
    */
    
    rightPID.Input = rightPID.Encoder - rightPID.PreviousEncoder;
    myPIDR.Compute();
    rightPID.PreviousEncoder = rightPID.Encoder;
    rightPID.PreviousInput = rightPID.Input;
    
    /* DEBUG
    Serial.print("RIGHT Setpoint: ");
    Serial.print(rightPID.SetpointTicks);
    Serial.print(" | Encoder: ");
    Serial.print(rightPID.Encoder);
    Serial.print(" | Input: ");
    Serial.print(rightPID.Input);
    Serial.print(" | Output: ");
    Serial.println(rightPID.Output);
    */
   
  /* Set the motor speeds accordingly */
  
  setMotorSpeeds(leftPID.Output * leftPID.f, rightPID.Output * rightPID.f);
}
