#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D2,D3); // RX | TX
SoftwareSerial Node2Serial(D4,D5); 

int count=0;
char cha, password[20];

void setup() {
  pinMode(D2, INPUT); 
  pinMode(D3, OUTPUT);
  pinMode(D4, INPUT); 
  pinMode(D5, OUTPUT); 
  Serial.begin(9600);
  NodeSerial.begin(57600);
  Node2 Serial.begin(57600);
  Serial.println("NodeMCU/ESP8266 Run");
 }

void loop() {
    int j_data = 3;
    int i_data = NodeSerial.parseInt();
    Node2Serial.print(j_data);Node2Serial.print("\n");
    Serial.print("i_data ");
    Serial.print(i_data);
    delay(50);
    delay(10);
} 
