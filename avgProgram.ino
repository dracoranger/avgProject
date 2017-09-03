/*
  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

  The circuit:
    - +V connection of the PING))) attached to +5V
    - GND connection of the PING))) attached to ground
    - SIG connection of the PING))) attached to digital pin 7

  created 3 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe
  Modified once again by DracoRanger for use in school
  Will probably never see the light of day
  Might push to github if I can be asked.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Ping
*/


#include <Servo.h>

// this constant won't change. It's the pin number of the sensor's output:
const int pingFront = 7;
const int pingSide = 8;
const int whiskerLeft = 2;
const int whiskerRight = 3;
const int lightSensor = 5;
const int tapeDetect= 9;

// Sets up bool for light
bool isDarkRoom=false;

//sets up whether or not was on wall
//used to differentiate between the wall and random obsticles
int byWall=0;


//Constant values, used to clean up if statements rather than worry about them being inconsistent
const int WALL_SAFE=5; //point of certainty that we're near a wall
const int FAR=18; //Side far
const int CLO=12; //Side close
const int FAR_FRONT=24; //Front close
const int FAR_SIDE=24; //In the middle of nowhere, used at beginning
const int T_90_TURN=1000;
const int T_1_FOOT=1000;
const int T_4_FEET=4000;

//We could create a function to help the robot "learn"
//No effing way I'm implementing a deep learning algorithm.
//Not enough runs and the chances of substantially improving are low as heck.
//On second run, change behavior to try safer tactics?


Servo myservo;  // create servo object to control a servo
                // twelve servo objects can be created on most boards

int i = 0;      // loop counter
const int dur = 2;  // counter duration 2 is the minimun setting to
                    // produce reliable results.  Increase to 25 if
                    // the motor controller behaves eratically.


void setup() {
  // initialize serial communication:
  Serial.begin(9600);

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
    for (i = 0; i < dur; i++ )
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

void loop() {
  /*
   * Deal with Everything else
   * TODO: Movement controls, whiskers, lighting, space out the delayMicrosecond?
   * 8/29 moved before AI to make sure that AI has the ability to use set vaiables and it compiles, which isn't necessarily unexpected
   *
  */

  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:

  long duration, inchesF, cmF, inchesS, cmS;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
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

  // convert the time into a distance
  inchesF = microsecondsToInches(duration);
  cmF = microsecondsToCentimeters(duration);

  pinMode(pingSide, INPUT);
  duration = pulseIn(pingSide, HIGH);

  // convert the time into a distance
  inchesS = microsecondsToInches(duration);
  cmS = microsecondsToCentimeters(duration);

  //TODO: Whisker alert



  Serial.print(inchesF);
  Serial.print("in, ");
  Serial.print(cmF);
  Serial.print("cm");
  Serial.println();

  delay(100);//Do we really want a serious delay?
             //Place detection into its own function and just run it every 100ms?
   /*
    * AI
    * The set behaviors of the robot are designated here
    *
    */

  if(whiskerLeft||whiskerRight){
    //STOP AND REVERSE, whiskers detected something's right in front of us
    //Turn as well?  Don't want to get stuck in an infinite loop
  }
  else if(isDarkRoom){
    //Head Towards the Light
    //deal with obsticles
    //basically recreate AI within this section, keeping the about to crash paramount
    //use light in front as director.  Need to worry about being attracted to the light from the door
  }
  else if(inchesS-CLO>FAR&&byWall>WALL_SAFE&&!tapePresent){
    //Discovered a Door.  Go investigate
    //Value of the distance between the robot and the wall suddenly increased by at least 18 inches, either a door or we got around a large obsticle
    //Need to worry about the large obsticle, hopefully byWall is helpful here
    //detect dark room and toggle isDarkRoom
    turnRight();//Or left...
    driveForwardLong();//clear doorframe
    int doit=detectLightInit();
    if(doit==1){//1 counts as a true generally. Not going to deal with the general case
      //decide to continue forward or turn right.  Recommend using inchesS as discriminator
    }
    else{
      driveBackwardsLong();
      turnLeft();//reverse the turn right
    }
  }
  else if(inchesF>FAR_FRONT&&inchesS>FAR_SIDE&&byWall==0){
    //in the middle of the room Go forward
  }
  else if(inchesF<FAR_FRONT && byWall==0){
    //reached the wall for the first time, turn left
    //Need to alter based on where turning brings us in relation to a wall

  }
  else if(inchesF>FAR_FRONT && ((inchesS>CLO && inchesS<FAR)||tapePresent)){
    //discovered the wall, continue along it in relative safety
    //increase byWall

  }
  else if(inchesF>FAR_FRONT && inchesS<CLO){
    //discovered the wall, continue along it, shift to the left to prevent running into it

  }
  else if(inchesF>FAR_FRONT && inchesS>FAR && inchesS<FAR_SIDE){
    //discovered the wall, continue along it, shifted to the right to prevent driving too far away

  }
  else if(inchesF<FAR_FRONT && ((inchesS>CLO && inchesS<FAR)||tapePresent) && byWall> WALL_SAFE){
    //discovered a turn, so lets turn

  }
  else{
    //we suddenly found a large gap or something early on.
    //could be a door or could just have been a large obsticle.  Nothing's definite at the moment.
    //think reset byWall to 0 and basically act as if we are just entering the room for the first time.
    //Really need to test this, probably the best idea to revert to careful without assumptions in this case.
    //May want to check how much byWall there is, because we could lose a good amount of certainty otherwise.

  }
 }


/*
 * Controller functions
 * Names should be self explanatory.
 * They're designed to be opposites of each other to allow for some level of control
 */

int turnRight(){
  for(int i=0; i<T_90_TURN; i++){
    right.writeMicroseconds(2000);
    left.writeMicroseconds(1000);
  }
  return 1;
}

int shiftRight(){

  return 1;
}

int turnLeft(){
  for(int i=0; i<T_90_TURN; i++){
    right.writeMicroseconds(1000);
    left.writeMicroseconds(2000);
  }
  return 1;
}

int shiftLeft(){

  return 1;
}

int driveForward(){

  return 1;
}
int driveForwardLong(){

  return 1;
}
int driveForward4Ft(){

  return 1;
}
int driveBackwards(){

  return 1;
}
int driveBackwardsLong(){

  return 1;
}

/*
 * Misc functions
 * You need to give at least a comment here.  Hopefully many more
 */
int detectLightInit(){//Detects the level of light and hopefully determines if we're in the right room
  if(true){//TODO holder value.

    return -1;
  }
  else{
     isDarkRoom=true;
    return 1;
  }
}

bool tapePresent(){
  //Detects whether or not there's tape on the ground

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
