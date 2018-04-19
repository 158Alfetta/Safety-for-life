#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
extern "C" {
#include<user_interface.h>
}

SoftwareSerial NodeSerial(D2,D3); // RX | TX

int count=0, command=0, ap_status=1, door_state = 1;
char cha, password[9]="88888888";

ESP8266WebServer server(80);


void setup() {
  pinMode(D2, INPUT); // get input from Mega wire to 53
  pinMode(D3, OUTPUT); // Output to Megawire to 52
  pinMode(D1, OUTPUT); // 2 get input from Mega wire to 51
  Serial.begin(9600);
  NodeSerial.begin(57600);
}

void loop() {
  if(!ap_status){
    WiFi.softAPdisconnect(1);
  }else{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("I LOVE COMPRO", password);
  }
  server.handleClient();
  delay(10);
  if(client_status() && door_state){
    Serial.println("ON");
    digitalWrite(D1, 1);
    delay(100);
    digitalWrite(D1, 0);
    door_state = 0;
  }else if(!client_status() && !door_state){
    door_state = 1;
  }
  Serial.println(F("Password : "));
  Serial.println(password);
  command = NodeSerial.parseInt();
  //get command 15 disconnect wifi / 47 change password // 16 open wifi
  Serial.print("command : ");Serial.print(command);
  if(command==47)
      {
      delay(100);
      Serial.println("COMEIN");
      delay(500);
      while(count<8){
        int i_data = NodeSerial.parseInt();
        if (NodeSerial.read() == '\n' && count<8) 
        {
          Serial.print(F("PASSWORD CHANGE")); Serial.println(F(" : ")); 
          Serial.print(i_data); Serial.print(F(" : ")); 
          cha = i_data;
          Serial.println(cha);
          password[count] = cha;
          count++;
          delay(200);
        }
      }
      count = 0;
      delay(50);
  }else if(command==15){
    ap_status = 0;
    Serial.println(F("AP MODE = OFF"));
  }else if(command==16){
    ap_status=1;
    Serial.println(F("AP MODE = ON"));
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
    
    if(address){
      Serial.println(F("COMPLETE"));
      chk=1;
    }
    stat_info = STAILQ_NEXT(stat_info, next);
    i++;
  }
  return chk;
}


