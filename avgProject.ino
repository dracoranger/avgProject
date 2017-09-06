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

  #include <Servo.h>
Servo Lservo;  // create servo object to control a servo
Servo Rservo;

int i = 0;  // analog pin used to connect the potentiometer
const int dur = 10;    // variable to read the value from the analog pin

void setup() {
  Lservo.attach(12);  // attaches the servo on pin 9 to the servo object
  Rservo.attach(13);
}

void loop() {                 // sets the servo position according to the scaled value
  forward();
  delay(6000);
  delay(1500);
  reverse();
  delay(6000);
  stopRobot();
}

//Motion routines for robot
void forward() {
  Lservo.writeMicroseconds(1250);
  Rservo.writeMicroseconds(1250);
}

void reverse() {
  Lservo.writeMicroseconds(1650);
  Rservo.writeMicroseconds(1650);
}

void stopRobot() {
  Lservo.detach();
  Rservo.detach();
}

*/


#include <Servo.h>

// this constant won't change. It's the pin number of the sensor's output:
//Probably should capitalize
const int pingFront = 7;
const int pingSide = 8;
const int WHISKER_LEFT = 2;
const int WHISKER_RIGHT = 3;
const int LIGHT_SENSOR = 5;
const int TAPE_DETECT= 9;
const int IR_LED=6;

// Sets up bool for light
bool isDarkRoom=false;

//sets up whether or not was on wall
//used to differentiate between the wall and random obsticles
int byWall=0;
bool scanningLeft=false;


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
  pinMode(WHISKER_LEFT,INPUT);
  pinMode(WHISKER_RIGHT,INPUT);
  pinMode(IR_LED,OUTPUT);
  right.attach(10);   // attaches the servo on pin 10 to the servo object
  left.attach(11);
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

void loop() {
  /*
   * Deal with Everything else
   * TODO: whiskers, lighting, space out the delayMicrosecond?
   * 8/29 moved before AI to make sure that AI has the ability to use set vaiables and it compiles, which isn't necessarily unexpected
   *
  */

  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:

  long duration, inchesF, cmF, inchesS, cmS;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(IR_LED, HIGH);
  delay(.25);
  digitalWrite(IR_LED, LOW);
  delay(.25);
  digitalWrite(IR_LED, HIGH);
  delay(.25);
  digitalWrite(IR_LED, LOW);
  delay(.25);
  digitalWrite(IR_LED, HIGH);
  delay(.25);
  digitalWrite(IR_LED, LOW);
  delay(.25);
  tapePresent();

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

  // Whisker alert Done in functions below and static

  //TODO: initialize light detection

  Serial.print("IR input Front: ");
  Serial.print(digitalRead(LIGHT_SENSOR));
  Serial.print(" IR input Side: ");
  Serial.print(digitalRead(TAPE_DETECT));
  Serial.print(" ");
  Serial.print(inchesF);
  Serial.print("in, ");
  Serial.print(cmF);
  Serial.print("cm. WhiskerLeft:");
  Serial.print(whiskerLeft());
  Serial.print(" whiskerRight");
  Serial.print(whiskerRight());
  Serial.println();

  delay(100);//Do we really want a serious delay?
             //Place detection into its own function and just run it every 100ms?
   /*
    * AI
    * The set behaviors of the robot are designated here
    *
    */
    //TEMPS while not implemented
    //bool tapePresent = false;


  if(whiskerLeft()||whiskerRight()){
    //STOP AND REVERSE, whiskers detected something's right in front of us
    //Turn as well?  Don't want to get stuck in an infinite loop
    driveBackwards();
    if(WHISKER_LEFT){
      shiftRight();
      shiftRight();
      shiftRight();
      shiftRight();
    }
    else{
      shiftLeft();
      shiftLeft();
      shiftLeft();
      shiftLeft();
    }
  }
  else if(isDarkRoom){
    //Head Towards the Light
    //deal with obsticles
    //basically recreate AI within this section, keeping the about to crash paramount
    //use light in front as director.  Need to worry about being attracted to the light from the door

    //might have an issue with the search function.  Not entirely sure how to implement
    //Might want to create a function before this which detects something in front and to the Side
    //if true, turn to 225 or so and go forward, so diagional round the room
    int curr=0;
    while(detectLightInit()!=1|| curr!=18){
      if(scanningLeft){
        shiftLeft();
      }
      else{
        shiftRight();
      }
      curr++;
    }
    if(detectLightInit()==1){//Not sure if this will keep it centered on the light
      driveForward;
    }
    else{
      if(scanningLeft){
        shiftRight();
        shiftRight();
        shiftRight();
        shiftRight();
      }
      else{
        shiftLeft();
        shiftLeft();
        shiftLeft();
        shiftLeft();
      }
      driveForward();
      scanningLeft= !scanningLeft;
    }
  }
  else if(inchesS-CLO>FAR&&byWall>WALL_SAFE&&!tapePresent()){
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
      driveForwardLong();
    }
  }
  else if(inchesF>FAR_FRONT&&inchesS>FAR_SIDE&&byWall==0){
    //in the middle of the room Go forward
    driveForward();
  }
  else if(inchesF<FAR_FRONT && byWall==0){
    //reached the wall for the first time, turn left
    //Need to alter based on where turning brings us in relation to a wall
    driveForward();
    turnLeft();
    driveForward();
    byWall=byWall+1;
  }
  else if(inchesF>FAR_FRONT && ((inchesS>CLO && inchesS<FAR)||tapePresent())){
    //discovered the wall, continue along it in relative safety
    //increase byWall
    driveForward();
    byWall=byWall+1;
  }
  else if(inchesF>FAR_FRONT && inchesS<CLO){
    //discovered the wall, continue along it, shift to the left to prevent running into it
    shiftLeft();
    driveForward();
    byWall=byWall+1;
  }
  else if(inchesF>FAR_FRONT && inchesS>FAR && inchesS<FAR_SIDE){
    //discovered the wall, continue along it, shifted to the right to prevent driving too far away
    shiftRight();
    driveForward();
    byWall=byWall+1;
  }
  else if(inchesF<FAR_FRONT && ((inchesS>CLO && inchesS<FAR)||tapePresent()) && byWall> WALL_SAFE){
    //discovered a turn, so lets turn
    turnLeft();
    driveForward();
    byWall=byWall+1;
  }
  else{
    //we suddenly found a large gap or something early on.
    //could be a door or could just have been a large obsticle.  Nothing's definite at the moment.
    //think reset byWall to 0 and basically act as if we are just entering the room for the first time.
    //Really need to test this, probably the best idea to revert to careful without assumptions in this case.
    //May want to check how much byWall there is, because we could lose a good amount of certainty otherwise.
    byWall=0;
    driveForward();
  }
 }


