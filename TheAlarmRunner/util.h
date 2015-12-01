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

