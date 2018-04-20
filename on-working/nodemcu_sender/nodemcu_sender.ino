#include <ESP8266WiFi.h>

#define WIFI_SSID "redmi4x"
#define WIFI_PASSWORD "77777776"
String LINE_TOKEN = "WtMZXp16iiizFrFegxtgZnh61U7QTBjEvrdbThZgI3e";
int command = 0;

void sender(String LINE_Token, String message){
  
    WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("redmi4x", "77777776");
  pinMode(D0, INPUT); // wire to mega 35
  pinMode(D1, INPUT); // wire to mega 37
  pinMode(D2, INPUT); // wire to mega 39
  pinMode(D3, INPUT); // wire to mega 41
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(digitalRead(D0)){
    command = 35;
  }else if(digitalRead(D1)){
    command = 37;
  }else if(digitalRead(D2)){
    command = 39;
  }else if(digitalRead(D3)){
    command = 41;
  }

  if(command == 35){
    sender(LINE_TOKEN, "Alert!! WITHOUT PERMISSION SIGN IN. YOU PLACE WAS INVADED");
    command = 0;
  }else if(command == 37){
    sender(LINE_TOKEN, "Welcome Back! You're home now. If it not you, you place was invaded");
    command = 0;
  }else if(command == 39){
    sender(LINE_TOKEN, "You just left, Have a good day!");
    command = 0;
  }else if(command == 41){
    sender(LINE_TOKEN, "Password has been changed!");
    command = 0;
  }
}
