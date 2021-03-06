/*******************************************************
 * Copyright (C) 
 * Thanawit Prasongpongchai
 * Phatrasek JIrabovonvisut
 * Sirawit Yeesong
 * Phoomin Leejongpermpoon
 * 
 * This file is part of The Alarm Runner Project.
 * for 2110363 HW SYN LAB I
 * 
 * Please do not copy/use these code without express permission
 *******************************************************/

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
const char* get2DString(int num);
const char* loadAlarmString();
unsigned char getAlarmHour();
unsigned char getAlarmMin();
unsigned char isAlarmTime(int, int);
void setAlarmTime(const char*);
//void loadAlarmTime();
//void setClockTime(String);
const char* getLineFromSerial();
//unsigned char waitForSerialLine(String waitingFor, LiquidCrystal_I2C lcd);
unsigned char waitForSerialString(const char* waiting);
void driveMotor(int lf, int rf ,int lb, int rb);
unsigned char getWebPage(const char*);

#endif
