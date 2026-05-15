#include <I2S.h>

// Adjust this value based on your room's noise level
// Higher = less sensitive, Lower = more sensitive
const int threshold = 8000; 
const int ledPin = 21; 

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Start with LED OFF

  // Configure XIAO ESP32-S3 Sense Microphone pins
  I2S.setAllPins(-1, 42, 41, -1, -1); 
  
  if (!I2S.begin(PDM_MONO_MODE, 16000, 16)) {
    Serial.println("Failed to initialize I2S!");
    while (1);
  }
  
  Serial.println("Microphone ready. Speak or clap to trigger LED!");
}

void loop() {
  int sample = I2S.read();

  // I2S.read() returns 0 or -1 if no data is ready
  if (sample != 0 && sample != -1) {
    
    // Use absolute value because audio waves go positive and negative
    int volume = abs(sample);

    if (volume > threshold) {
      Serial.print("Triggered! Volume: ");
      Serial.println(volume);
      
      digitalWrite(ledPin, LOW);  // Turn LED ON
      delay(500);                 // Keep it on for half a second
      digitalWrite(ledPin, HIGH); // Turn LED OFF
    }
  }
}
