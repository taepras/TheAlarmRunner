#include <Arduino.h>
#include "util.h"
#include "init.h"

int pushed[3], pushing[3];
int mode = 0, oldMode = 1;
int active = 0;

void readButtons(){
  pushing[0] = !digitalRead(BUTTON1);
  pushing[1] = !digitalRead(BUTTON2);
  pushing[2] = !digitalRead(BUTTON3);
//  Serial.print(active);
//  Serial.print(pushed[0]);
//  Serial.print(pushing[0]);
//  Serial.println(realIsJustPressed(BUTTON1));
//  if((realIsJustPressed(BUTTON1) || realIsJustPressed(BUTTON2) || realIsJustPressed(BUTTON3)) && active < 2)
//    active++;
}

void updateButtonStates(){
  for(int i = 0; i < BUTTON3 - BUTTON1 + 1; i++){
    pushed[i] = pushing[i]; 
  }
}

void resetInputs(){
  for(int i = 0; i < BUTTON3 - BUTTON1 + 1; i++){
    pushed[i] = 0;
    pushing[i] = 0; 
  }
}

int isJustPressed(int port){
  return !pushed[port - BUTTON1] && pushing[port - BUTTON1];
}

int isJustPressedAndActive(int port){
  return !pushed[port - BUTTON1] && pushing[port - BUTTON1] && active;
}

int isPressing(int port){
  return pushing[port - BUTTON1];
}

int wasPressed(int port){
  return pushed[port - BUTTON1];
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
  active = 1;
}

void setInactive(){
  active = 0;
}

int isActive(){
  return active;
}

