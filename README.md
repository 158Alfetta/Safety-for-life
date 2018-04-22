# Safety-for-life

We're surprinsingly to present our project. This project is about to use micro-controller act as sercurity system :)
Secure your house and will let you know every activities at the door! (i mean notification).

So if you have a door you need a key.....it's not anymore
this sercurity system let you know that

  * The key.  ----------> Unnecessary
  * RFID Card. ----------> Unnecessary
  * Fills the password every time. ----------> Unnecessary
  
Just need the smartphone can connect to WIFI. Only 2 Step!
  * Connect WiFi to NodeMCU.
  * Door Unlocked!

How incredible!
_____________________________________________________________________________________________________

## Overview
 ### How to use
 * When you want to get in. if you turn on wifi, your phone will connect to system automatically then you walk in front of the door
 if you phone don't regconize the password, fill the password.
 ### This system have 2 mode (it's reported you on the LCD Screen Always).
* FULL ACTIVATED MODE
  * it's appear when you outside
  * The Infrared will turn on and will detect abnormal activities. and report to you.
* SEMI ACTIVATED MODE
  * Appear When you inside your place.
  * Infrared turn off.
  * You can use keypad function
     * Press "&ast;&ast;&ast;&ast;" to show corrent password.
     * Press "000#" to going out.
     * Press "&ast;#&ast;#" to change password.
 ### How Secure?
  * Password Protection 8 PIN.
  * Immediately report to you when have connection to system.
  * Can check the time on Line Application when you went out or you get in.
  * Don't use physical method to get in (like key or RFID card), So in case of you forget the key. you don't need to concern.
  * Detect and report you when have an abnormal activity at the door.
  * We don't use Bluetooth because it's unsafe than WiFi system and you need to pairing.
_____________________________________________________________________________________________________


## Function
 Various situations are there, so you need to know.
 * Every activities will notice you politely on the 'Line Application'.
 * if door locked and you need to enter you place.(Sign in)
   * Connect wifi ssid "I LOVE COMPRO" Fill password ('88888888' is default).
   * When password is correct. you have 45 seconds to stand in front of the door and twist door knob.
   * While you twist the door knob. Sensor will detect your activity then unlock the door. Sensor will turn off automatically.
 * if door locked and you want to go outside.(Sign out)
   * Press "000#" on the keypad. Display Show "----Door Unlocked----" Then you have 6 Seconds to open the door  
     before the alart sound start.
   * After you open the door, you have 20 Sec. for going out. Sensor IR will turn on automatically.
 * if door locked and you was outside your place.
   * If IR sensor detect someone who doing unknown activities on your knob. will report line message to you.  
   and alert sound will play repeating loudly.
 * if door lock and you need to open temporary.
   * Press "####" on the keypad. you have 8 seconds to open the door. in this way wifiAP and IR sensor will not turn on.
 * On Keypad Function
   * Press "&ast;&ast;&ast;&ast;" to show corrent password.
   * Press "000#" to going out.
   * Press "&ast;#&ast;#" to change password.
   
## Manual
 There have 3 important files.
 1. Main_project  
  A main project source of Arduino Mega 2560 Board
  * List of function available
    * LCD Controll (display status, show passord, show input from keypad).
    * Get input from keypad.
    * Change password, Show password.
    * Control sound alarm.(Speaker).
    * Control Relay which is controlling solenoid.
    * Send command to NodeMCU#1 (WiFiAP).
    * Send command to NodeMCU#2 (Line Notify).
    * recieve status from both of NodeMCU.
 2. wifiAP-Nodemcu  
  Source code of NodeMCU V3 (#1) is about WiFi Access Point.
  * List of function available
    * Access point service maximum 5 user.
    * Change password (that recieve data form Mega by serial connection).
    * Access point password protection.
 3. nodemcu_sender
 Source code of NodeMCU V2 (#2) is about line notify and client wifi.
  * List of function available
    * cilent wifi.
    * Line notification with different 4 message for various situation.
    
## Installation
 List of several pieces of silicon board including how to connect each other.
 * LCD Display 16x2 Dot Matrix

 | LCD | Arduino Mega |
 | --------- | ---------- |
 |   SCL   |    SCL   |
 |   SDA   |    SDA   |
 |   VCC   |    VIN   |
 |   GND   |    GND   |
 
 * Infrared sensor module (IR)
 
 | IR | Arduino Mega |
 | --------- | ---------- |
 |   SIG   |    26   |
 |   EN   |    28   |
 |   VCC   |    5V   |
 |   GND   |    GND   |  

* Relay 5V Module

 | Relay | Arduino Mega |
 |-------|--------------|
 | IN    | 48           |  
 | VCC   | 5V           |               
 | GND   | GND          |
 
* Speaker

 | Speaker            | Arduino Mega |   
 | ------------- |-------------|
 | +            | 10           |
 | -             | GND           |
 
* NodeMCU (V3) #1

 | NodeMCU (V3) #1| Arduino Mega |   
 | ------------- |-------------|
 | D1            |   51       |
 | D2            |   53         |
 | D3            |   52       |
 | VIN           |   VIN       |
 | GND           |   GND      |
 
* NodeMCU (V2) #2

 | NodeMCU (V2) #2| Arduino Mega |   
 | ------------- |-------------|
 | D0            |   35       |
 | D1            |   37         |
 | D2            |   39       |
 | D3            |   41       |
 | GND           |   GND      |
 
* Keypad

| Keypad | Arduino Mega |  
| ------------- |-------------|  
| Left to Right | 8 to 2      |


## Requirements
 * List of Library (kindly download in "Library" Branch.)
   * Wire.h
   * LCD.h
   * LiquidCrystal_I2C.h 
   * Keypad.h
   * SoftwareSerial.h
   * pitches.h
   * ESP8266WiFi.h
 * Smartphone
 * Wireless network on your home.
 

## Author
* 60070112 อภินันท์ พงศ์รัตนโชติ
* 60070120 อุดมเอก ชุมทองมา
* 60070180 ภูวิศ คุ้มภัย

---

Faculty of Information Technology

King Mongkut's Institute of Technology Ladkrabang

