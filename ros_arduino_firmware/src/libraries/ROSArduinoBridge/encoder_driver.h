// Encoder values
long coder[2] = {
  0,0};

/////////////// ENCODER FUNCTIONS

void EncoderTickL()
{
  coder[LEFT] ++;  //count the left wheel encoder interrupts
}


void EncoderTickR()
{
  coder[RIGHT] ++; //count the right wheel encoder interrupts
}

long readEncoder(int i) {
  if (i == LEFT) return coder[LEFT];
  else return coder[RIGHT];
}

/* Wrap the encoder reset function */
void resetEncoder(int i) {
  if (i == LEFT) {
    coder[LEFT] = 0;
  }
  else {
    coder[RIGHT] = 0;
  }
}

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

void initEncoders()
{
  attachInterrupt(0, EncoderTickL, CHANGE);
  attachInterrupt(1, EncoderTickR, CHANGE); 
}

