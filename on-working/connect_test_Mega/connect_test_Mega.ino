int i_data, count=0;

char password[8] = "87508750";


#include <SoftwareSerial.h>

SoftwareSerial NanoSerial(50, 42); // RX | TX
SoftwareSerial Nano2Serial(52, 44);


int j_data;
void setup()

{  

  pinMode(50,INPUT);

  pinMode(42,OUTPUT);

  pinMode(52,INPUT);

  pinMode(44,OUTPUT);

  Serial.begin(9600);

  NanoSerial.begin(57600);
  Nano2Serial.begin(57600);

}

void loop() {
  int i_data = 7;
  int j_data = Nano2Serial.parseInt();
  NanoSerial.print(i_data);NanoSerial.print("\n");
  Serial.print("j_data ");
  Serial.print(j_data);
  delay(50);
}

 
