#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include "GPIO.h"
#include "DCMOTOR.h"

using namespace std;

DCMOTOR::DCMOTOR()
{
  this -> _pin_num1 = "7";
  this -> _pin_num2 = "20";
  //this->_speed = 0;
  pin1 = new GPIO("7", "out");
  pin2 = new GPIO("20", "out");
}

DCMOTOR::DCMOTOR(string pin_num1, string pin_num2)
{
  this ->_pin_num1 = pin_num1;
  this ->_pin_num2 = pin_num2;
  //this->_speed = 0;
  pin1 = new GPIO(this->_pin_num1, "out");
  pin2 = new GPIO(this->_pin_num2, "out");
}

void DCMOTOR::turn_clockwise(void)
{
  pin1->set();
  pin2->reset();
}

void DCMOTOR::turn_aclockwise(void)
{
  pin1->reset();
  pin2->set();
}

void DCMOTOR::set_speed(string speed)
{
  this->_speed = speed;
  this->speed->set_speed(speed);
}
