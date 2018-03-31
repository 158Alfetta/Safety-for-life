#include <ESP8266WiFi.h>

IPAddress ip(192,168,43,50);
IPAddress gate(192,168,43,1);
IPAddress subn(255,255,255,0);
void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin("sony", "88888888");
  Serial.begin(115200);
  WiFi.softAP("test", "00000000");
  WiFi.softAPConfig(ip, gate, subn);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(250);
      Serial.print(".");
   }

   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   Serial.println(WiFi.gatewayIP());
  Serial.println((WiFi.softAPIP()));
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Connect station: ");
  Serial.println((WiFi.softAPgetStationNum()));
  delay(2000);
}
