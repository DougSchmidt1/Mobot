//       ^
// Frwd  |
//     __|__
//    [     ]
// []-[     ]-{}
// LM [__.__] RM
//     
# include <memory>
# include <Servo.h>
// Define stepper motor connections and steps per revolution:
int setSpeed = 1000;
// Setup servo sweep
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
int backpos = 0;
float duration, distance, range;
const int trigPin = 2;
const int echoPin = 3;
double once = 2.25;

// RIGHT Motor connections
int enR = 12;
int R1 = 11;
int R2 = 10;
// LEFT Motor connections
int enL = 7;
int L1 = 9;
int L2 = 8;
int pwmSpeedR = 255;
//170;
int pwmSpeedL = 255;
int pwmMin = 128;
int dirEnum = 1;

class ENVIRONMENT {
  public:
    //measurements
    double range = 0.0;
    double rangeSweep [180] = {0};
    //double range [5] = {0, 0, 0, 0, 0}
};

class MOBOT {
    public:
        //measurements in mm
        int length = 255;
        int bodywidth = 205;
        int height = 160;
        int width = 275;

        void RangeToBlocker(int distance){   
            // Ideally this will acept the servo angle parameters to do a trigonometric range approximation
            // Right now, this assumed the ultrasonic sensor is pointed exactly forward
            
            if (distance < length){
              // Look servo sweep
              // Review Ultrasonic messurement
              // Process photo if applicable
              //turnLeft(1);
            } else {
              double LUs = distance / length;
              //moveForward(LUs);
              }                
                
        }
        double calculateVolume(){   
            return length * bodywidth * height;
        }
};

ENVIRONMENT thisENV;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  // Turn off motors - Initial state
  digitalWrite(enR, 0);
  digitalWrite(enL, 0);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  myservo.attach(5);
  Serial.begin(115200);
}

void loop() {
  delay(1000);
  
  
  dirEnum = pickDirection();
  //lookSweep();
  // Foward
  // if ( dirEnum == 1){
  //   moveForward(3);
  // // Left
  // } else if (dirEnum == 0){
  //   turnLeft(once);
  // } else if (dirEnum == 2){
  //   turnRight(once);
  // } else{
  //   moveBackward(3);
  // }
  delay(1000);
}

double getRange(){
  double sum = 0;
  //Serial.println();
  // Average 3 measurements as a dumb smoothing filter
  // for ( int i = 0; i < 2; i++ ){
  //   digitalWrite(trigPin, LOW);
  //   delayMicroseconds(2);
  //   digitalWrite(trigPin, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(trigPin, LOW);
  //   duration = pulseIn(echoPin, HIGH);
  //   distance = (duration*.0343)/2.0;
  //   if (distance = 0){
  //     break;
  //   }
    // Serial.print("Meas");
    // Serial.print(i+1);
    // Serial.print(": ");
    // Serial.print(distance);
    // Serial.println("cm");
    // Serial.print("Meas");
    // Serial.print(i);
    // Serial.print(": ");
    // Serial.print(distance); //distance);
    // Serial.println("cm");
    //sum += distance;
    //delay(10);
  //}
  //range = sum / 2.0000;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2.0;
  Serial.print("Distance: ");
  Serial.print(distance); //distance);
  Serial.println("cm");
  return distance;
}

void lookAngle(int lookPos) {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
       // in steps of 1 degree
       myservo.write(lookPos);              // tell servo to go to position in variable 'pos'
       delay(20);                       // waits 15ms for the servo to reach the position
  }
    delay(250);
}

