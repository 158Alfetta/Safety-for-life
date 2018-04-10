#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
extern "C" {
#include<user_interface.h>
}

SoftwareSerial NodeSerial(D2,D3); // RX | TX

int count=0, cg_pw=0;
char cha, password[9]="88888888";

ESP8266WebServer server(80);

void handleRoot() { 
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  pinMode(D2, INPUT); 
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D7, OUTPUT);
  Serial.begin(9600);
  NodeSerial.begin(57600);
  Serial.println("NodeMCU/ESP8266 Run");
  //wifi statement
  WiFi.mode(WIFI_AP);
  WiFi.softAP("I LUV COMPRO++", password);
}

void loop() {
  server.handleClient();
  delay(10);
  if(client_status()){
    Serial.println("ON");
    digitalWrite(D7,HIGH);
  }else{
    digitalWrite(D7,LOW);
  }
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
}

int client_status(){
  int chk=0;
  unsigned char number_client;
  struct station_info *stat_info;
  
  struct ip_addr *IPaddress;
  IPAddress address;
  int i=1;
  stat_info = wifi_softap_get_station_info();
  
  while (stat_info != NULL) {
    
    IPaddress = &stat_info->ip;
    address = IPaddress->addr;
    
    //Serial.print("client= ");
    //Serial.print(i);
    //Serial.print(" ip adress is = ");
    //Serial.print((address));
    if(address){
      Serial.println("COMPLETE");
      chk=1;
    }
    stat_info = STAILQ_NEXT(stat_info, next);
    i++;
    Serial.println();
    
  }
  return chk;
}
