int LEFT_WHEEL = 0; //Interrupt 0, input pin 2
int RIGHT_WHEEL = 1; //Interrupt 1, input pin 3
float ENC_DIST_COEF = 1.05; //cm per encoder interrupt

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

long wheelEncoder[2] = {
  0,0};

void setup(){ 
  Serial.begin(9600);                            
  attachInterrupt(LEFT_WHEEL, LwheelEnc, CHANGE);    
  attachInterrupt(RIGHT_WHEEL, RwheelEnc, CHANGE);  
  for(int i=4;i<=7;i++)
    pinMode(i, OUTPUT);   
}

void loop(){
  float distance = wheelEncoder[RIGHT_WHEEL]*ENC_DIST_COEF;
  advance(100,100);
  if(distance>100) stop();
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


void LwheelEnc(){
  wheelEncoder[LEFT_WHEEL] ++;  //count the left wheel encoder interrupts
}


void RwheelEnc(){
  wheelEncoder[RIGHT_WHEEL] ++; //count the right wheel encoder interrupts
}


