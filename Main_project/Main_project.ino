#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

#define I2C_ADDR 0x3F // <
#define BACKLIGHT_PIN 3

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

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);
SoftwareSerial MegaSerial(43, 42); // RX | TX
int count_cg;
int count = 0; //Counting_Char_On_Dispay(16_or_32)
char cmd_chk[4]; //Check_Show_or_Reset_Password
int ast_count=0; // counting asterisk '*' (ดอกจัน)
int first_char_chk=0; // confirm that '*' need to be first
char password[20]="88888888"; //Password_AP (Just sample)
char tmp_pw[20]; //in case of cancel change password
int state_door = 0; // check status

void setup()
{
lcd.begin (16,2); // <
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
Serial.begin(9600);
pinMode(43,INPUT);
pinMode(42,OUTPUT);
pinMode(50,INPUT);
MegaSerial.begin(57600);
}

void loop()
{
  char key = keypad.getKey();
  //****_Show_Password_####_to_Reset_Password
  
  if (key != NO_KEY){
    if(key == '*' && first_char_chk == 0 || key == '#' && first_char_chk == 0){ // detected '*' & '#'
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
  if(ast_count == 4){
    if(cmd_chk[0] == '*' && cmd_chk[1] == '*'&& cmd_chk[2] == '*'&& cmd_chk[3] == '*'){
      lcd.clear();lcd.print("PW: ");lcd.print(password);
      delay(1000);
      lcd.clear();
    }else if(cmd_chk[0] == '#' && cmd_chk[1] == '#'&& cmd_chk[2] == '#'&& cmd_chk[3] == '#'){
      // LCD setting part
      lcd.clear();lcd.print("CHANGE PASSWORD");
      lcd.setCursor(0, 1);lcd.print("-----SETUP.-----");
      delay(1000);
      lcd.clear();lcd.setCursor(0, 0);lcd.print("Enter New PW");
      lcd.setCursor(0, 1);lcd.print("PW MustBe 8 Char");
      delay(1200);
      lcd.clear();lcd.print("New_PW:");
      int j=7;
      delay(200);
      // check status
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
        //END LCD
      }
      MegaSerial.print(47);
      delay(2500);
      
      while(1){
        Serial.println("Waiting for NodeMCU");
        if(!digitalRead(50)){
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
      }
      lcd.clear();lcd.print("YOUR NEW PW IS:");lcd.setCursor(0, 1);lcd.print(password);
      delay(5000);
      lcd.clear();
    }
    ast_count = 0;
    count=0;
  }
  if(count == 16){ // first line full
    lcd.setCursor(0, 1);
    }
  else if(count == 32){ // Display Full
    delay(500);
    lcd.clear();
    count = 0;
    first_char_chk = 0;
  }
}
