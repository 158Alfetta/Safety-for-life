#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D2,D3); // RX | TX

int count=0;
char cha, password[20];

void setup() {
  pinMode(D2, INPUT); 
  pinMode(D3, OUTPUT); 
  Serial.begin(9600);
  NodeSerial.begin(57600);
  Serial.println("NodeMCU/ESP8266 Run");
 }

void loop() {
  while (NodeSerial.available() > 0) 
  {
    int i_data = NodeSerial.parseInt();
    if (NodeSerial.read() == '\n' && count<8) 
    {
          Serial.print("PASSWORD CHANGE"); Serial.println(" : "); 
          Serial.print(i_data); Serial.print(" : "); 
          cha = i_data;
          Serial.println(cha);
          password[count] = cha;
          count++;
    }
    Serial.print("Password : ");
    Serial.print(password);
    delay(50);
  }
  delay(10);
} 
