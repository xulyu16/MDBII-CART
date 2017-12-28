/*
 * PID.h
 *
 *      Author: Melody
 */

#ifndef PID_H_
#define PID_H_

#include <stdint.h>
#include <math.h>

class PID {
public:
	PID(float kp, float ki, float kd, float min, float max, float deadband = 0);
	float calculate(float setpoint, float currVal, float dt);
	void reset();
	
private:
	float _error;
	float _last_error;
	float _kp, _ki, _kd;
	float _max, _min;
	float _integral;
	float _deadband;
};

#endif /* PID_H_ */
