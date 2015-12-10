#ifndef TAR_ACTIONS
#define TAR_ACTIONS

unsigned char isLcdBacklightOn();
void turnBacklightOn();
void updateBacklight();
void blinkLCD();
void noBlinkLCD();
void playAlarmSound();
void stopAlarmSound();
void printLcdCenter(const char* text, int row);
char* get2DString(int num);
unsigned char  bcdToDec(unsigned char  val);
unsigned char  decToBcd(unsigned char  val);
char* loadAlarmString();
unsigned char getAlarmHour();
unsigned char getAlarmMin();
unsigned char isAlarmTime(int, int);
void setAlarmTime(String);
//void loadAlarmTime();
void setClockTime(String);
String getLineFromSerial();
//unsigned char waitForSerialLine(String waitingFor, LiquidCrystal_I2C lcd);
unsigned char waitForSerialString(String waiting);
void driveMotor(int lf, int rf ,int lb, int rb);

#endif
