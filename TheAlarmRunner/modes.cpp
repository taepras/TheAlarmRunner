//#include <SoftwareSerial.h>

#include "DS1307RTC_CUSTOM.h"
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
  /*if(timeRefreshCount <= 0){
    timeRefreshCount = CLOCK_PERIOD / REFRESH_RATE;
    tmElements_t tm;
    //TODO: FIX THIS
    String timeString = "";
    
    int tmp;
    if (RTC.read(tm)) {
      timeString = (get2DString(tm.Hour) + ":" + get2DString(tm.Minute) + ":" + get2DString(tm.Second));
//      timeString[1] = get2DString(tm.Hour)[1];
//      timeString[3] = get2DString(tm.Minute)[0];
//      timeString[4] = get2DString(tm.Minute)[1];
//      timeString[6] = get2DString(tm.Second)[0];
//      timeString[7] = get2DString(tm.Second)[1];
      printLcdCenter(lcd, timeString, 0);
//      dateString[0] = get2DString(tm.Day)[0];
//      dateString[1] = get2DString(tm.Day)[1];
//      dateString[3] = get2DString(tm.Month)[0];
//      dateString[4] = get2DString(tm.Month)[1];
//      dateString[6] = get2DString(tm.Year)[0];
//      dateString[7] = get2DString(tm.Year)[1];
//      dateString[8] = get2DString(tm.Year)[2];
//      dateString[9] = get2DString(tm.Year)[3];
      //dateString = (const char*)(get2DString(tm.Day) + "/" + get2DString(tm.Month) + "/" + String(tmYearToCalendar(tm.Year)));
//      printLcdCenter(lcd, dateString, 1);
      if(isAlarmTime(tm.Hour, tm.Minute) && tm.Second < 3)
        setMode(ALARM);
      if(tm.Minute == 0 && tm.Second < 2)
        setMode(UPDATE);
    } else {
      printLcdCenter(lcd, "RTC", 0);
      printLcdCenter(lcd, "ERROR", 1);
    }
  }*/
  
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
  printLcdCenter(lcd, "IT'S TIME!.", 0);
  printLcdCenter(lcd, loadAlarmString(), 1);
  blinkLCD(lcd);
}

void alarmLoop(LiquidCrystal_I2C lcd){
  static int job = 0;
  playAlarmSound();
  if(isPressing(BUMPER))
    job = 1;
  
  if(job == 0){
    driveMotor(HIGH, HIGH, LOW, LOW);
  }
  
  if(job >= 1 && job <= TURNING_TIME / REFRESH_RATE){
    driveMotor(LOW, LOW, HIGH, HIGH);
    job++;
  }else if(job > TURNING_TIME / REFRESH_RATE && job <= TURNING_TIME / REFRESH_RATE * 2){
    driveMotor(HIGH, LOW, LOW, HIGH);
    job++;
  } else {
    job = 0;
  }
  if(analogRead(LDR) > LDR_THRESHOLD ||
      isJustPressed(BUTTON1) || 
      isJustPressed(BUTTON2)){
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

//extern SoftwareSerial mySerial;
//const char* request = "GET http://tae.in.th/hw/alarm.php HTTP/1.0\r\n\r\n";
void updateLoop(LiquidCrystal_I2C lcd){
  Serial.println("AT");
  delay(400);
  if(waitForSerialString("OK\r\n")){
    printLcdCenter(lcd, "WIFI MODULE", 0);
    printLcdCenter(lcd, "READY", 1);
    Serial.println("AT+CIPSTART=\"TCP\",\"tae.in.th\",80");
    delay(400);
    if(waitForSerialString("OK\r\n")){
      printLcdCenter(lcd, "CONNECTED TO", 0);
      printLcdCenter(lcd, "SERVER", 1);
      Serial.println("AT+CIPSEND=46");
      delay(400);
      if(waitForSerialString("> ")){
        Serial.print("GET http://tae.in.th/hw/alarm.php HTTP/1.0\r\n\r\n");
        delay(400);
        if(waitForSerialString("ALARM\r\n")){
          setAlarmTime(getLineFromSerial());
          printLcdCenter(lcd, "ALARM UPDATED", 0);
          digitalWrite(2, HIGH);
          delay(2000);
          digitalWrite(2, LOW);
          Serial.print(loadAlarmString());
          #ifdef DEBUG
            Serial.println("WI-FI UPDATING SUCCESS!");
          #endif
          setMode(NORMAL);
          return;
        }
      }
    }
  }
  printLcdCenter(lcd, "WI-FI UPDATE", 0);
  printLcdCenter(lcd, "ERROR!", 1);
  delay(2000);
  setMode(NORMAL);
}

void viewAlarmSetup(LiquidCrystal_I2C lcd){
  printLcdCenter(lcd, "UPCOMING ALARM", 0);
  printLcdCenter(lcd, loadAlarmString(), 1);
  Serial.println(loadAlarmString());
  delay(2000);
}

void viewAlarmLoop(LiquidCrystal_I2C lcd){
  setMode(NORMAL);
}

