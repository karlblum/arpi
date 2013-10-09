typedef struct {
  double SetpointTicks;
  double Input;  
  double PreviousInput;  
  double Output;
  double Encoder; 
  double PreviousEncoder; 
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
   
   rightPID.Input = 0;
   rightPID.PreviousInput = 0;
   rightPID.Output = 0;
   rightPID.SetpointTicks = 0;
   rightPID.Encoder = 0;
   rightPID.PreviousEncoder = 0;
}

void doPID(PidInfo * p) {
      p->Input = p->Encoder - p->PreviousEncoder;
      myPIDL.Compute();
      p->PreviousEncoder = p->Encoder;
      p->PreviousInput = p->Input;
}

void updatePID() {
    leftPID.Encoder = readEncoder(LEFT);
    rightPID.Encoder = readEncoder(RIGHT);
  
  if (!moving){
    if (leftPID.PreviousInput != 0 || rightPID.PreviousInput != 0) resetPID();
    return;
  }

  /* Compute PID update for each motor */
  doPID(&rightPID);
  doPID(&leftPID);

  /* Set the motor speeds accordingly */
  setMotorSpeeds(leftPID.Output, rightPID.Output);
}
