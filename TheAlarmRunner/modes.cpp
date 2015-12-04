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
      if(tm.Minute == 0 && tm.Second < 2)
        setMode(UPDATE);
    } else {
      printLcdCenter(lcd, "RTC", 0);
      printLcdCenter(lcd, "ERROR", 1);
    }
  }
  
#ifdef DEBUG
  if(isJustPressedAndActive(BUTTON2)){
    setMode(ALARM);
  }
#endif

  if(isJustPressedAndActive(BUTTON1))
    setMode(UPDATE);
  if(isJustPressedAndActive(BUTTON2))
    setMode(VIEW_ALARM);

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
  if(isPressing(BUMPER))
    job = 1;

  if(job == 0)
    driveMotor(HIGH, HIGH, LOW, LOW);
  if(job >= 1 && job <= TURNING_TIME){
    driveMotor(LOW, LOW, HIGH, HIGH);
    job++;
  }
  if(job > TURNING_TIME && job <= TURNING_TIME * 2){
    driveMotor(HIGH, LOW, LOW, HIGH);
    job++;
  } else {
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
    driveMotor(LOW, LOW, LOW, LOW);
  }  
}

void updateSetup(LiquidCrystal_I2C lcd){
  printLcdCenter(lcd, "UPDATING", 0);
  printLcdCenter(lcd, "ALARM TIME", 1);
}

String request = "GET http://tae.in.th/hw/timealarm.php HTTP/1.0";
void updateLoop(LiquidCrystal_I2C lcd){
  Serial.println("AT");
  if(waitForSerialString("OK\r\n")){
    printLcdCenter(lcd, "WIFI MODULE", 0);
    printLcdCenter(lcd, "READY", 1);
    if(waitForSerialString("OK\r\n")){
      Serial.println("AT+CIPSTART=\"TCP\",\"tae.in.th\",80");
      if(waitForSerialString("OK\r\n")){
        printLcdCenter(lcd, "CONNECTED TO", 0);
        printLcdCenter(lcd, "SERVER", 1);
        if(waitForSerialString("OK\r\n")){
          Serial.println("AT+CIPSEND=" + (request.length() + 2));
          if(waitForSerialString("> ")){
            Serial.println(request);
            if(waitForSerialString("ALARM\r\n")){
              setAlarmTime(getLineFromSerial());
              printLcdCenter(lcd, "ALARM UPDATED", 0);
              printLcdCenter(lcd, (get2DString(getAlarmHour()) + ":" + get2DString(getAlarmMin())), 1);
              setMode(NORMAL);
              return;
            }
          }
        }
      }
    }
  }
  printLcdCenter(lcd, "WI-FI UPDATE", 0);
  printLcdCenter(lcd, "ERROR!", 1);
  setMode(NORMAL);
}

void viewAlarmSetup(LiquidCrystal_I2C lcd){
  printLcdCenter(lcd, "UPCOMING ALARM", 0);
  printLcdCenter(lcd, (get2DString(getAlarmHour()) + ":" + get2DString(getAlarmMin())), 1);
}

void viewAlarmLoop(LiquidCrystal_I2C lcd){
  setMode(NORMAL);
}

