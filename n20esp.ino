// Motor A Pins
const int PWMA = 25; 
const int AIN1 = 26;
const int AIN2 = 27;

// Motor B Pins
const int PWMB = 14;
const int BIN1 = 12;
const int BIN2 = 18; 

const int STBY = 5;

// New PWM Settings
const int freq = 5000;
const int res = 8; // 8-bit resolution (0-255)

void setup() {
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // NEW ESP32 3.0 API: ledcAttach(pin, frequency, resolution)
  // No need for separate channel numbers anymore!
  ledcAttach(PWMA, freq, res);
  ledcAttach(PWMB, freq, res);

  digitalWrite(STBY, HIGH); 
}

void loop() {
  drive(200, 200); 
  delay(2000);
  drive(0, 0);
  delay(1000);
}

void drive(int speedA, int speedB) {
  // Motor A
  if (speedA > 0) {
    digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW);
  } else if (speedA < 0) {
    digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);
  }
  // NEW API: ledcWrite(pin, value) instead of channel
  ledcWrite(PWMA, abs(speedA));

  // Motor B
  if (speedB > 0) {
    digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
  } else if (speedB < 0) {
    digitalWrite(BIN1, LOW); digitalWrite(BIN2, HIGH);
  } else {
    digitalWrite(BIN1, LOW); digitalWrite(BIN2, LOW);
  }
  ledcWrite(PWMB, abs(speedB));
}
