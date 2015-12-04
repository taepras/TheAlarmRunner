#ifndef TAR_ACTIONS
#define TAR_ACTIONS

unsigned char isLcdBacklightOn();
void turnBacklightOn(LiquidCrystal_I2C lcd);
void updateBacklight(LiquidCrystal_I2C lcd);
void blinkLCD(LiquidCrystal_I2C lcd);
void noBlinkLCD(LiquidCrystal_I2C lcd);
void playAlarmSound();
void stopAlarmSound();
void printLcdCenter(LiquidCrystal_I2C lcd, String text, int row);
String get2DString(int num);
unsigned char getAlarmHour();
unsigned char getAlarmMin();
unsigned char isAlarmTime(int, int);
void setAlarmTime(String);
void loadAlarmTime();
void setClockTime(String);
String getLineFromSerial();
unsigned char waitForSerialLine(String waitingFor, LiquidCrystal_I2C lcd);
void driveMotor(int lf, int rf ,int lb, int rb);
unsigned char waitForSerialString(String waiting);

#endif
