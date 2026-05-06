// TB6612FNG Pin,ESP32 GPIO,Description
// PWMA,GPIO 25,Speed Control for Motor A
// AIN1,GPIO 26,Direction 1 for Motor A
// AIN2,GPIO 27,Direction 2 for Motor A
// PWMB,GPIO 14,Speed Control for Motor B
// BIN1,GPIO 19,Direction 1 for Motor B (Safe Pin)
// BIN2,GPIO 18,Direction 2 for Motor B
// STBY,GPIO 5,Standby (Must be HIGH to run)
// VCC,3.3V,Logic Power (From ESP32)
// GND,GND,Logic Ground (Common Ground)
#include <WiFi.h>
#include <WebServer.h>

// --- Motor Pin Definitions ---
const int PWMA = 25; const int AIN1 = 26; const int AIN2 = 27;
const int PWMB = 14; const int BIN1 = 12; const int BIN2 = 18;
const int STBY = 5;

// --- WiFi Configuration ---
const char* ssid = "Xypher_Bot_Control";
const char* password = "password123";

WebServer server(80);

// PWM Settings for ESP32 Core 3.0+
const int freq = 5000;
const int res = 8;
int currentSpeed = 220; // Default drive speed (0-255)

// --- Polished UI Dashboard ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<style>
  body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background: #1a1a1a; color: white; margin: 0; padding-top: 50px; }
  h2 { color: #00ff00; letter-spacing: 2px; text-transform: uppercase; }
  .controller { display: grid; grid-template-columns: 100px 100px 100px; grid-template-rows: 100px 100px 100px; gap: 15px; justify-content: center; margin-top: 30px; }
  .btn { background: #333; color: white; border: 2px solid #444; border-radius: 12px; font-weight: bold; font-size: 14px; cursor: pointer; transition: 0.1s; display: flex; align-items: center; justify-content: center; text-transform: uppercase; -webkit-user-select: none; }
  .btn:active { background: #00ff00; color: black; border-color: #00ff00; transform: scale(0.95); box-shadow: 0 0 15px #00ff00; }
  .fwd { grid-column: 2; }
  .lft { grid-column: 1; grid-row: 2; }
  .stop { grid-column: 2; grid-row: 2; background: #b30000; border-color: #ff3333; color: white; }
  .rgt { grid-column: 3; grid-row: 2; }
  .bwd { grid-column: 2; grid-row: 3; }
</style></head>
<body>
  <h2>Rover Control</h2>
  <div class="controller">
    <div class="btn fwd" onmousedown="move('F')" onmouseup="move('S')" ontouchstart="move('F')" ontouchend="move('S')">Forward</div>
    <div class="btn lft" onmousedown="move('L')" onmouseup="move('S')" ontouchstart="move('L')" ontouchend="move('S')">Left</div>
    <div class="btn stop" onclick="move('S')">Stop</div>
    <div class="btn rgt" onmousedown="move('R')" onmouseup="move('S')" ontouchstart="move('R')" ontouchend="move('S')">Right</div>
    <div class="btn bwd" onmousedown="move('B')" onmouseup="move('S')" ontouchstart="move('B')" ontouchend="move('S')">Backward</div>
  </div>
  <p style="margin-top:40px; color:#888;">IP: 192.168.4.1</p>
  <script>
    function move(dir) { fetch('/action?dir=' + dir); }
  </script>
</body></html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // Pin setup
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  // PWM setup (Core 3.0+)
  ledcAttach(PWMA, freq, res);
  ledcAttach(PWMB, freq, res);

  // Start Access Point
  WiFi.softAP(ssid, password);
  Serial.print("Started. Connect to: "); Serial.println(ssid);
  Serial.print("IP: "); Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send(200, "text/html", index_html); });
  server.on("/action", handleAction);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleAction() {
  String dir = server.arg("dir");
  if (dir == "F") drive(currentSpeed, currentSpeed);
  else if (dir == "B") drive(-currentSpeed, -currentSpeed);
  else if (dir == "L") drive(-currentSpeed, currentSpeed);
  else if (dir == "R") drive(currentSpeed, -currentSpeed);
  else drive(0, 0);
  server.send(200, "text/plain", "OK");
}

void drive(int speedA, int speedB) {
  // Motor A Logic
  digitalWrite(AIN1, (speedA > 0));
  digitalWrite(AIN2, (speedA < 0));
  ledcWrite(PWMA, abs(speedA));

  // Motor B Logic
  digitalWrite(BIN1, (speedB > 0));
  digitalWrite(BIN2, (speedB < 0));
  ledcWrite(PWMB, abs(speedB));
}
