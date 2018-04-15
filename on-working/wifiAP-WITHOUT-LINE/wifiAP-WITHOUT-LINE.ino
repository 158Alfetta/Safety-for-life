#include <Gsender.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
extern "C" {
#include<user_interface.h>
}

void handleRoot();
int client_status();

const char* ssid = "redmi4x";                          
const char* password_c = "77777776";                       
uint8_t connection_state = 0;                  
uint16_t reconnect_interval = 10000;


SoftwareSerial NodeSerial(D2,D3); // RX | TX

int count=0, command=0, ap_status=1, door_state = 1;
char cha, password[9]="88888888";

ESP8266WebServer server(80);

void handleRoot() { 
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  pinMode(D2, INPUT); // get input from Mega wire to 53
  pinMode(D3, OUTPUT); // Output to Megawire to 52
  pinMode(D1, OUTPUT); // 2 get input from Mega wire to 51
  Serial.begin(9600);
  NodeSerial.begin(57600);
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

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);  
    } else {
        WiFi.begin(ssid, password_c);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) 
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        return false;
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}

void loop() {
  if(ap_status){
    WiFi.mode(WIFI_AP);
    WiFi.softAP("I LOVE COMPRO", password);
  }else{
    WiFi.softAPdisconnect(1);
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
    WiFi.softAPdisconnect(1);
    connection_state = WiFiConnect();
    if(!connection_state)
        Awaits();
    Gsender *gsender = Gsender::Instance();
    String subject = "##SECURITY SYSTEM##";
    if(gsender->Subject(subject)->Send("60070120@kmitl.com", "Welcome back!, Now you get into home. If it not you, you was invaded")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
    
  }else if(command==16){
    ap_status=1;
    Serial.println(F("AP MODE = ON"));
    connection_state = WiFiConnect();
    if(!connection_state)
        Awaits();
    Gsender *gsender = Gsender::Instance();
    String subject = "##SECURITY SYSTEM##";
    if(gsender->Subject(subject)->Send("60070120@kmitl.com", "You left home now, HAVE A GOOD DAY!")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
  }else if(command==12){
    ap_status=1;
    WiFi.softAPdisconnect(1);
    Serial.println(F("AP MODE = ON"));
    connection_state = WiFiConnect();
    if(!connection_state)
        Awaits();
    Gsender *gsender = Gsender::Instance();
    String subject = "##SECURITY SYSTEM##";
    if(gsender->Subject(subject)->Send("60070120@kmitl.com", "Alert!! WITHOUT PERMISSION ENTERING, YOUR HOUSE WAS INVADED")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
  }
}


