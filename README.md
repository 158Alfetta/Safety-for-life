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

## Function

in process.

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
    * Act as cilent wifi.
    * Line notification with different 4 message for various situation.
## Installation

[Full documentation](https://github.com/compro-itkmitl/demo-repo/wiki/)

## Requirements
 List of Library (kindly download in "Library" Branch.)
  * Wire.h
  * LCD.h
  * LiquidCrystal_I2C.h 
  * Keypad.h
  * SoftwareSerial.h
  * pitches.h

## Author
* 60070112 อภินันท์ พงศ์รัตนโชติ
* 60070120 อุดมเอก ชุมทองมา
* 60070180 ภูวิศ คุ้มภัย

---

Faculty of Information Technology

King Mongkut's Institute of Technology Ladkrabang
