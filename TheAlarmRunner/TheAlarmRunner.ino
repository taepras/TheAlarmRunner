#include "DS1307RTC_CUSTOM.h"
//#include <SoftwareSerial.h>
#include <Time.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include "init.h"
#include "modes.h"
#include "util.h"
#include "actions.h"

//SoftwareSerial mySerial(11,12);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

void setup() {
//  setAlarmTime("21:19");
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.backlight();
  Serial.begin(9600);
//  mySerial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  pinMode(SPEAKER, OUTPUT);
  pinMode(BUMPER, INPUT_PULLUP);
  pinMode(MOTOR_R_F, OUTPUT);
  pinMode(MOTOR_R_B, OUTPUT);
  pinMode(MOTOR_L_F, OUTPUT);
  pinMode(MOTOR_L_B, OUTPUT);  
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  printLcdCenter("INITIALIZING", 0);
  printLcdCenter("THE ALARM RUNNER", 1);
  delay(2000);
  turnBacklightOn();
}

void loop() {
  delay(REFRESH_RATE);

  if(isLcdBacklightOn())
    lcd.backlight();
  else
    lcd.noBacklight();
    
  readButtons();
  
  updateBacklight();
  if(isJustPressed(BUTTON1) || isJustPressed(BUTTON2) || isJustPressed(BUTTON3)){
    turnBacklightOn(); 
  }
  
  switch(getMode()){
    case NORMAL: normalLoop(); break;
    case ALARM: alarmLoop(); break;
    case UPDATE: updateLoop(); break;
    case VIEW_ALARM: viewAlarmLoop(); break;
  }
    
  if(modeJustChanged()){
    switch(getMode()){
      case NORMAL: normalSetup(); break;
      case ALARM: alarmSetup(); break;
      case UPDATE: updateSetup(); break;
      case VIEW_ALARM: viewAlarmSetup(); break;
    }  
  }

  updateButtonStates();
  updateMode();
}
