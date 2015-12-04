#include <DS1307RTC.h>
#include <Time.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  setTime(22,47,00,4,12,2015);
  RTC.set(now());
  lcd.init();
  lcd.print("YAY");
  lcd.backlight();
}

void loop() {

}
