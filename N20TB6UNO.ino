/*
TB6612FNG Pin,Arduino Uno Pin,Purpose
VM,12V Positive (+),Motor Power
VCC,5V Pin,Logic Power
GND,GND (Common),Connect Battery (-) and Uno GND
PWMA,Pin 3,Motor A Speed (PWM)
AIN1 / AIN2,Pin 4 / Pin 5,Motor A Direction
PWMB,Pin 9,Motor B Speed (PWM)
BIN1 / BIN2,Pin 7 / Pin 8,Motor B Direction
STBY,Pin 6,Standby (Set HIGH to run)
AO1 / AO2,Motor A terminals,N20 Motor Left
BO1 / BO2,Motor B terminals,N20 Motor Right
 * TB6612FNG Dual Motor Control - Arduino Uno
 * Controls two 12V N20 Gear Motors
 */

// Motor A (Left)
const int AIN1 = 4;
const int AIN2 = 5;
const int PWMA = 3;

// Motor B (Right)
const int BIN1 = 7;
const int BIN2 = 8;
const int PWMB = 9;

// Common Standby Pin
const int STBY = 6;

void setup() {
  // Initialize all pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Enable the motor driver
  digitalWrite(STBY, HIGH);
  
  Serial.begin(9600);
  Serial.println("Uno Motor Control Ready");
}

void loop() {
  Serial.println("Moving Forward...");
  moveForward(200);
  delay(2000);

  Serial.println("Turning Left...");
  turnLeft(180);
  delay(1000);

  Serial.println("Stopping (Braking)...");
  stopMotors();
  delay(2000);
}

// --- Control Functions ---

void moveForward(int speed) {
  // Motor A Forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);

  // Motor B Forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed);
}

void turnLeft(int speed) {
  // Motor A Backward, Motor B Forward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, speed);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed);
}

void stopMotors() {
  // Short Brake: Sets both pins HIGH to lock motors
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}
