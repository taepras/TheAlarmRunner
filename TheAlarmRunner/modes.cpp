#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Time.h>
#include "modes.h"
#include "util.h"
#include "init.h"
#include "actions.h"

void normalSetup(LiquidCrystal_I2C lcd){
	lcd.clear();
}

void normalLoop(LiquidCrystal_I2C lcd){
  static short timeRefreshCount = 0;
  timeRefreshCount--;
  if(timeRefreshCount <= 0){
    timeRefreshCount = CLOCK_PERIOD / REFRESH_RATE;
    tmElements_t tm;
    String timeString;
    int tmp;
    if (RTC.read(tm)) {
      timeString = (get2DString(tm.Hour) + ":" + get2DString(tm.Minute) + ":" + get2DString(tm.Second));
      printLcdCenter(lcd, timeString, 0);
      timeString = (get2DString(tm.Day) + "/" + get2DString(tm.Month) + "/" + String(tmYearToCalendar(tm.Year)));
      printLcdCenter(lcd, timeString, 1);
      if(isAlarmTime(tm.Hour, tm.Minute) && tm.Second < 3)
        setMode(ALARM);
    } else {
      printLcdCenter(lcd, "RTC", 0);
      printLcdCenter(lcd, "ERROR", 1);
    }
  }

#ifdef DEBUG
  Serial.print("$$$$$$ ");
  Serial.print(isJustPressed(BUTTON1));  
  Serial.println(isJustPressedAndActive(BUTTON1));
  if(isJustPressedAndActive(BUTTON2)){
    setMode(ALARM);
  }
  if(isJustPressedAndActive(BUTTON1)){
    storeAlarmTime(15, 50, lcd);
    delay(5000);
    loadAlarmTime(lcd);
  }
  
#endif
}

void alarmSetup(LiquidCrystal_I2C lcd){
  lcd.clear();
  printLcdCenter(lcd, "IT'S TIME!.", 0);
  printLcdCenter(lcd, get2DString(getAlarmHour()) + ":" + get2DString(getAlarmMin()), 1);
  blinkLCD(lcd);
}

void alarmLoop(LiquidCrystal_I2C lcd){
  static int job = 0;
  playAlarmSound();
  if(!digitalRead(BUMPER))
    job = 1;

  if(job == 0){
    digitalWrite(MOTOR_L_F, HIGH);
    digitalWrite(MOTOR_R_F, HIGH);
    digitalWrite(MOTOR_L_B, LOW);
    digitalWrite(MOTOR_R_B, LOW);
  }
  if(job >= 1 && job <= TURNING_TIME){
    digitalWrite(MOTOR_L_B, HIGH);
    digitalWrite(MOTOR_R_B, HIGH);
    digitalWrite(MOTOR_L_F, LOW);
    digitalWrite(MOTOR_R_F, LOW);
    job = job + 1;
  }
  if(job > TURNING_TIME && job <= TURNING_TIME * 2){
    digitalWrite(MOTOR_L_F, HIGH);
    digitalWrite(MOTOR_R_B, HIGH);
    digitalWrite(MOTOR_L_B, LOW);
    digitalWrite(MOTOR_R_F, LOW);
    job = job + 1;
    if(job == TURNING_TIME * 2)
      job = 0;
  }  
  if(analogRead(LDR) > LDR_THRESHOLD ||
      isJustPressed(BUTTON1) || 
      isJustPressed(BUTTON2) || 
      isJustPressed(BUTTON3)){
    job = 0;
    setMode(NORMAL);
    stopAlarmSound();
    noBlinkLCD(lcd);
    digitalWrite(MOTOR_L_F, LOW);
    digitalWrite(MOTOR_R_F, LOW);
    digitalWrite(MOTOR_L_B, LOW);
    digitalWrite(MOTOR_R_B, LOW);
  }  
}

void updateSetup(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.print("UPDATE.");
}

void updateLoop(LiquidCrystal_I2C lcd){
  lcd.setCursor(0, 1);
}
