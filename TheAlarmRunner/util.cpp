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

#include <Arduino.h>
#include "util.h"
#include "init.h"

int pushed, pushing;
int mode = 0, oldMode = 1;
int active = 0;

void readButtons(){
  pushing = 0;
  for(int i = 0; i < 14; i++)
    pushing |= (!digitalRead(i) << i);
  if(isJustPressed(BUTTON1) || isJustPressed(BUTTON2) || isJustPressed(BUTTON3))
    active++;
}

void updateButtonStates(){
  pushed = pushing;
}

void resetInputs(){
  pushed = 0;
  pushing = 0;
}

int isJustPressed(int port){
  return !((pushed >> port) & 1) && ((pushing >> port) & 1);
}

int isJustPressedAndActive(int port){
  return !((pushed >> port) & 1) && ((pushing >> port) & 1) && active > 0;
}

int isPressing(int port){
  return (pushing >> port) & 1;
}

int wasPressed(int port){
  return (pushed >> port) & 1;
}

void updateMode(){
  oldMode = mode;
}

void setMode(int newMode){
  mode = newMode;
}

int getMode(){
  return mode;
}

int modeJustChanged(){
  return mode != oldMode;
}

void setActive(){
  active++;
}

void setInactive(){
  active = 0;
}

int isActive(){
  return active;
}

