//#include <SoftwareSerial.h>

#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Time.h>
#include "DS1307RTC_CUSTOM.h"
#include "util.h"
#include "init.h"
#include "actions.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------- LCD

extern LiquidCrystal_I2C lcd;

int blinkCounter = 0;
int backlightCounter = 0;
unsigned char lcdStatus = 0;     // LSB = is blinking, Next Bit = LCD is on

unsigned char isLcdBacklightOn(){
  return (lcdStatus >> 1) & 1;
}

void lcdBacklightOn(){ lcdStatus |= 2; }
void lcdBacklightOff(){ lcdStatus &= ~2; }

void turnBacklightOn(){
  lcdBacklightOn();
  backlightCounter = LCD_LIGHT_TIME / REFRESH_RATE;
}

void updateBacklight(){
  if(lcdStatus & 1 == 1){
    if (blinkCounter <= 0) {
      if(lcdStatus & 2)
        lcdBacklightOn();
      else
        lcdBacklightOff();
      lcdStatus ^= 2;
      blinkCounter = BLINK_PERIOD / REFRESH_RATE / 2;
    } else
      blinkCounter--;  
  }else{
    if (backlightCounter <= 0) {
      lcdBacklightOff();
      if(isActive())
        setInactive();
      lcdStatus &= ~2;
    } else {
      lcdBacklightOn();
      backlightCounter--;  
    }
  }
}

void blinkLCD(){
  lcdStatus |= 1;
  blinkCounter = 0;
}

void noBlinkLCD(){
  lcdStatus &= ~1;
  blinkCounter = 0;
  turnBacklightOn();
}

void playAlarmSound(){
  if(lcdStatus & 2)
    tone(SPEAKER, 500);
  else
    tone(SPEAKER, 800);
}

void stopAlarmSound(){
  noTone(SPEAKER);
}

