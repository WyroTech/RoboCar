#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

/*Motor*/
class Motor
{
public:
  void init();
  void drive(boolean direction_A, uint8_t speed_A,
                                     boolean direction_B, uint8_t speed_B,
                                     boolean controlED
  );
private:
//TB6612
#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3
public:
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};


#endif //MOTOR_H
