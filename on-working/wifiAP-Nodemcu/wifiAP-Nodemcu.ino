#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D2,D3); // RX | TX

int count=0, cg_pw=0;
char cha, password[9]="88888888";

void setup() {
  pinMode(D2, INPUT); 
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  Serial.begin(9600);
  NodeSerial.begin(57600);
  Serial.println("NodeMCU/ESP8266 Run");
  //wifi statement
  WiFi.mode(WIFI_AP);
  WiFi.softAP("NodeMCU", password);
}

void loop() {
    Serial.println("Password : ");
    Serial.println(password);
    cg_pw = NodeSerial.parseInt();
  //change_pw
  Serial.print("cg_pw : ");Serial.print(cg_pw);
  if(cg_pw==47)
      {
      delay(100);
      Serial.println("COMEIN");
      digitalWrite(D2,HIGH);
      delay(500);
      digitalWrite(D2,LOW);
      while(count<8){
        int i_data = NodeSerial.parseInt();
        if (NodeSerial.read() == '\n' && count<8) 
        {
          Serial.print("PASSWORD CHANGE"); Serial.println(" : "); 
          Serial.print(i_data); Serial.print(" : "); 
          cha = i_data;
          Serial.println(cha);
          password[count] = cha;
          count++;
          delay(200);
        }
      }
      count = 0;
      Serial.print("Password : ");
      Serial.print(password);
      delay(50);
    delay(10);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("NodeMCU", password);
  }

  Serial.println("Connect station: ");
  Serial.println(WiFi.softAPgetStationNum());
  delay(2000);
}
