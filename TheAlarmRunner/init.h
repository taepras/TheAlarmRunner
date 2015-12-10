#ifndef TAR_INIT
#define TAR_INIT

// --------------------- config ---------------------

//#define DEBUG

#define LCD_LIGHT_TIME 8000
#define LDR_THRESHOLD 850
#define TURNING_TIME 1000
#define REFRESH_RATE 20
#define BLINK_PERIOD 600
#define CLOCK_PERIOD 1000

#define SERIAL_TIMEOUT 10000

// ------------------- end config -------------------

#define DS1307_I2C_ADDRESS 0x68
#define LCD_I2C_ADDRESS 0x27

#define STR_BUFF_SIZE 128

#define NORMAL 0
#define ALARM 1
#define UPDATE 2
#define VIEW_ALARM 3

#define RX 0
#define TX 1
#define SPEAKER 3
#define BUMPER 4
#define MOTOR_R_F 6
#define MOTOR_R_B 5
#define MOTOR_L_F 7
#define MOTOR_L_B 8
#define BUTTON1 9
#define BUTTON2 10
#define BUTTON3 11
#define LDR A2

#endif
