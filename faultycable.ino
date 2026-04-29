#include <Arduino.h>

// On XIAO ESP32S3, GPIO 1 is connected to a voltage divider for the battery/bus
const int vBatPin = 1; 

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  // Configure ADC for the XIAO S3 range
  analogReadResolution(12); 
  
  Serial.println("--- XIAO S3 POWER MONITOR STARTING ---");
}

void loop() {
  // Read the raw ADC value
  int rawValue = analogRead(vBatPin);
  
  // Convert to millivolts using the 12-bit range and internal divider
  // The XIAO has an internal 1/2 divider (two 1M ohm resistors)
  // Voltage = (ADC_Value / 4095) * Vref * 2 (for divider)
  uint32_t mv = analogReadMilliVolts(vBatPin) * 2;

  Serial.print("Current Bus Voltage: ");
  Serial.print(mv);
  Serial.println(" mV");

  if (mv < 3100) {
    Serial.println(">>> WARNING: CRITICAL VOLTAGE DROP! CABLE OR PORT IS WEAK <<<");
  } else if (mv > 4300) {
    Serial.println("Status: USB Power detected (Charging/Bus Level)");
  } else {
    Serial.println("Status: Stable 3.3V Rail Operation");
  }

  Serial.println("------------------------------------");
  delay(1000); // Check every second
}
