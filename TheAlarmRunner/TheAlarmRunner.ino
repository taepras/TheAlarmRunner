#include <DS1307RTC.h>
#include <EEPROM.h>
#include <Time.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include "init.h"
#include "modes.h"
#include "util.h"
#include "actions.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

void setup() {
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.backlight();
  Serial.begin(9600);
  pinMode(SPEAKER, OUTPUT);
  pinMode(BUMPER, INPUT_PULLUP);
  pinMode(MOTOR_R_F, OUTPUT);
  pinMode(MOTOR_R_B, OUTPUT);
  pinMode(MOTOR_L_F, OUTPUT);
  pinMode(MOTOR_L_B, OUTPUT);  
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  printLcdCenter(lcd, "INITIALIZING", 0);
  printLcdCenter(lcd, "THE ALARM RUNNER", 1);
  delay(1000);
  lcd.noBacklight();
}

void loop() {
  delay(REFRESH_RATE);

  readButtons();

  Serial.println(isActive());  
  
  updateBacklight(lcd);
  if(isJustPressed(BUTTON1) || isJustPressed(BUTTON2) || isJustPressed(BUTTON3)){
    turnBacklightOn(lcd);
    if(!isActive()){
      resetInputs();
      Serial.print(">>>> ");  
      Serial.print(isJustPressed(BUTTON1));  
      Serial.println(isJustPressedAndActive(BUTTON1));  
      setActive();
    }  
  }
  
  switch(getMode()){
    case NORMAL: normalLoop(lcd); break;
    case ALARM: alarmLoop(lcd); break;
    case UPDATE: updateLoop(lcd); break;
  }
    
  if(modeJustChanged()){
    switch(getMode()){
      case NORMAL: normalSetup(lcd); break;
      case ALARM: alarmSetup(lcd); break;
      case UPDATE: updateSetup(lcd); break;
    }  
  }


  updateButtonStates();
  updateMode();
}
