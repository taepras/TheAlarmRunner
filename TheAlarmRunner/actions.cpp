#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Time.h>
#include <DS1307RTC.h>
#include "util.h"
#include "init.h"
#include "actions.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------- LCD

int blinkCounter = 0;
int backlightCounter = 0;
unsigned char lcdStatus = 0;     // LSB = is blinking, Next Bit = LCD is on

unsigned char isLcdBacklightOn(){
  return (lcdStatus >> 1) & 1;
}

void lcdBacklightOn(){ lcdStatus |= 2; }
void lcdBacklightOff(){ lcdStatus &= ~2; }

void turnBacklightOn(LiquidCrystal_I2C lcd){
  lcdBacklightOn();
  backlightCounter = LCD_LIGHT_TIME / REFRESH_RATE;
}

void updateBacklight(LiquidCrystal_I2C lcd){
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

void blinkLCD(LiquidCrystal_I2C lcd){
  lcdStatus |= 1;
  blinkCounter = 0;
}

void noBlinkLCD(LiquidCrystal_I2C lcd){
  lcdStatus &= ~1;
  blinkCounter = 0;
  turnBacklightOn(lcd);
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

void printLcdCenter(LiquidCrystal_I2C lcd, String text, int row){
  lcd.setCursor(0, row);
  lcd.print("                ");
  lcd.setCursor(max(8 - text.length() / 2, 0), row);
  lcd.print(text);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------- RTC

String get2DString(int num){
  if(num < 10)
    return "0" + String(num);
  else
    return String(num);
}

unsigned char alarmHour;
unsigned char alarmMin;

unsigned char getAlarmHour(){
  return alarmHour;
}

unsigned char getAlarmMin(){
  return alarmMin;
}

unsigned char isAlarmTime(int hr, int mn){
  return hr == alarmHour && mn == alarmMin;
}

void setAlarmTime(String timeString){
  lcdBacklightOn();
  for(int i = 0; i < timeString.length(); i++)
    EEPROM.write(i, timeString.charAt(i));
  loadAlarmTime();
}

void loadAlarmTime(){
  alarmHour = (EEPROM.read(0) - '0') * 10 + (EEPROM.read(1) - '0');
  alarmMin = (EEPROM.read(3) - '0') * 10 + (EEPROM.read(4) - '0');
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

String getLineFromSerial(){
  unsigned int startTime = millis();
  String recieved = "";
  char buff[] = {0, 0};
  while(buff[1] != '\r' || buff[0] != '\n'){
    char tmp = (char)Serial.read();
    if(tmp > 0){
      if(buff[1] > 0)
        recieved += buff[1];
      buff[1] = buff[0];
      buff[0] = tmp;
    }
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
  for(int i = 0; i < s.length(); i++)
    if(ca[i] != s.charAt(i))
      return false;
  return true;
}

unsigned char waitForSerialString(String waiting){
  unsigned int startTime = millis();
  int n = waiting.length();
  char buff[n];
  do{
    for(int i = 0; i < n - 1; i++){
      buff[i] = buff[i + 1];
    }
    char serialIn = (char)Serial.read();
    if(serialIn != -1)
      buff[n - 1] = serialIn;
    if(millis() - startTime >= SERIAL_TIMEOUT)
      return false;
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
