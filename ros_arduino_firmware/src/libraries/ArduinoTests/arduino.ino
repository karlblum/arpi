int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

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
void back_off (char a,char b)          //Move backward
        {
          analogWrite (E1,a);
          digitalWrite(M1,LOW);   
          analogWrite (E2,b);    
          digitalWrite(M2,LOW);
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
void setup(void) 
{ 
    int i;
    for(int i=6;i<=9;i++)
    pinMode(i, OUTPUT);  
    Serial.begin(19200);      //Set Baud Rate
} 
void loop(void) 
{ 
     char val = Serial.read();
     if(val!=-1)
       {
          switch(val)
           {
             case 'w'://Move Forward
                     advance (100,100);   //PWM Speed Control
                     break;
             case 's'://Move Backward
                     back_off (100,100);
                     break;
             case 'a'://Turn Left
                     turn_L (255,255);
                     break;       
             case 'd'://Turn Right
                     turn_R (255,255);
                     break;          
            }     
          delay(100); 
       }
      else stop();  
}
