#include<iostream>
#include<fstream>
#include<unistd.h>
#include<string>
#include "PWM.h"

using namespace std;

PWM::PWM()
{
	_speed = "0";
	_pin_number = "2";
	SLOTH_WRITE.open(SLOT_PATH.c_str());
	SLOTH_WRITE << "am33xx_pwm" << flush;
	SLOTH_WRITE.close();
	usleep(100);
	SLOTH_WRITE.open(SLOT_PATH.c_str());
	SLOTH_WRITE << "bone_pwm_P9_22" << flush;
	SLOTH_WRITE.close();
	polarity.open((PWM_PATH + "/polarity").c_str());
	polarity << "0" << flush;
	polarity.close();
	period.open((PWM_PATH + "/period").c_str());
	period << "20000000" << flush;
	period.close();
	duty.open((PWM_PATH + "/duty").c_str());
	duty << "0" << flush;
	duty.close();
	run.open((PWM_PATH + "/run").c_str());
	run << "1" << flush;
	run.close();
}

void PWM::set_speed(string speed) 
{
	this->_speed = speed;
	this->duty.open((PWM_PATH + "/duty").c_str());
	this->duty << speed.c_str() << flush;
	this->duty.close();
}

void PWM::stop(void) 
{
	this->_speed = "0";
	this->duty.open((PWM_PATH + "/duty").c_str());
	this->duty << "0" << flush;
	this->duty.close();
}

//string get_speed(void) 
//{
//	return this->_speed;
//}
