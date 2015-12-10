//#include <SoftwareSerial.h>

#include "DS1307RTC_CUSTOM.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Time.h>
#include "modes.h"
#include "util.h"
#include "init.h"
#include "actions.h"

void normalSetup(){
	printLcdCenter("", 0);
  printLcdCenter("", 1);
}

void normalLoop(){
  static short timeRefreshCount = 0;
  timeRefreshCount--;
  if(timeRefreshCount <= 0){
    timeRefreshCount = CLOCK_PERIOD / REFRESH_RATE;
    tmElements_t tm;
    //TODO: FIX THIS
    char timeString[11] = "";
    char chtmp[8];
    int tmp;
    if (RTC.read(tm)) {
      strcat(timeString, get2DString(tm.Hour));
      strcat(timeString, ":");
      strcat(timeString, get2DString(tm.Minute));
      strcat(timeString, ":");
      strcat(timeString, get2DString(tm.Second));
      printLcdCenter(timeString, 0);
      strcpy(timeString, "");
      strcat(timeString, get2DString(tm.Day));
      strcat(timeString, "/");
      strcat(timeString, get2DString(tm.Month));
      strcat(timeString, "/");
      strcat(timeString, itoa(tmYearToCalendar(tm.Year), chtmp, 10));
      printLcdCenter(timeString, 1);
//      timeString = String(get2DString(tm.Day)) + "/" + String(get2DString(tm.Month)) + "/" + String(tmYearToCalendar(tm.Year));
//      printLcdCenter(timeString, 1);
      if(isAlarmTime(tm.Hour, tm.Minute) && tm.Second < 3)
        setMode(ALARM);
      else if(tm.Minute == 0 && tm.Second < 2){
        turnBacklightOn();
        setMode(UPDATE);
      }
    } else {
      printLcdCenter("RTC", 0);
      printLcdCenter("ERROR", 1);
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
  
  if(job == 0){
//    driveMotor(HIGH, HIGH, LOW, LOW);
    digitalWrite(MOTOR_L_F, HIGH);
    digitalWrite(MOTOR_R_F, HIGH);
    digitalWrite(MOTOR_L_B, LOW);
    digitalWrite(MOTOR_R_B, LOW);
  }
  
  if(job >= 1 && job <= TURNING_TIME / REFRESH_RATE){
//    driveMotor(LOW, LOW, HIGH, HIGH);
    digitalWrite(MOTOR_L_F, LOW);
    digitalWrite(MOTOR_R_F, LOW);
    digitalWrite(MOTOR_L_B, HIGH);
    digitalWrite(MOTOR_R_B, HIGH);
    job++;
  }else if(job > TURNING_TIME / REFRESH_RATE && job <= TURNING_TIME / REFRESH_RATE * 2){
//    driveMotor(HIGH, LOW, LOW, HIGH);
    digitalWrite(MOTOR_L_F, HIGH);
    digitalWrite(MOTOR_R_F, LOW);
    digitalWrite(MOTOR_L_B, LOW);
    digitalWrite(MOTOR_R_B, HIGH);
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
    noBlinkLCD();
//    driveMotor(LOW, LOW, LOW, LOW);
    digitalWrite(MOTOR_L_F, LOW);
    digitalWrite(MOTOR_R_F, LOW);
    digitalWrite(MOTOR_L_B, LOW);
    digitalWrite(MOTOR_R_B, LOW);
  }  
}

void updateSetup(){
  printLcdCenter("UPDATING", 0);
  printLcdCenter("ALARM TIME", 1);
}

//extern SoftwareSerial mySerial;
//const char* request = "GET http://tae.in.th/hw/alarm.php HTTP/1.0\r\n\r\n";
void updateLoop(){
  Serial.println("AT");
  if(getWebPage("GET http://tae.in.th/hw/alarm.php HTTP/1.0\r\n\r\n")){
    if(waitForSerialString("ALARM\r\n")){
      setAlarmTime(getLineFromSerial());
      printLcdCenter("ALARM UPDATED", 0);
      printLcdCenter(loadAlarmString(), 1);
      delay(2000);
      setMode(NORMAL);
      return;
    }
  }
  printLcdCenter("WI-FI UPDATE", 0);
  printLcdCenter("ERROR!", 1);
  delay(2000);
  setMode(NORMAL);
}

short viewAlarmTimeLeft;
void viewAlarmSetup(){
  printLcdCenter("UPCOMING ALARM", 0);
  printLcdCenter(loadAlarmString(), 1);
  viewAlarmTimeLeft = 2000;
}

void viewAlarmLoop(){
  Serial.println(viewAlarmTimeLeft);
  viewAlarmTimeLeft -= REFRESH_RATE;
  if(isJustPressedAndActive(BUTTON2))
    setMode(SET_ALARM);
  else if(viewAlarmTimeLeft <= 0)
    setMode(NORMAL);
}

int currentDigit = 0;
char newAlarmString[8];
extern LiquidCrystal_I2C lcd;
void setAlarmSetup(){
  printLcdCenter("SET ALARM", 0);
  for(int i = 0; i < 8; i++)
    newAlarmString[i] = '\0';
  strcat(newAlarmString, loadAlarmString());
  printLcdCenter(newAlarmString, 1);
  currentDigit = 0;
  lcd.blink();
  lcd.setCursor(6, 1);
}

void setAlarmLoop(){
  if(isJustPressedAndActive(BUTTON1) || currentDigit == 2){
    currentDigit++;
    lcd.setCursor(6 + currentDigit, 1);
  }
  if(isJustPressedAndActive(BUTTON2)){
    switch(currentDigit){
    case 0: newAlarmString[currentDigit] = (newAlarmString[currentDigit] < '2') ? newAlarmString[currentDigit] + 1 : '0'; break;
    case 1:
      newAlarmString[currentDigit] = (newAlarmString[0] < '2') ? 
                                        ((newAlarmString[currentDigit] < '9') ? newAlarmString[currentDigit] + 1 : '0') : 
                                        ((newAlarmString[currentDigit] < '3') ? newAlarmString[currentDigit] + 1 : '0'); 
      break;
    case 3: newAlarmString[currentDigit] = (newAlarmString[currentDigit] < '5') ? newAlarmString[currentDigit] + 1 : '0'; break;
    case 4: newAlarmString[currentDigit] = (newAlarmString[currentDigit] < '9') ? newAlarmString[currentDigit] + 1 : '0'; break;
    }
    printLcdCenter(newAlarmString, 1);
    lcd.setCursor(6 + currentDigit, 1);
  }
  if(currentDigit == 5){
    lcd.noBlink();
    setAlarmTime(newAlarmString);
    delay(10);
    printLcdCenter("ALARM SET", 0);
    char request[64] = "GET http://tae.in.th/hw/setalarm.php?h=";
    strcat(request, get2DString(getAlarmHour()));
    strcat(request, "&m=");
    strcat(request, get2DString(getAlarmMin()));
    strcat(request, " HTTP/1.0\r\n\r\n");
    Serial.println("AT");
    if(getWebPage(request) && waitForSerialString("OK\r\n")){
      setAlarmTime(getLineFromSerial());
      printLcdCenter("ALARM SYNCED", 0);
    }else
      printLcdCenter("ALARM NOT SYNCED", 0);
    delay(2000);
    setMode(NORMAL);
  }
}
