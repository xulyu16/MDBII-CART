#ifndef PWM_H
#define PWM_H

#include<iostream>
#include<fstream>
#include<unistd.h>
#include<string>

using namespace std;

const string SLOT_PATH = "/sys/devices/bone_capemgr.9/slots";
const string PWM_PATH = "/sys/devices/ocp.3/pwm_test_P9_22.15";

class PWM
{
public:
	PWM();
	void set_speed(string speed);
//	string get_speed(void);
	void stop(void);

private:
	string _speed;
	string _pin_number;
	ofstream SLOTH_WRITE;
	ofstream duty;
	ofstream polarity;
	ofstream period;
	ofstream run;
};


#endif // PWM_H

