#include "ArpiEncoders.h"

ArpiEncoders::ArpiEncoders()
{
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_XY,   OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_OE,   OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_EN1,  OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_EN2,  OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTX, OUTPUT);
   pinMode(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTY, OUTPUT);

   // XY LOW  X Axis AKA 1st Axis
   // XY HIGH Y Axis AKA 2nd Axis
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_XY, LOW);

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_OE, HIGH);  // Active LOW

   switchCountMode( countMode );

   // Byte Selected MSB SEL1  LOW SEL2 HIGH
   // Byte Selected 2nd SEL1 HIGH SEL2 HIGH
   // Byte Selected 3rd SEL1  LOW SEL2 LOW
   // Byte Selected LSB SEL1 HIGH SEL2 LOW
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, HIGH);

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTX, HIGH);  // Active LOW
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTY, HIGH);  // Active LOW

   XAxisReset( );
   YAxisReset( );
}

// Communicates with a HCTL-2032 IC to get reset the X encoder count
// see Avago/Agilent/HP HCTL-2032 PDF for details
void ArpiEncoders::ResetR( )
{
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTX, LOW);
   delayMicroseconds(1);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTX, HIGH);
   delayMicroseconds(1);
}

// Communicates with a HCTL-2032 IC to get the X Axis encoder count via an 8bit parallel bus
// see Avago/Agilent/HP HCTL-2032 Datasheet PDF for details
unsigned long ArpiEncoders::GetCountR( )
{
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_XY,   LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_OE,   LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, HIGH);
   delayMicroseconds(1);
   busByte = PINA;
   count   = busByte;
   count <<= 8;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, HIGH);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, HIGH);
   delayMicroseconds(1);
   busByte = PINA;
   count  += busByte;
   count <<= 8;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, LOW);
   delayMicroseconds(1);
   busByte = PINA;
   count  += busByte;
   count <<= 8;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, HIGH);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, LOW);
   delayMicroseconds(1);
   busByte = PINA;
   count  += busByte;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_OE,  HIGH);

   return count;
}

// Communicates with a HCTL-2032 IC to get reset the Y encoder count
// see Avago/Agilent/HP HCTL-2032 PDF for details
void ArpiEncoders::ResetL( )
{
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTY, LOW);
   delayMicroseconds(1);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_RSTY, HIGH);
   delayMicroseconds(1);
}

// Communicates with a HCTL-2032 IC to get the Y Axis encoder count via an 8bit parallel bus
// see Avago/Agilent/HP HCTL-2032 PDF for details
unsigned long ArpiEncoders::GetCountL( )
{
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_XY,   HIGH);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_OE,   LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, HIGH);
   delayMicroseconds(1);
   busByte = PINA;
   count   = busByte;
   count <<= 8;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, HIGH);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, HIGH);
   delayMicroseconds(1);
   busByte = PINA;
   count  += busByte;
   count <<= 8;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, LOW);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, LOW);
   delayMicroseconds(1);
   busByte = PINA;
   count  += busByte;
   count <<= 8;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL1, HIGH);
   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_SEL2, LOW);
   delayMicroseconds(1);
   busByte = PINA;
   count  += busByte;

   digitalWrite(MEGA_QUADRATURE_ENCODER_COUNTER_PIN_OE,  HIGH);

   return count;
}

