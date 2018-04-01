int i_data, count=0;

char password[8] = "87508750";


#include <SoftwareSerial.h>

SoftwareSerial NanoSerial(43, 42); // RX | TX

void setup()

{  

  pinMode(43,INPUT);

  pinMode(42,OUTPUT);

  Serial.begin(9600);

  NanoSerial.begin(57600);

}

void loop() {
  delay(2000);
  if(count<8){
    i_data = password[count];
    Serial.print(i_data);
    NanoSerial.print(i_data);NanoSerial.print("\n");
    delay(100);
    count++; 
  }
}

 
