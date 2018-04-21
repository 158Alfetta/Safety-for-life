#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <pitches.h>

#define I2C_ADDR 0x3F // <
#define BACKLIGHT_PIN 3

// keypad section
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {8,7,6,5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4,3,2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);
SoftwareSerial MegaSerial(52, 53); // RX | T

int count = 0; //Counting_Char_On_Dispay(16_or_32)
char cmd_chk[4]; //Check_Show_or_Reset_Password
int ast_count=0; // counting asterisk '*' (ดอกจัน)
int first_char_chk=0; // confirm that '*' need to be first
char password[20]="88888888"; //Password_AP (Just sample)
int door_num; // DOOR STATUS (21 = Ready)
int SIG = 26; // IR
int EN = 28; // IR
unsigned long timer; // real time
unsigned long detect_door; // detect time (start)
unsigned long detect_ir; // detect time (start)
long warn_door=60000; // limit of time waiting for respond form IR
int lcd_status = 1;
int lcd_first = 1;

void AlertNormalSound();
int LcdShowStatus();

void setup()
{
digitalWrite(48, 0);
lcd.begin (16,2); // <
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
Serial.begin(9600);

//AP
pinMode(52,INPUT);// Serial Input NodeMCU
pinMode(53,OUTPUT);// Serial Output NodeMCU
pinMode(51,INPUT);// Serial2 Input NodeMCU

//Door sys.
pinMode(48,OUTPUT);// command Relay to open the door

//IR section 
pinMode(SIG, INPUT);  //26
pinMode(EN, OUTPUT);  //28
digitalWrite(EN, 1);  //start

//sound section
pinMode(10, OUTPUT);  // speaker

//sender section Connect to Nodemcu2
pinMode(35, OUTPUT);// (wire to D0) //line notify alert
pinMode(37, OUTPUT);// (wire to D1) //line notify sign-in
pinMode(39, OUTPUT);// (wire to D2) //line notify sign-out
pinMode(41, OUTPUT);// (wire to D3) // line notify password has been changed.
MegaSerial.begin(57600);
}

