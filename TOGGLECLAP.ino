#include <I2S.h>

const int threshold = 30000; // Adjust this based on Serial Monitor readings
const int ledPin = 21; 
const int sampleWindow = 128; // Number of samples to check per loop

bool ledState = false; 

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Start OFF

  I2S.setAllPins(-1, 42, 41, -1, -1); 
  
  if (!I2S.begin(PDM_MONO_MODE, 16000, 16)) {
    Serial.println("Failed to initialize I2S!");
    while (1);
  }
  Serial.println("Stable Clap Toggle Ready!");
}

void loop() {
  int maxVolume = 0;

  // Collect a batch of samples to find the peak volume
  for (int i = 0; i < sampleWindow; i++) {
    int sample = I2S.read();
    if (sample != 0 && sample != -1) {
      int volume = abs(sample);
      if (volume > maxVolume) {
        maxVolume = volume;
      }
    }
  }

  // Only trigger if the loudest peak in that window exceeds the threshold
  if (maxVolume > threshold) {
    ledState = !ledState; // Toggle state

    Serial.print("Clap Detected! Peak: ");
    Serial.print(maxVolume);
    Serial.println(ledState ? " -> LED ON" : " -> LED OFF");

    digitalWrite(ledPin, ledState ? LOW : HIGH); 
    
    // Increased delay to 500ms to ignore echoes/vibrations
    delay(500); 
    
    // Clear the I2S buffer to prevent "leftover" noise from triggering again
    while(I2S.read() != -1); 
  }
}
