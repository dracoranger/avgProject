#include <Servo.h>

// this constant won't change. It's the pin number of the sensor's output:
//Probably should capitalize
const int pingFront = 6;
const int pingSide = 8;
const int LIGHT_SENSOR = 5;
const int TAPE_DETECT= 3;
const int IR_LED=4;

Servo right;  // create servo object to control a servo
Servo left;              // twelve servo objects can be created on most boards

int i = 0;      // loop counter
const int dur = 2;  // counter duration 2 is the minimun setting to
                    // produce reliable results.  Increase to 25 if
                    // the motor controller behaves eratically.


void setup() {
  // initialize serial communication:
  pinMode(LIGHT_SENSOR, INPUT);
  pinMode(TAPE_DETECT, INPUT);
  pinMode(IR_LED,OUTPUT);
  right.attach(13);   // attaches the servo on pin 10 to the servo object
  left.attach(12);
  //This is one of the servo ports on the Parallax shield. Others are 11, 12, and 13
  Serial.begin(9600);
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
    for (i = 0; i < dur; i++ )
 {
  left.writeMicroseconds(1500);    // motor stop
  delay(15);
  }



}

void loop() {                // sets the servo position according to the scaled value

  long duration, inchesF, cmF, inchesS, cmS;

  pinMode(pingFront, OUTPUT);
  digitalWrite(pingFront, LOW);
  delayMicroseconds(2);
  digitalWrite(pingFront, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingFront, LOW);
  pinMode(pingSide, OUTPUT);
  digitalWrite(pingSide, LOW);
  delayMicroseconds(2);
  digitalWrite(pingSide, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingSide, LOW);



  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingFront, INPUT);
  duration = pulseIn(pingFront, HIGH);

  inchesF = microsecondsToInches(duration);
  cmF = microsecondsToCentimeters(duration);

  pinMode(pingSide, INPUT);
  duration = pulseIn(pingSide, HIGH);

  inchesS = microsecondsToInches(duration);
  cmS = microsecondsToCentimeters(duration);

  Serial.print(" Front: ");
  Serial.print(inchesF);
  Serial.print("in, ");
  Serial.print(cmF);
  Serial.print("cm. Side ");
  Serial.print(inchesF);
  Serial.print("in, ");
  Serial.print(cmF);
  Serial.print("cm.");

  Serial.println();

  delay(200);
  if(inchesF>25){
    forward();
    delay(1000);
  //delay(1500);
  else{
    reverse();
    delay(1000);
    leftTurn();
    delay(500);
    spd = inschesS;
  }
  //delay(6000);
  //stopRobot();

//Motion routines for robot
void forward() {
  left.writeMicroseconds(1650);
  right.writeMicroseconds(1650);
}
void reverse() {
  left.writeMicroseconds(1250);
  right.writeMicroseconds(1250);
}
void leftTurn() {
  left.writeMicroseconds(1250);
  right.writeMicroseconds(1650);
}
void stopRobot() {
  left.detach();
  right.detach();
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
