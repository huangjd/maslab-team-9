#ifndef __CONFIG_H__
#define __CONFIG_H__

#define NUM_PINS 34
#define BAUD_RATE 115200
#define ADC_RESOLUTION 8
#define PWM_RESOLUTION 8
#define PWM0_FREQUENCY 488.28
#define PWM1_FREQUENCY 488.28
#define PWM2_FREQUENCY 488.28
#define FTM0PIN 5
#define FTM1PIN 6
#define FTM2PIN 25

#define RX_MAX 64
#define TX_MAX 64

// PIN

#define SHORT_IR_L        34
#define SHORT_IR_B        35
#define SHORT_IR_R        36
#define SHORT_IR_F        37

#define WHEEL_DIR_L       0
#define WHEEL_DIR_R       1
#define TIMER1_INTERRUPT  2
#define WHEEL_SPEED_L     3
#define WHEEL_SPEED_R     4
#define ULTRA_SHORT_IR_L  5
#define ULTRA_SHORT_IR_B  6
#define ULTRA_SHORT_IR_R  7
#define ULTRA_SHORT_IR_F  8
#define TIMER2_INTERRUPT  9
#define GYRO_CS           10
#define GYRO_MOSI         11
#define GYRO_MISO         12
#define ENCODER_L_A       24
#define ENCODER_L_B       25
#define ENCODER_R_A       26
#define ENCODER_R_B       27
#define STEPPER_R_EN      28

#define CLAMP_L           23
#define CLAMP_R           22
#define CONTAINER_L_1     21
#define CONTAINER_L_2     20
#define COLOR_L_CLK       19
#define COLOR_L_DATA      18
#define CONTAINER_R_1     17
#define CONTAINER_R_2     16
#define STEPPER_L_CLK     15
#define STEPPER_L_DIR     14
#define GYRO_SCK          13
#define STEPPER_L_EN      33
#define STEPPER_R_CLK     32
#define STEPPER_R_DIR     31
#define COLOR_R_CLK       30
#define COLOR_R_DATA      29

// IRQL
#define TIMER1_IRQL 129
#define TIMER2_IRQL 130

#define LEFT  0
#define RIGHT 1

#define OPEN  0
#define CLOSE 2

#define UP    2
#define DOWN  0

#define FORWARD_CONSTANT   20
#define TURN_CONSTANT      20

#define STEPPER_DELAY      1
#define STEPPER_STEP_1     220
#define STEPPER_STEP_2     440

#define DOOR_MOTOR_SCALE   10
#define DOOR_MOTOR_CONSATANT 10

#define CLAMP_CLOSE           0
#define CLAMP_OPEN            45

#define EMERGENCY_BACKUP_AMOUNT 150

#endif // __CONFIG_H__

