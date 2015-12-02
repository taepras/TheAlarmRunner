#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "util.h"
#include "init.h"
#include "actions.h"

int backlightCounter = 0;
int blinkCounter = 0;
int lcdStatus = 0;     // LSB = is blinking, Next Bit = LCD is on

void printLcdStatus(){
  Serial.println(lcdStatus, BIN);
}

void turnBacklightOn(LiquidCrystal_I2C lcd){
  lcd.backlight();
  lcdStatus |= 2;
  backlightCounter = LCD_LIGHT_TIME / REFRESH_RATE;
}

void updateBacklight(LiquidCrystal_I2C lcd){
  if(lcdStatus & 1 == 1){
    if (blinkCounter <= 0) {
      if(lcdStatus & 2)
        lcd.backlight();
      else
        lcd.noBacklight();
      lcdStatus ^= 2;
      blinkCounter = BLINK_PERIOD / REFRESH_RATE / 2;
    } else
      blinkCounter--;  
  }else{
    if (backlightCounter <= 0) {
      lcd.noBacklight();
      if(isActive())
        setInactive();
      lcdStatus &= ~2;
    } else {
      lcd.backlight();
      backlightCounter--;  
    }
  }
}

void blinkLCD(LiquidCrystal_I2C lcd){
  lcdStatus |= 1;
  //backlightTime = millis();
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

String get2DString(int num){
  if(num < 10)
    return "0" + String(num);
  else
    return String(num);
}

int alarmHour = 14;
int alarmMin = 38;

int getAlarmHour(){
  return alarmHour;
}

int getAlarmMin(){
  return alarmMin;
}

int isAlarmTime(int hr, int mn){
  return hr == alarmHour && mn == alarmMin;
}

void storeAlarmTime(int hr, int mn, LiquidCrystal_I2C lcd){
  lcd.backlight();
  String save = (get2DString(hr) + ":" + get2DString(mn));
  for(int i = 0; i < save.length(); i++)
    EEPROM.write(i, save.charAt(i));
  printLcdCenter(lcd, "ALARM SAVED", 0);
  printLcdCenter(lcd, save, 1);
}

void loadAlarmTime(LiquidCrystal_I2C lcd){
  alarmHour = (EEPROM.read(0) - '0') * 10 + (EEPROM.read(1) - '0');
  alarmMin = (EEPROM.read(3) - '0') * 10 + (EEPROM.read(4) - '0');
  printLcdCenter(lcd, "ALARM LOADED", 0);
  printLcdCenter(lcd, (get2DString(alarmHour) + ":" + get2DString(alarmMin)), 1);
}

