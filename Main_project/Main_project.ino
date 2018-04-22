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
SoftwareSerial MegaSerial(52, 53); // RX | TX

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
long warn_door=45000; // limit of time waiting for respond form IR
int lcd_status = 1;   //   To select the message that want to show on LCD
int lcd_first = 1;  // To return display.

// Function Declaration.
void AlertNormalSound();
int LcdShowStatus();
void SendingMessage(int sender);

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
pinMode(51,INPUT);// 

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
pinMode(39, OUTPUT);// (wire to D2) //line notify alert
pinMode(41, OUTPUT);// (wire to D3) //line notify sign-in
MegaSerial.begin(57600);
digitalWrite(35, 1);
digitalWrite(37, 1);
digitalWrite(39, 1);
digitalWrite(41, 1);
}

void loop()
{
  if(lcd_first){ // display status on LCD.
    lcd_first = LcdShowStatus(lcd_status);
  }
  if(!digitalRead(SIG)){
    // when detected activities on ลูกบิด and no wifi connect
    // will send notification to line and alarm.
    lcd.clear();lcd.print("Alert!");delay(500);lcd.clear();
    SendingMessage(35); //sending alert message.
    delay(3500); // waiting for sending  
    for(int i=0;i<15;i++){    //  alarm play
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
    SendingMessage(41); // sending notice message.
  }
  
  if(door_num==21){   // someone connect to AP
    detect_door = timer;  //  start time that detect connection.
    while((timer-detect_door)<warn_door){
      timer = millis();
      if(!digitalRead(SIG)){  //  if IR detected activity.
        digitalWrite(48, HIGH); // open door for 6 sec.
        tone(10, NOTE_E4, 600);
        lcd.clear();
        lcd.setCursor(0, 1);lcd.print("WELCOME BACK!");
        digitalWrite(EN, 0);    // Turn off IR sensor
        delay(6000);
        lcd.setCursor(0, 0);
        AlertNormalSound();
        digitalWrite(48, LOW);
        MegaSerial.print(15);   // Send infomation to Wifi AP to shut down AP.
        SendingMessage(37);     // sending message
        delay(200);
        lcd_first = 1;    // return LCD
        lcd_status = 0;   // change to semi activated display
        break;
      }
    }
    lcd.clear();lcd.print("-##DOOR LOCKED##");
    delay(300);lcd.clear();
    door_num = 0; //   reset wificonnection status. for suspend timeout connection.
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
      lcd_first = 1;                            //return lcd
      lcd_status = 0;                           // semi activate status
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
      lcd_first = 1;
      lcd_status = 0;

    }else if(cmd_chk[0] == '0' && cmd_chk[1] == '0'&& cmd_chk[2] == '0' && cmd_chk[3] == '#'){
      //  ###############################################
      //  GET OUT.
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
      delay(5000);
      AlertNormalSound();
      digitalWrite(48, LOW);
      SendingMessage(39);
      // ====================================================
      
      delay(1000);
      
      lcd.setCursor(0, 0);
      lcd.clear();lcd.print("SYSTEM ACTIVATE--");lcd.setCursor(0, 1);
      for(int i=19; i>0; i--){
        lcd.print("IN");
        lcd.setCursor(3, 1);
        lcd.print(i);
        lcd.setCursor(5, 1);
        lcd.print(" SECOND!");
        delay(700);
        lcd.clear();
      }
      
      digitalWrite(EN, 1);  // open IR Again
      lcd.clear();lcd.setCursor(0, 0);
      lcd_first = 1;                // return LCD
      lcd_status = 1;               // full activate status
      
    }else if(cmd_chk[0] == '#' && cmd_chk[1] == '#'&& cmd_chk[2] == '#' && cmd_chk[3] == '#'){
      //  ###############################################
      //  OPEN THE DOOR TEPMPORARY. (open the door not full service, You in home). 
      //  1. open door for 8 seconds.
      //  ###############################################
      
      tone(10, NOTE_E6, 100);
      digitalWrite(48, HIGH);
      tone(10, NOTE_E4, 700);

      // +++++++++ LCD DISPLAY COUNTDOWN +++++++++++++++++++++++
      
      lcd.clear();lcd.setCursor(0, 0);lcd.print("WILL LOCK AGAIN");lcd.setCursor(1, 0);
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
  else if(count == 32){ // Display Full clear the display
    delay(50);
    lcd.clear();
    count = 0;
    first_char_chk = 0;
    lcd_first = 1;
  }
  // ============================================
}

void SendingMessage(int sender){
  // #########################################
  // a function will sending to NodeMCU #2
  // for sending message to use 
  // 'sender' is the number of port. 
  // #########################################
  digitalWrite(sender, 0);
  delay(50);
  digitalWrite(sender, 1);
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
  // ##################################################################################
  // LCD DISPLAY THAT SHOW STATUS ALWAYS. 
  //
  // if lcd_status == 1 will show full activate
  // is that mean every action by keypad will end in semi activated status.
  // obviously maybe it's bug but if you can use keypad firstly you need to be inside
  // it's bug, But it's not bug, So it's not bug.
  //
  // ##################################################################################
  if(lcd_status){
    lcd.print(">SECURITY SYSTEM");lcd.setCursor(0, 1);
    lcd.print("#FULL ACTIVATED#");lcd.setCursor(0, 0);
  }else{
    lcd.print(">SECURITY SYSTEM");lcd.setCursor(0, 1);
    lcd.print("#SEMI ACTIVATED#");lcd.setCursor(0, 0);
  }
  return 0;
}