/*
 * Controller functions
 * Names should be self explanatory.
 * They're designed to be opposites of each other to allow for some level of control
 */

 const int lTime = 6000;
 const int sTime= lTime/4;

int turnRight(){
  for(int i=0; i<T_90_TURN; i++){
    right.writeMicroseconds(2000);
    left.writeMicroseconds(1000);
  }
  return 1;
}

int shiftRight(){
  for(int i=0; i<T_90_TURN/9; i++){
    right.writeMicroseconds(2000);
    left.writeMicroseconds(1000);
  }
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
  for(int i=0; i<T_90_TURN/9; i++){
    right.writeMicroseconds(1000);
    left.writeMicroseconds(2000);
  }
  return 1;
}

int driveForward(){
  for(int i=0; i<T_1_FOOT; i++){
    right.writeMicroseconds(1000);
    left.writeMicroseconds(1000);
  }
  return 1;
}
int driveForwardLong(){
  for(int i=0; i<T_1_FOOT*2; i++){
    right.writeMicroseconds(1000);
    left.writeMicroseconds(1000);
  }
  return 1;
}
int driveForward4Ft(){
  for(int i=0; i<T_4_FEET; i++){
    right.writeMicroseconds(1000);
    left.writeMicroseconds(1000);
  }
  return 1;
}
int driveBackwards(){
  for(int i=0; i<T_1_FOOT; i++){
    right.writeMicroseconds(2000);
    left.writeMicroseconds(2000);
  }
  return 1;
}
int driveBackwardsLong(){
  for(int i=0; i<T_1_FOOT*2; i++){
    right.writeMicroseconds(2000);
    left.writeMicroseconds(2000);
  }
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
  int input=digitalRead(TAPE_DETECT);
  if(input==0){
    //Light detected, no tape
    return false;
  }
  else{
    return true;
  }
}

bool whiskerLeft(){
  if(digitalRead(WHISKER_LEFT)==1){
    return true;
  }
  else{
    return false;
  }

}

bool whiskerRight(){
  if(digitalRead(WHISKER_RIGHT)==1){
    return true;
  }
  else{
    return false;
  }
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
