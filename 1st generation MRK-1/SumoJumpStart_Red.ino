//This JumpStart Code is for the black Sumo Robot League circuit board
//Please use  if your circuit board is red
/******     Authors - Luke Stephens & William J Ashby, PhD  ********
Our code is open source, MIT License (details at the end).
Please use our code in your projects and tell your friends about us!
The MRK-2 robot kit is available from http://www.SumoRobotLeague.com
********************************************************************/

// use these if your circuitboard is red
#define pingPin 3
#define echoPin A0
#define leftSensor A1
#define rightSensor A2
#define rearSensor A3
#define buttonPin 2
#define leftDirection 4
#define buzzerPin 5
#define IREmitter 6
#define LED 7
#define rightDirection 8
#define leftMotorSpeed 9
#define rightMotorSpeed 10
//*/

bool leftIsFlipped = false; //If the left motor goes backwards when it is supposed to go forwards set to true
bool rightIsFlipped = true; 
int lBlack = 800; int rBlack = 800; //default black and white values
int lWhite = 100; int rWhite = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightDirection, OUTPUT);
  pinMode(leftMotorSpeed, OUTPUT);
  pinMode(rightMotorSpeed, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(IREmitter, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  calibrateBlackWhite(); 
  // this calibrate function reuires pressing the pushbutton to calibrate black and then again for white
}

void loop() {
  // put your main code here, to run repeatedly

}


// below here are the functions to help you get up and running fast.  Feel free to look around but making changes could break your code.

void drive(int x) {
  // map converts [-100,100] to [-255,255]
  int spd = map(x, -100, 100, -255, 255);
  setSpeeed(spd, 0);
  setSpeeed(spd, 1);
}

void setSpeeed(int sped, int motor) {
  int speedPin, directionPin;
  bool flippedPin;
  if ( motor == 0 ) {
    speedPin = leftMotorSpeed;
    directionPin = leftDirection;
    flippedPin = leftIsFlipped;
  } else if ( motor == 1 ) {
    speedPin = rightMotorSpeed;
    directionPin = rightDirection;
    flippedPin = rightIsFlipped;
  }

  if ( sped == 0 ) {
    analogWrite(speedPin, 0);
    digitalWrite(directionPin, LOW);
  } else if ( (sped > 0 && !flippedPin) || (sped < 0 && flippedPin) ) {
    digitalWrite(directionPin, HIGH);
    analogWrite(speedPin, abs(sped));
  } else if ( (sped < 0 && !flippedPin) || (sped > 0 && flippedPin) ) {
    digitalWrite(directionPin, LOW);
    analogWrite(speedPin, abs(sped));
  }
}

void turnDegrees(double y) {
  bool lWheel = 0;
  bool rWheel = 0;

  if (y > 0) {
    lWheel = 0;
    rWheel = 1;
    if (leftIsFlipped) {
      lWheel = 1;
    }
    if (rightIsFlipped) {
      rWheel = 0;
    }
    digitalWrite(leftDirection, lWheel); analogWrite(leftMotorSpeed, 255);
    digitalWrite(rightDirection, rWheel); analogWrite(rightMotorSpeed, 255);
    delay(y * 6.21111);
    analogWrite(leftMotorSpeed, 0);
    analogWrite(rightMotorSpeed, 0);
  } else if (y < 0) {
    lWheel = 1;
    rWheel = 0;
    if (leftIsFlipped) {
      lWheel = 0;
    }
    if (rightIsFlipped) {
      rWheel = 1;
    }
    digitalWrite(leftDirection, lWheel); analogWrite(leftMotorSpeed, 255);
    digitalWrite(rightDirection, rWheel); analogWrite(rightMotorSpeed, 255);
    delay(abs(y * 6.21111));
    analogWrite(leftMotorSpeed, 0);
    analogWrite(rightMotorSpeed, 0);
  } else Serial.println("0 degree turn");
}

boolean objectWithin(int cm) {
  return getDistance() < cm;
}

int getDistance() {
  long duration;
  int distance;
  digitalWrite(pingPin, LOW);// Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (int) ( (duration / 2) / 29.1 );
  return distance;
}

//returns true if either front IR sensor detects white
//uses the thresholds for the left and right front IR sensors that
//are calculated via the calibrateBlackWhite function
boolean isWhite() {
  digitalWrite(IREmitter, HIGH);
  int left = analogRead(leftSensor);
  Serial.print("left: "); Serial.println(left);
  int right = analogRead(rightSensor);
  Serial.print("right: "); Serial.println(right);
  digitalWrite(IREmitter, LOW);

  int lThresh = (lWhite + ((lBlack - lWhite) / 2));
  Serial.print("lThresh: "); Serial.println(lThresh);
  int rThresh = (rWhite + ((rBlack - rWhite) / 2));
  Serial.print("rThresh: "); Serial.println(rThresh);

  if (left < lThresh || right < rThresh) {
    return true;
  } else return false;
}

//averages 5 black and 5 white IR readings for each front IR sensor
//then calculates the threshold between the balck and white readings
void calibrateBlackWhite() {
  int n = 5;
  int lBlacks[n]; int rBlacks[n];
  int lWhites[n]; int rWhites[n];
  tone(buzzerPin, 800, 1000);
  Serial.println("place front sensors on black and press button");
  while (digitalRead(buttonPin) == 1) {
    delay(100);
  }

  digitalWrite(IREmitter, HIGH);
  for (int i = 0; i < n; i++) {
    lBlacks[i] = analogRead(leftSensor);
    rBlacks[i] = analogRead(rightSensor);
    Serial.print("lBlacks: ");
    Serial.print(lBlacks[i]);
    Serial.print(" rBlacks: ");
    Serial.println(rBlacks[i]);
  }
  digitalWrite(IREmitter, LOW);
  tone(buzzerPin, 1800, 1000);
  Serial.println("place front sensors on white and press button");
  delay(1000);
  while (digitalRead(buttonPin) == 1) {
    delay(100);
  }

  digitalWrite(IREmitter, HIGH);
  for (int i = 0; i < n; i++) {
    lWhites[i] = analogRead(leftSensor);
    rWhites[i] = analogRead(rightSensor);
    Serial.print("lWhites: ");
    Serial.print(lWhites[i]);
    Serial.print(" rWhites: ");
    Serial.println(rWhites[i]);
  }
  digitalWrite(IREmitter, LOW);

  lBlack = 0;  //delete the default values
  rBlack = 0;  //set on lines 27 and 28
  lWhite = 0;
  rWhite = 0;  
  for (int i = 0; i < n; i++) {
    lBlack += lBlacks[i];
    rBlack += rBlacks[i];
    lWhite += lWhites[i];
    rWhite += rWhites[i];
  }
  lBlack = lBlack / n; rBlack = rBlack / n; //average black and white values
  lWhite = lWhite / n; rWhite = rWhite / n;
  Serial.print("average of left blacks: "); Serial.println(lBlack);
  Serial.print("average of right blacks: "); Serial.println(rBlack);
  Serial.print("average of left whites: "); Serial.println(lWhite);
  Serial.print("average of right whites: "); Serial.println(rWhite);

  delay(1000);
}
