#ifndef TAR_ACTIONS
#define TAR_ACTIONS

void printLcdStatus();
void turnBacklightOn(LiquidCrystal_I2C lcd);
void updateBacklight(LiquidCrystal_I2C lcd);
void blinkLCD(LiquidCrystal_I2C lcd);
void noBlinkLCD(LiquidCrystal_I2C lcd);
void playAlarmSound();
void stopAlarmSound();
void printLcdCenter(LiquidCrystal_I2C lcd, String text, int row);
String get2DString(int num);
int getAlarmHour();
int getAlarmMin();
int isAlarmTime(int, int);
void storeAlarmTime(int, int, LiquidCrystal_I2C);
void loadAlarmTime(LiquidCrystal_I2C lcd);

#endif
