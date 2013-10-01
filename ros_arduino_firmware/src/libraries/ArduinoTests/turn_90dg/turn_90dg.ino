int LEFT_WHEEL = 0; //Interrupt 0, input pin 2
int RIGHT_WHEEL = 1; //Interrupt 1, input pin 3
float ENC_DIST_COEF = 1.05; //cm per encoder interrupt

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

long wheelEncoder[2] = {
  0,0};

int encCount = 0;

void setup(){ 
  Serial.begin(9600);                            
  attachInterrupt(LEFT_WHEEL, LwheelEnc, CHANGE);    
  attachInterrupt(RIGHT_WHEEL, RwheelEnc, CHANGE);  
  for(int i=4;i<=7;i++)
    pinMode(i, OUTPUT);   
}

void loop(){

  if(wheelEncoder[0] >= encCount) {
    stop();
    Serial.print("Moved: ");
    Serial.print(wheelEncoder[0]);
    Serial.print("[L], ");
    Serial.print(wheelEncoder[1]);
    Serial.println("[R]");
    
    encCount = getInput();
    Serial.print("Input: ");
    Serial.println(encCount);   
    wheelEncoder[0] = 0;
    wheelEncoder[1] = 0;
  } else {
    turn_L(255,255);
  }
    delay(100);
}


void stop(void)                    //Stop
{
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);      
}   

void advance(char a,char b)          //Move forward
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}

void turn_L (char a,char b)             //Turn Left
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}


void LwheelEnc(){
  wheelEncoder[LEFT_WHEEL] ++;  //count the left wheel encoder interrupts
}


void RwheelEnc(){
  wheelEncoder[RIGHT_WHEEL] ++; //count the right wheel encoder interrupts
}

int getInput(){
  char intBuffer[12];
  String intData = "";
  int delimiter = (int) '\n';
  boolean receivingInput = true;

  while (receivingInput) {
    int ch = Serial.read();
    if (ch == -1) {
      
    }
    else if (ch == delimiter) {
      receivingInput = false;
    }
    else {
      intData += (char) ch;
    }
  }

  // Copy read data into a char array for use by atoi
  // Include room for the null terminator
  int intLength = intData.length() + 1;
  intData.toCharArray(intBuffer, intLength);

  // Reinitialize intData for use next time around the loop
  intData = "";

  // Convert ASCII-encoded integer to an int
  int i = atoi(intBuffer);
  Serial.println("Returning value");
  return i;
}








