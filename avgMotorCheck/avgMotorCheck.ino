/*
 *   Code to operate the MMP-5 with a Sabertooth Motor Controller set to R/C mode
 *   The Sabertooth expects to receive a "center" or "stop" position first (as if the R/C
 *   joystick was in the neutral position.
 *   Then it expect to receive the end limits.
 *   If this code is not executed when the controller is powered up, the MMP-5 may move
 *   eratically or not at all
 *   COL Lisa A. Shay
 */

#include <Servo.h>

Servo right;  // create servo object to control a servo
                // twelve servo objects can be created on most boards
Servo left;

int i = 0;      // loop counter
const int dur = 2;  // counter duration 2 is the minimun setting to
                    // produce reliable results.  Increase to 25 if
                    // the motor controller behaves eratically.

const int timeTo4FT=1; //Change this to alter how long the system runs and how far it goes

void setup()
{
  right.attach(10);   // attaches the servo on pin 10 to the servo object
  left.attach(11);
  //This is one of the servo ports on the Parallax shield. Others are 11, 12, and 13

  for (i = 0; i < dur; i++ )
 {
  right.writeMicroseconds(1500);    // motor stop
  delay(15);
  }

  for (i = 0; i< dur; i++)
  {
     right.writeMicroseconds(1000);     // full speed in one direction
     delay(15);
  }
  for (i = 0; i< dur; i++)
  {
    right.writeMicroseconds(2000);   // full speed in the other direction
    delay(15);
  }
    for (i = 0; i < dur; i++ )
 {
  right.writeMicroseconds(1500);    // motor stop
  delay(15);
  }


// test that code did what it was supposed to do (omit or replace with your algorithm)
  for (i = 0; i< dur*100; i++)
  {
     right.writeMicroseconds(1250);     // half speed in one direction
     delay(15);
  }
    for (i = 0; i< dur*100; i++)
  {
     right.writeMicroseconds(1750);     // half speed in one direction
     delay(15);
  }
   right.writeMicroseconds(1500);    // motor stop


  for (i = 0; i < dur; i++ )
 {
  left.writeMicroseconds(1500);    // motor stop
  delay(15);
  }

  for (i = 0; i< dur; i++)
  {
     left.writeMicroseconds(1000);     // full speed in one direction
     delay(15);
  }
  for (i = 0; i< dur; i++)
  {
    left.writeMicroseconds(2000);   // full speed in the other direction
    delay(15);
  }
    for (i = 0; i < dur; i++)
 {
  left.writeMicroseconds(1500);    // motor stop
  delay(15);
  }

// test that code did what it was supposed to do (omit or replace with your algorithm)
  for (i = 0; i< dur*100; i++)
  {
     left.writeMicroseconds(1250);     // half speed in one direction
     delay(15);
  }
    for (i = 0; i< dur*100; i++)
  {
     left.writeMicroseconds(1750);     // half speed in one direction
     delay(15);
  }
   left.writeMicroseconds(1500);    // motor stop

}

void loop()
{
 for(i=0; i<timeTo4FT*100;i++){
     right.writeMicroseconds(1000);
     left.writeMicroseconds(1000);
    delay(15);
 }

}
