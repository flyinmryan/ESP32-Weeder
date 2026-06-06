#ifndef CONFIG_H
#define CONFIG_H

// Camera Pins (Standard ESP32-S3 Camera Pinout)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       13
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     14 // Double check this, S3 often uses 14

// Robot (Wheels) - Phase/Enable
#define WHEEL_L_PHASE     15
#define WHEEL_L_ENABLE    16
#define WHEEL_R_PHASE     17
#define WHEEL_R_ENABLE    18

// Z-Axis - Phase/Enable
#define ZAXIS_L_PHASE     19
#define ZAXIS_L_ENABLE    20
#define ZAXIS_R_PHASE     21
#define ZAXIS_R_ENABLE    35

// Servos (PWM)
#define SERVO_LID_PIN     41
#define SERVO_TILT_PIN    42
#define SERVO_ROLL_PIN    45
#define SERVO_SWING_PIN   46

// Sun Tracker (Photoresistors)
#define QUAD1_PIN         4
#define QUAD2_PIN         5
#define QUAD3_PIN         6
#define QUAD4_PIN         7

// Other
#define MONITOR_SPEED     115200

#endif // CONFIG_H
