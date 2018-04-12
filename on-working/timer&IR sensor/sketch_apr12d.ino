int SIG = 27;
int EN = 28;
unsigned long timer;
unsigned long detect;


void setup() {
  pinMode(SIG, INPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, 0);
  Serial.begin(9600);
}

void loop() {
  timer = millis();
  Serial.println("NOW IS ");Serial.print(timer);
  digitalWrite(EN, 1);
  if(!digitalRead(SIG)){
    detect = timer;
    Serial.println("DETECT TIME IS ");Serial.print(detect);
    Serial.println("DETECTED");
    while(1){
      Serial.println("RESULT : ");Serial.print(timer-detect);
      if((timer-detect)>20000){
        Serial.println("OK");
        break;
      }else{
        Serial.println("miss");
      }
    }
  }
}
