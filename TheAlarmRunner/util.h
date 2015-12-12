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

#ifndef TAR_UTIL
#define TAR_UTIL

void readButtons();
void updateButtonStates();
void resetInputs();
int isJustPressed(int port);
int isJustPressedAndActive(int port);
int isPressing(int port);
int wasPressed(int port);
void updateMode();
void setMode(int newMode);
int getMode();
int modeJustChanged();
void setActive();
void setInactive();
int isActive();

#endif

