

#ifndef CARCORE_H
#define CARCORE_H

#include <Servo.h>
#include "Arduino.h"
#include <stdint.h>
#include <math.h>
#include "PID.h"

/*****Defines******/

#define END 0
#define Obstacle 1
#define Drive 2
#define turnRight 0
#define turnLeft 1
#define noTurn 2
#define CRUISE_SPEED 1400
#define MIN_SPEED 1450
#define MAX_SPEED 1350
#define MIN_ACTUATOR_VOLTAGE 0.7
#define MAX_ACTUATOR_VOLTAGE 3

#define CAR_DIRECTION_FORWARD 0
#define CAR_DIRECTION_LEFT 1
#define CAR_DIRECTION_RIGHT 2
#define CAR_DIRECTION_STOP 3
#define THRESHOLD 100 

const int readEndPin = 52; // digital pin as interrupt from break
const int goPin = 49;
const int turnrightPin = 53;                 // DUE digital pins 51 53 control turning
const int turnleftPin = 51;
const int readActuatorPin = A7; // reads 0 - 3.3v feedback 
const int steptoTurn = 400;
const int power = 500;
const int iniMotorPower = 250;



//const float lenghtCar = 1.48;
//const float diameterCar = 0.95;


/**End of Defines**/

 

class carcore
 {
 public:
 	carcore();  
 	void stabilizer(float hangle, float cangle, uint16_t deltaTime); //Function runs PIDs 
 	//void setThrottle(uint16_t val);     
 	void setDirection(uint8_t dir);    //Function sets direction
 	void navigate(int angle);               //Function provides movement
 	void updateMotors(void);           //Function updates motors
 	//float readActuatorPosition(void);
 	//void readSensors(void) Function from Lawrance
 	//void checkLR(void);
 	//void calculatePID(void); 
 	void calculateSteeringAngle(int angle);
 	//void calculateMotorSteering(void);
 	//void motorPIDcontrol(void); will be stabilizer function
 	//void motorObstacleControl(void); First bang bang function for steering
 	void stopMotors(void);
 	void actuatorPosition(void);
 	void steeringControl(void);
 	void readSensor(void);
 	//void foobar(void);
 	unsigned int currentDirection;
 

 private:
 	PID* xController;

 	//unsigned int currentDirection;
 	int sensor;
 	int state;
 	int turn;
 	float actuatorFeedback;
 	int obstacleLeft;
 	int obstacleRight;
 	float delta;
	float headingAngle;
	float currentAngle;


 	struct motors
 	{
 		Servo left;
 		unsigned int left_setpoint;
		unsigned int left_presetpoint;
 		unsigned int left_speed;
 		Servo right;
 		unsigned int right_setpoint;
		unsigned int right_presetpoint;
 		unsigned int right_speed;
 	}motors;

 	struct steeringctl
 	{
 		float actuatorActualvoltage;
 		float actuatorDesiredvoltage;
 		float VoltageToTravel;
 		float actuatorWait;
 		float absVoltageToTravel;
 		float distanceToTravel;
 	}steeringctl;
 }; 




#endif