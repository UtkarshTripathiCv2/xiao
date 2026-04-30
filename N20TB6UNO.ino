// Pin Definitions
const int AIN1 = 4;
const int AIN2 = 5;
const int PWMA = 3;  // PWM Pin
const int STBY = 6;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Enable the driver
  digitalWrite(STBY, HIGH);
}

void loop() {
  // Move Forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 200); // Speed 0-255
  
  delay(2000);

  // Brake
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  delay(1000);

  // Move Backward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 150);
  
  delay(2000);

  // Standby (Complete Power Down)
  digitalWrite(STBY, LOW);
  delay(1000);
  digitalWrite(STBY, HIGH);
}
