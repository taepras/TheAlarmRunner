//#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
//#include <Time.h>
#include "modes.h"
#include "util.h"
#include "init.h"
#include "actions.h"

extern LiquidCrystal_I2C lcd;

void normalSetup(){
	lcd.clear();
}

void normalLoop(){
  static short timeRefreshCount = 0;
  timeRefreshCount--;
  if(timeRefreshCount <= 0){
    timeRefreshCount = CLOCK_PERIOD / REFRESH_RATE;
//    tmElements_t tm;
    //TODO: FIX THIS
    char timeString[] = "00:00:00";
    char dateString[] = "00/00/0000";
    //if (RTC.read(tm)) {

      byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
      
      Wire.beginTransmission(DS1307_I2C_ADDRESS);
      Wire.write(decToBcd(0));
      Wire.endTransmission();
      Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
      second = bcdToDec(Wire.read() & 0x7f);
      minute = bcdToDec(Wire.read());
      hour = bcdToDec(Wire.read() & 0x3f);
      dayOfWeek = bcdToDec(Wire.read());
      dayOfMonth = bcdToDec(Wire.read());
      month = bcdToDec(Wire.read());
      year = bcdToDec(Wire.read());
      lcd.setCursor(0, 0);
      lcd.print("   ");
      lcd.print(get2DString(hour));
      lcd.print(":");
      lcd.print(get2DString(minute));
      lcd.print(":");
      lcd.print(get2DString(second));
      lcd.print("   ");
      lcd.setCursor(0, 1);
      lcd.print("  ");
      lcd.print(get2DString(month));
      lcd.print("/");
      lcd.print(get2DString(dayOfMonth));
      lcd.print("/");
      lcd.print(year,DEC);
      lcd.println("  ");

      Serial.print("   ");
      Serial.print(get2DString(hour));
      Serial.print(":");
      Serial.print(get2DString(minute));
      Serial.print(":");
      Serial.print(get2DString(second));
      Serial.println("   ");
      Serial.print("  ");
      Serial.print(get2DString(month));
      Serial.print("/");
      Serial.print(get2DString(dayOfMonth));
      Serial.print("/");
      Serial.print(year,DEC);
      Serial.print("  ");
      
//      timeString[0] = get2DString(tm.Hour)[0];
//      timeString[1] = get2DString(tm.Hour)[1];
//      timeString[3] = get2DString(tm.Minute)[0];
//      timeString[4] = get2DString(tm.Minute)[1];
//      timeString[6] = get2DString(tm.Second)[0];
//      timeString[7] = get2DString(tm.Second)[1];
//      printLcdCenter(timeString, 0);
//      dateString[0] = get2DString(tm.Day)[0];
//      dateString[1] = get2DString(tm.Day)[1];
//      dateString[3] = get2DString(tm.Month)[0];
//      dateString[4] = get2DString(tm.Month)[1];
//      dateString[6] = get2DString(tm.Year)[0];
//      dateString[7] = get2DString(tm.Year)[1];
//      dateString[8] = get2DString(tm.Year)[2];
//      dateString[9] = get2DString(tm.Year)[3];
//      //dateString = (const char*)(get2DString(tm.Day) + "/" + get2DString(tm.Month) + "/" + String(tmYearToCalendar(tm.Year)));
//      printLcdCenter(dateString, 1);
//      if(isAlarmTime(hour, minute) && second < 3)
//        setMode(ALARM);
//      if(minute == 0 && second < 3)
//        setMode(UPDATE);
//    } else {
//      printLcdCenter("RTC", 0);
//      printLcdCenter("ERROR", 1);
//    }
  }
  
#ifdef DEBUG_ALARM
  if(isJustPressedAndActive(BUTTON2)){
    setMode(ALARM);
  }
#endif

  if(isJustPressedAndActive(BUTTON1))
    setMode(UPDATE);
  if(isJustPressedAndActive(BUTTON2))
    setMode(VIEW_ALARM);

}

void alarmSetup(){
  printLcdCenter("IT'S TIME!.", 0);
  printLcdCenter(loadAlarmString(), 1);
  blinkLCD();
}

void alarmLoop(){
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
    noBlinkLCD();
    driveMotor(LOW, LOW, LOW, LOW);
  }  
}

void updateSetup(){
  printLcdCenter("UPDATING", 0);
  printLcdCenter("ALARM TIME", 1);
}


//const char* request = "GET http://tae.in.th/hw/alarm.php HTTP/1.0\r\n\r\n";
void updateLoop(){
  Serial.println("AT");
  if(waitForSerialString("OK\r\n")){
    printLcdCenter("WIFI MODULE", 0);
    printLcdCenter("READY", 1);
    Serial.println("AT+CIPSTART=\"TCP\",\"tae.in.th\",80");
    if(waitForSerialString("OK\r\n")){
      printLcdCenter("CONNECTED TO", 0);
      printLcdCenter("SERVER", 1);
      Serial.println("AT+CIPSEND=46");
      if(waitForSerialString("> ")){
        Serial.print("GET http://tae.in.th/hw/alarm.php HTTP/1.0\r\n\r\n");
        if(waitForSerialString("ALARM\r\n")){
          setAlarmTime(getLineFromSerial());
          printLcdCenter("ALARM UPDATED", 0);
          printLcdCenter(loadAlarmString(), 1);
          #ifdef SERIAL_DEBUG
            Serial.print(loadAlarmString());
            Serial.println("WI-FI UPDATING SUCCESS!");
          #endif
          setMode(NORMAL);
          return;
        }
      }
    }
  }
  printLcdCenter("WI-FI UPDATE", 0);
  printLcdCenter("ERROR!", 1);
  delay(2000);
  setMode(NORMAL);
}

void viewAlarmSetup(){
  printLcdCenter("UPCOMING ALARM", 0);
  printLcdCenter(loadAlarmString(), 1);
  Serial.print(loadAlarmString());
  delay(2000);
}

void viewAlarmLoop(){
  setMode(NORMAL);
}

