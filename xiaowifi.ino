#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "eis";
const char* password = "ack333.";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n--- XIAO ESP32S3 Wi-Fi Test ---");
  
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Use a timer to see how long it takes
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\nConnection Failed. Check credentials or battery power.");
  }
}

void loop() {
  // Just stay connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection lost. Reconnecting...");
    WiFi.begin(ssid, password);
  }
  delay(5000);
}
