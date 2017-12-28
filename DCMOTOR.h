#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include "GPIO.h"
#include "PWM.h"
using namespace std;

class DCMOTOR
{
 public:
  DCMOTOR();
  DCMOTOR(string pin_num1, string pin_num2);
  void turn_clockwise(void);
  void turn_aclockwise(void);
  void set_speed(string duty_cycle);
  //void set_speed(int speed);
  //int get_speed(void);    To be implemented later.

 private:
  string _pin_num1;
  string _pin_num2;
  string _speed;
  GPIO* pin1;
  GPIO* pin2;
  PWM* speed;
  
};

#endif