void lookSweepMeasure(){
  for (pos = 0; pos <= 179; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      thisENV.rangeSweep[ pos ] = getRange(); 
      Serial.println(thisENV.rangeSweep[pos]);
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(200);
  Serial.println(pos);
  for (pos = 179; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                // tell servo to go to position in variable 'pos'
    thisENV.rangeSweep[ pos ] = (thisENV.rangeSweep[ pos ] + getRange() ) / 2.0;
    Serial.println(thisENV.rangeSweep[pos]);
    myservo.write(pos);  
    delay(20);                       // waits 15ms for the servo to reach the position
  }
}

void lookSweep() {
    for (pos = 0; pos <= 179; pos += 1) { // goes from 0 degrees to 180 degrees
       // in steps of 1 degree
       myservo.write(pos);              // tell servo to go to position in variable 'pos'
       delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(500);

  for (pos = 179; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
delay(500);
}

int stepCalc(double in){
  int t = in*50;
  if (t > 50){
    return t;
  } else{
    return 50;
  }
}

int pickDirection(){
  lookAngle(90);
  
  lookSweepMeasure();
  Serial.print("Range sweep: ");
  Serial.print(thisENV.rangeSweep[0]); 
  Serial.print(": ");
  Serial.print(thisENV.rangeSweep[45]);
  Serial.print(": ");
  Serial.print(thisENV.rangeSweep[90]);
  Serial.print(": ");
  Serial.print(thisENV.rangeSweep[135]);
  Serial.print(": ");
  Serial.println(thisENV.rangeSweep[179]);
  return 1;
}

int delayCalc(double in){
  int t = in*1000;
  return t;
}

void motorsOff(){
   // Now turn off motors
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
}

void accelerateTo( int pwmLeft, 
                   int pwmRight ){
    int minVal = min(pwmLeft, pwmRight);
    for (int i = pwmMin; i < minVal ; i++) {
    analogWrite(enR, i);
    analogWrite(enL, i);
    delay(20);
   }
}
void moveForward(double LUs){
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
    // Accelerate from zero to maximum speed
  //accelerateTo( pwmSpeedL, pwmSpeedR );
  analogWrite(enR, pwmSpeedR);
  analogWrite(enL, pwmSpeedL);
  // Turn on motor A & B
  //int t = delayCalc(LUs) / 100;
  int full = stepCalc(LUs);
  //bool skipright = false;
  // Because these motors are so variable, 
  // Its best to apply another 50% duty cycle to slow the speed
  for ( int i = 0; i<full; i++){
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
    //if (skipright){
    //  skipright = false;
    //} else{
    //  skipright = true;
    //  digitalWrite(R1, LOW);
    //  digitalWrite(R2, LOW);
    //}
    delay(10);
    //delay( t );
    motorsOff();
    delay(10);
  }
}

void moveBackward(double LUs){
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enR, pwmSpeedR);
  analogWrite(enL, pwmSpeedL);
  // Turn on motor A & B
  
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  delay( delayCalc(LUs) );
  motorsOff();  
}

void turnRight(double LUs){
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enR, pwmSpeedR);
  analogWrite(enL, pwmSpeedL);
  // Turn on motor A & B
  int full = stepCalc(LUs);
  for ( int i = 0; i<full; i++){
     digitalWrite(R1, LOW);
     digitalWrite(R2, HIGH);
     digitalWrite(L1, LOW);
     digitalWrite(L2, HIGH);
     delay(10);
     motorsOff();
     delay(10);
  }
}  

void turnLeft(double LUs){
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enR, pwmSpeedR);
  analogWrite(enL, pwmSpeedL);
  int full = stepCalc(LUs);
  for ( int i = 0; i<full; i++){
     digitalWrite(R1, HIGH);
     digitalWrite(R2, LOW);
     digitalWrite(L1, HIGH);
     digitalWrite(L2, LOW);
     delay(10);
     motorsOff();
     delay(10);
   }
}

// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enR, pwmSpeedR);
  analogWrite(enL, pwmSpeedL);

  // Turn on motor A & B
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  delay(2000);
  
  // Now change motor directions
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  delay(2000);
  
  // Turn off motors
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enR, i);
    analogWrite(enL, i);
    delay(20);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(enR, i);
    analogWrite(enL, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
}