void loop()
{
  if(lcd_first){ // display status on LCD.
    lcd_first = LcdShowStatus(lcd_status);
  }
  if(!digitalRead(SIG)){
    // when detected activities on ลูกบิด and no wifi connect
    // will send notification to line and alarm 
    lcd.clear();lcd.print("Alert!");delay(500);lcd.clear();
    digitalWrite(35, 1);  //send to Nodemcu #2
    delay(70);
    digitalWrite(35, 0);
    delay(5000); // waiting for sending  
    for(int i=0;i<30;i++){    //  alarm play
     tone(10, NOTE_C4, 200);
     delay(300);
     tone(10, NOTE_A3, 200); 
    }
    lcd_first = 1;
    lcd_status = 1;
  }
  timer = millis(); // start timing
  char key = keypad.getKey(); // get num from keypad
            
  if(digitalRead(51)){  // recieve wifi ap status check connection.
    door_num=21;
    digitalWrite(41, 1);delay(50);digitalWrite(41, 1);  //  send to nodemcu#2 to send line notification.
  }
  
  if(door_num==21){   // someone connect to AP
    detect_door = timer;  //  start time that detect connection.
    while((timer-detect_door)<warn_door){
      timer = millis();
      if(!digitalRead(SIG)){  //  if IR detected activity.
        digitalWrite(48, HIGH); // open door for 6 sec.
        tone(10, NOTE_E4, 600);
        lcd.clear();lcd.print("-##DOOR LOCKED##");
        lcd.setCursor(0, 1);lcd.print("WELCOME BACK!");
        delay(6000);
        lcd.setCursor(0, 0);
        digitalWrite(EN, 0);    // Turn off IR sensor
        AlertNormalSound();
        digitalWrite(48, LOW);
        MegaSerial.print(15);   // Send infomation to Wifi AP to shut down AP.
        digitalWrite(37, 1);  // send line noti.
        delay(70);            // delay for respond.
        digitalWrite(37, 0); // close send line noti.
        lcd_first = 1;
        lcd_status = 0;
        break;
      }
    }
    lcd.clear();lcd.print("-##DOOR LOCKED##");
    delay(300);lcd.clear();
    door_num = 0; //   reset wificonnection status.
  }

  // +++++++++++++++ NEXT SECTION IS ACTIVITIES AROUND THE KEYPAD KUB+++++++++++++++++++++++++++++++++++++++
  
  if (key != NO_KEY){
    if(count==0){
      lcd.clear();
    }
    if(key == '*' && first_char_chk == 0 || key == '#' && first_char_chk == 0 || key == '0' && first_char_chk == 0){ // detected '*' & '#' & '0'
      cmd_chk[ast_count] = key; // fills the '*' in array for checking  
      ast_count++;
    }
    else{
      first_char_chk = 1; // To check the first states ('*' must become first)
    }
    lcd.print(key);
    Serial.println(key);
    count++;
  }
  if(ast_count == 4){ // check command from keypad 
    if(cmd_chk[0] == '*' && cmd_chk[1] == '*'&& cmd_chk[2] == '*'&& cmd_chk[3] == '*'){ // SHOW PASSWORD
      tone(10, NOTE_E6, 100);
      lcd.clear();lcd.print("PW: ");lcd.print(password);
      delay(1000);
      lcd.clear();
    }else if(cmd_chk[0] == '*' && cmd_chk[1] == '#'&& cmd_chk[2] == '*'&& cmd_chk[3] == '#'){ 
      //  ###############################################
      //  CHANGE PASSWORD
      //  1.get password from keypad 8 character
      //  2.send password[8] politely to NodeMCU(WIFI_AP)
      //  ################################################
      tone(10, NOTE_E6, 100);
      
      // ----------- LCD SETTING-------------
      lcd.clear();lcd.print("CHANGE PASSWORD");lcd.setCursor(0, 1);lcd.print("-----SETUP.-----");
      delay(1000);lcd.clear();lcd.setCursor(0, 0);lcd.print("Enter New PW");
      lcd.setCursor(0, 1);lcd.print("PW MustBe 8 Char");delay(1200);lcd.clear();lcd.print("New_PW:");
      // ------------------------------------
      
      // ----------- LCD PASSWORD DISPLAY PANEL ---------------
      int j=7;
      for(int i=0;i<8;i++){
        delay(200);
        while(1){
          key = keypad.getKey();
          if(key != NO_KEY){
            if(key == '*' || key == '#'){
              lcd.setCursor(0, 1);lcd.print("CAN'T USE *, #");
              continue;
            }
            password[i] = key; // change password state
            break;
          }
        }
        lcd.setCursor(j, 0);lcd.print(password[i]);
        j++;
      }
      // --------------------------------------------------------

      
      MegaSerial.print(47); // send command to WifiAP
      delay(2500); // waiting

      // ------------------ SEND TO NODEMCU ---------------------
      while(1){
        Serial.println("Waiting for NodeMCU");
          int cnt=0;
          while(cnt<8){
            delay(200);
            int i_data = password[cnt];
            Serial.print(i_data);
            MegaSerial.print(i_data);MegaSerial.print("\n");
            delay(100);
            cnt++;
          }
          break;
      }
      // ------------------------------------------------------
      
      lcd.clear();lcd.print("YOUR NEW PW IS:");lcd.setCursor(0, 1);lcd.print(password);delay(3000);lcd.clear();

    }else if(cmd_chk[0] == '0' && cmd_chk[1] == '0'&& cmd_chk[2] == '0' && cmd_chk[3] == '#'){
      //  ###############################################
      //  GET OUT AND LEAVE HOME FOR A WHILE.
      //  1. open door, open AP again and send line notification
      //  2. waiting for 20 seconds then open IR Again.
      //  ###############################################
      tone(10, NOTE_E6, 100);
      lcd.clear();lcd.print("HAVE A GOOD DAY");lcd.setCursor(0, 1);
      lcd.print("#DOOR UNLOCKED#");
      
      MegaSerial.print(16); // send to open AP again.
      delay(250);
      
      // ===============Door System Section.=================
      digitalWrite(48, HIGH);
      tone(10, NOTE_E4, 700);
      delay(6000);
      AlertNormalSound();
      digitalWrite(48, LOW);
      // ====================================================
      
      delay(20000);
      digitalWrite(EN, 1);  // open IR Again
      lcd.clear();lcd.setCursor(0, 0);
      digitalWrite(39, 1);  //  send to nodeMCU#2
      delay(70);
      digitalWrite(39, 0);
      lcd_first = 1;
      lcd_status = 1;
      
    }else if(cmd_chk[0] == '#' && cmd_chk[1] == '#'&& cmd_chk[2] == '#' && cmd_chk[3] == '#'){
      //  ###############################################
      //  OPEN THE DOOR TEPMPORARY. (open the door not full service, You in home). 
      //  1. open door for 8 seconds.
      //  ###############################################
      tone(10, NOTE_E6, 100);
      digitalWrite(48, HIGH);
      tone(10, NOTE_E4, 700);

      // +++++++++ LCD DISPLAY COUNTDOWN +++++++++++++++++++++++
      lcd.clear();lcd.print("WILL LOCK AGAIN");lcd.setCursor(0, 1);
      for(int i=8; i>0; i--){
        lcd.print("IN");
        lcd.setCursor(3, 1);
        lcd.print(i);
        lcd.setCursor(5, 1);
        lcd.print(" SECOND!");
        delay(700);
        lcd.clear();
      }
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
      AlertNormalSound();
      
      digitalWrite(48, LOW); // close door
      lcd.clear();lcd.setCursor(0, 0);lcd.print("#DOOR LOCKED!#");
      delay(500);lcd.clear();
      lcd_first = 1;
      lcd_status = 0;
    }
    ast_count = 0;
    count=0;
  }

  // ========LCD line display management=========
  if(count == 16){ // first line full
    lcd.setCursor(0, 1);
    }
  else if(count == 32){ // Display Full
    delay(50);
    lcd.clear();
    count = 0;
    first_char_chk = 0;
  }
  // ============================================
}

void AlertNormalSound(){
  // #########################################
  // Play sound reminder the door is closing.
  // #########################################
  tone(10, NOTE_A6, 90);
  delay(150);
  tone(10, NOTE_A6, 90);
  delay(150);
  tone(10, NOTE_A6, 90);
}

int LcdShowStatus(int lcd_status){
  // #########################################
  // LCD DISPLAY THAT SHOW STATUS ALWAYS. 
  // #########################################
  if(lcd_status){
    lcd.print(">SECURITY SYSTEM");lcd.setCursor(0, 1);
    lcd.print("#FULL ACTIVATED#");lcd.setCursor(0, 0);
  }else{
    lcd.print(">SECURITY SYSTEM");lcd.setCursor(0, 1);
    lcd.print("#SEMI ACTIVATED#");lcd.setCursor(0, 0);
  }
  return 0;
}

