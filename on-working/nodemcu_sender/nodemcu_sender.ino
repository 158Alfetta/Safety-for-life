#include <ESP8266WiFi.h>

#define WIFI_SSID "redmi4x"
#define WIFI_PASSWORD "77777776"
String LINE_TOKEN = "WtMZXp16iiizFrFegxtgZnh61U7QTBjEvrdbThZgI3e";
int command = 0;

void sender(String LINE_Token, String message){
  //##########################################
  //line notification
  //##########################################
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
  if(!digitalRead(D0)){
    sender(LINE_TOKEN, "Alert!! WITHOUT PERMISSION DETECTED. YOU PLACE MAYBE WAS INVADED");
    delay(200);
  }
  if(!digitalRead(D1)){
    sender(LINE_TOKEN, "Welcome Back! You're home now. If it not you, you place was invaded");
    delay(200);
  }
  if(!digitalRead(D2)){
     sender(LINE_TOKEN, "You just left, Have a good day!");
     delay(200);
  }
  if(!digitalRead(D3)){
    sender(LINE_TOKEN, "Connected to Wifi Security System.");
    delay(200);
  }
}
