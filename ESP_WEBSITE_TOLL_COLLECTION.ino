#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// ---- soft‑AP credentials ----
const char* apSSID = "ESP_Recharge";
const char* apPsw = ""; // open network (no password)
// ---- web server on port 80 ----
ESP8266WebServer server(80);
// HTML form (same as before)
const char FORM[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>RFID Recharge</title></head>
<body style="font-family: sans-serif; text-align:center;">
<h2>Recharge RFID Card</h2>
<form action="/recharge" method="GET">
<label>UID:</label><br>
<input type="text" name="uid" required><br><br>
<label>Amount:</label><br>
<input type="number" name="amount" required><br><br>
<input type="submit" value="Recharge">
</form>
</body>
</html>
)rawliteral";
void setup() {
Serial.begin(9600); // to Arduino UNO on pins 0/1
delay(100);
// 1) start SoftAP
WiFi.mode(WIFI_AP);
WiFi.softAP(apSSID, apPsw);
IPAddress IP = WiFi.softAPIP(); // default is 192.168.4.1
Serial.print("AP IP address: ");
Serial.println(IP);
// 2) set up HTTP routes
server.on("/", HTTP_GET, []() {
server.send_P(200, "text/html", FORM);
});
server.on("/recharge", HTTP_GET, []() {
if (server.hasArg("uid") && server.hasArg("amount")) {
String uid = server.arg("uid");
String amount = server.arg("amount");
String msg = uid + "," + amount + "\n";
Serial.print(msg); // send to Arduino
server.send(200, "text/html",
"<h3>Sent:</h3><pre>" + msg + "</pre><a href='/'>Back</a>");
} else {
server.send(400, "text/html", "<h3>Missing UID or amount</h3><a href='/'>Back</a>");
}
});
server.begin();
Serial.println("HTTP server started");
}
void loop() {
server.handleClient();
}
