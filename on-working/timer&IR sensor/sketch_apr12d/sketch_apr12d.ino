int SIG = 26;
int EN = 28;
unsigned long timer;
unsigned long detect;
long warning=20000;


void setup() {
  pinMode(SIG, INPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, 0);
  Serial.begin(9600);
}

void loop() {
  timer = millis();
  Serial.print("NOW IS ");Serial.println(timer);
  digitalWrite(EN, 1);
  if(!digitalRead(SIG)){
    detect = timer;
    delay(500);
    Serial.println("DETECT TIME IS ");Serial.println(detect);
    Serial.println("DETECTED");
    while((timer-detect)<warning){
      timer = millis();
      Serial.println("OK");
      Serial.println("SUM ");Serial.println(timer-detect);
    }
 }
}
