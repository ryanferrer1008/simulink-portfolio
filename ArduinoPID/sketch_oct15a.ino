#include <PID_v1.h> //Remember that to use the library inside the documents ->"create a folder called (library)" -> insert the library you want to use in your arduino
//We use the library
// Ultrasonic (front and left)
const int ECHO_PIN = 3;
const int TRIG_PIN = 4;

// Motors: A = right, B = left
const int A_1A = 9;    // RIGHT fwd (PWM)
const int A_1B = 6;    // RIGHT rev
const int B_1A = 11;   // LEFT  rev
const int B_1B = 10;   // LEFT  fwd (PWM)

// PID
double Setpoint = 12.0;      // cm
double Input = 0, Output = 0;
double Kp = 7, Ki = 0.05, Kd = 8;   //CHANGE THE VALUES HERE
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);

// Speed policy
const int baseSpeed = 120;   // left motor constant speed
const int maxAdjust = 100;   // right motor adjustment range

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(A_1A, OUTPUT); pinMode(A_1B, OUTPUT);
  pinMode(B_1A, OUTPUT); pinMode(B_1B, OUTPUT);

  myPID.SetSampleTime(50);         
  myPID.SetOutputLimits(-maxAdjust, maxAdjust); //here is between -100 to 100
  myPID.SetMode(AUTOMATIC);

  // left motor constant forward
  analogWrite(B_1A, 0);
  analogWrite(B_1B, baseSpeed);
}

void loop() {
  //This is for the ultrasonic reading
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long us = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (us == 0) {
    // bad read: go straight at base speed this cycle
    analogWrite(A_1B, 0); 
    analogWrite(A_1A, baseSpeed);
    analogWrite(B_1A, 0); 
    analogWrite(B_1B, baseSpeed);
    delay(50);
    return;
  }

  double cm = us / 58.0;
  
  if (cm < 3 || cm > 300) {
    analogWrite(A_1B, 0); 
    analogWrite(A_1A, baseSpeed);
    analogWrite(B_1A, 0); 
    analogWrite(B_1B, baseSpeed);
    delay(50);
    return;
  }

  Input = cm;

  //PID & apply to RIGHT motor only
  myPID.Compute();
  int rightPWM = baseSpeed + (int)Output;
  if (rightPWM < 0) rightPWM = 0;
  if (rightPWM > 255) rightPWM = 255;

  analogWrite(A_1B, 0);
  analogWrite(A_1A, rightPWM);

  //keep left motor at base
  analogWrite(B_1A, 0);
  analogWrite(B_1B, baseSpeed);
}