void printLcdCenter(String text, int row){
  lcd.setCursor(0, row);
  for(int i = 0; i < 16; i++)
    lcd.print(" ");
  //lcd.print("                ");
  lcd.setCursor(max(8 - text.length() / 2, 0), row);
  for(int i = 0; i < text.length(); i++)
    lcd.print(text.charAt(i));

  //Serial.println(text);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------- RTC

String get2DString(int num){
  String a = "";
  a += (num / 10);
  a += (num % 10);
  Serial.print(num);
  Serial.print(" ");
  Serial.println(a);
  return a;  
}

//unsigned char alarmHour;
//unsigned char alarmMin;

unsigned char getAlarmHour(){
  return (EEPROM.read(0) - '0') * 10 + (EEPROM.read(1) - '0');
}

unsigned char getAlarmMin(){
  return (EEPROM.read(3) - '0') * 10 + (EEPROM.read(4) - '0');
}

unsigned char isAlarmTime(int hr, int mn){
  return hr == getAlarmHour() && mn == getAlarmMin();
}

void setAlarmTime(String timeString){
  lcdBacklightOn();
  for(int i = 0; i < timeString.length(); i++)
    EEPROM.write(i, timeString.charAt(i));
  //loadAlarmTime();
}

//void loadAlarmTime(){
//  alarmHour = (EEPROM.read(0) - '0') * 10 + (EEPROM.read(1) - '0');
//  alarmMin = (EEPROM.read(3) - '0') * 10 + (EEPROM.read(4) - '0');
//}

String loadAlarmString(){
  String c = "";
  for(int i = 0; i < 5; i++)
    c += (char)EEPROM.read(i);
  return c;
}

//void setClockTime(String timeString){
//  // yyyy/mm/dd hh:mm:ss
//  int ye = (timeString.charAt(0) - '0') * 1000 + (timeString.charAt(1) - '0') * 100 + (timeString.charAt(2) - '0') * 10 + (timeString.charAt(3) - '0');
//  int mo = (timeString.charAt(5) - '0') * 10 + (timeString.charAt(6) - '0');
//  int da = (timeString.charAt(8) - '0') * 10 + (timeString.charAt(9) - '0');
//  int hr = (timeString.charAt(11) - '0') * 10 + (timeString.charAt(12) - '0');
//  int mn = (timeString.charAt(14) - '0') * 10 + (timeString.charAt(15) - '0');
//  int sc = (timeString.charAt(17) - '0') * 10 + (timeString.charAt(18) - '0');
//  setTime(hr, mn, sc, da, mo ,ye);
//  RTC.set(now());
//  //printLcdCenter(lcd, (get2DString(alarmHour) + ":" + get2DString(alarmMin)), 1);
//}

//-------------------------------------------------------------------------------------------------------------------------------------------------- Serial & Wifi

long startTime;
//extern SoftwareSerial mySerial;

String getLineFromSerial(){
  #ifdef DEBUG
    Serial.println("Waiting for input");
  #endif
  startTime = millis();
  String recieved = "";
  char buff[] = {0, 0};
  while(buff[1] != '\r' || buff[0] != '\n'){
    while(!Serial.available() && millis() - startTime < SERIAL_TIMEOUT);
    if(buff[1] > 0)
      recieved += buff[1];
    buff[1] = buff[0];
    buff[0] = (char)Serial.read();
    #ifdef DEBUG
//      Serial.print("Buffer: ");
//      Serial.print((char)buff[1]);
      Serial.print((char)buff[0]);
//      Serial.print("\ttime: ");
//      Serial.print(startTime);
//      Serial.print(" vs ");
//      Serial.println(millis());
    #endif
    if(millis() - startTime >= SERIAL_TIMEOUT)
      return "TIMEOUT";
  }
  return recieved;
}

//unsigned char waitForSerialLine(String waitingFor, LiquidCrystal_I2C lcd){
//  unsigned int startTime = millis();
//  String recievedLine = "";
//  do{
//    recievedLine = getLineFromSerial();
//    printLcdCenter(lcd, recievedLine, 0);
//    if(recievedLine == "TIMEOUT" || millis() - startTime >= SERIAL_TIMEOUT){
//      return false;
//    }
//  }while(recievedLine != waitingFor);
//  return true;
//}

unsigned char eq(char *ca, String s){
  for(int i = 0; i < s.length(); i++){
    if(ca[i] != s.charAt(i))
      return false;
  }
  #ifdef DEBUG
    Serial.println("EQ!");
  #endif
  return true;
}

unsigned char waitForSerialString(String waiting){
  #ifdef DEBUG
    Serial.print("Waiting for... ");
    Serial.println(waiting);
  #endif 
  startTime = millis();
  int n = waiting.length();
  char buff[n];
  for(int i = 0; i < n; i++) buff[i] = 0;
  do{
    #ifdef DEBUG
      Serial.print("Buffer: ");
      for(int i = 0; i < n; i++){
        Serial.print((char)buff[i]);
      }
      Serial.println("");
    #endif
    for(int i = 0; i < n - 1; i++){
      buff[i] = buff[i + 1];
    }
    while(!Serial.available() && millis() - startTime < SERIAL_TIMEOUT);
    buff[n - 1] = (char)Serial.read();
//    printLcdCenter(""+buff[n-1], 1);
//    delay(100);
Serial.print(buff[n-1]);
    #ifdef DEBUG
      
      Serial.print(buff[n-1]);
    #endif
    if(millis() - startTime >= SERIAL_TIMEOUT){
      #ifdef DEBUG
        Serial.println("TIMEOUT!");
      #endif
      return false;
    }
  }while(!eq(buff, waiting));
  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------- Motor

void driveMotor(int lf, int rf ,int lb, int rb){
  digitalWrite(MOTOR_L_F, lf);
  digitalWrite(MOTOR_R_F, rf);
  digitalWrite(MOTOR_L_B, lb);
  digitalWrite(MOTOR_R_B, rb);
}
