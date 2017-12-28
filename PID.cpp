#include "PID.h"


PID::PID(float kp, float ki, float kd, float min, float max, float deadband) {
	// TODO Auto-generated constructor stub
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_max = max;
	_min = min;
	_error = 0;
	_last_error = 0;
	_integral = 0;
	_deadband = deadband;
}


void PID::reset()
{
	_error = 0;
	_last_error = 0;
	_integral = 0;
}

float PID::calculate(float setpoint, float currVal, float dt)
{
	float output = 0;
	_error = setpoint - currVal;
	if( fabs(_error) > _deadband )
	{
		// Calculate p
		float POut = _error * _kp;

		// Calculate i
		_integral += _error * dt;
		float IOut = _integral * _ki;

		// Calculate d
		float derivative = (_error - _last_error) / dt;
		float DOut = derivative * _kd;

		output = POut + IOut + DOut;

		// Make sure we are within limits
		if( output > 0 )
		{
			output = fmax(fmin(output, _max),_min);
		}
		else // Use same limits in the negative range
		{
			output = fmax(fmin(output, -1*_min),-1*_max);
		}

		_last_error = _error;
	}
	else
	{
		reset();
	}

	return output;
}
