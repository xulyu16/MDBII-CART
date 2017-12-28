
#include "carcore.h"
#include "Arduino.h"
#include "stdlib.h"


carcore::carcore()
{
	this->currentDirection = CAR_DIRECTION_FORWARD;
	xController = new PID(0.01, 0.001, 0.1, 0, 500, 3); // 3 degree deadband

  // Start motors
  this->motors.left.attach(9);
  this->motors.right.attach(10);
  this->motors.left.writeMicroseconds(1500);
  this->motors.right.writeMicroseconds(1500);

  //Initialize Struct
  this->steeringctl.actuatorActualvoltage = 0;
  this->steeringctl.actuatorDesiredvoltage = 0;
  //this->steeringctl.amounttoTurn = 0;

  // Start Actuator
  pinMode(turnleftPin, OUTPUT);
  pinMode(turnleftPin, LOW);
  pinMode(turnrightPin, OUTPUT);
  pinMode(turnrightPin, LOW);
}



void carcore::stopMotors(void)
{
	bool flag_left = true;
	bool flag_right = true;

  		
	while(flag_right == true || flag_left == true)
	{
       if(this->motors.left_speed >= 1500) { this->motors.left.writeMicroseconds(1500); flag_left = false; }
         else if (this->motors.left_speed < 1500) {this ->motors.left_speed = this->motors.left_speed + 50; this->motors.left.writeMicroseconds(this->motors.left_speed);Serial.print("Motor left speed is"); Serial.print(this->motors.left_speed);}


       if(this->motors.right_speed >= 1500) { motors.right.writeMicroseconds(1500); flag_right = false ;}
		 else if (this->motors.right_speed < 1500) {this->motors.right_speed = this->motors.right_speed + 50; this->motors.right.writeMicroseconds(this->motors.left_speed); Serial.print("Motor left speed is"); Serial.print(this->motors.left_speed);}

      delay(100);
    }
}







void carcore::calculateSteeringAngle(int angle)
{


	this->steeringctl.actuatorDesiredvoltage = (angle*0.8/180) ; 	// Calculates distance we want to go (angle*3.3/180) = Desired voltage, that serves as an input
	
	this->steeringctl.actuatorActualvoltage = ((3.3 / 4094)*(analogRead(readActuatorPin)));	 //Gives distance that we are
	//distance = (voltage - (4/5))/(11/700)
	Serial.print("Value of voltage from actuator is \n");
	Serial.print((3.3 / 4094)*(analogRead(readActuatorPin)));
	delay(5000);	
	Serial.print("Desired voltage is: \n");
	Serial.print(this->steeringctl.actuatorDesiredvoltage);
	delay(5000);
	this->steeringctl.VoltageToTravel = (this->steeringctl.actuatorDesiredvoltage-this->steeringctl.actuatorActualvoltage); // calculates distance to travel
	this->steeringctl.absVoltageToTravel = abs(constrain(this->steeringctl.VoltageToTravel, MIN_ACTUATOR_VOLTAGE, MAX_ACTUATOR_VOLTAGE));
	this->steeringctl.distanceToTravel = (this->steeringctl.absVoltageToTravel - (4/5))*(175/3);
	this->steeringctl.actuatorWait = (21.759*this->steeringctl.distanceToTravel+241.71); //Transforms it to a wait to feed the actuator
	Serial.print("Calculated distance to travel is: \n");
	Serial.print(this->steeringctl.distanceToTravel);
	delay(1000);
	Serial.print("Calculated Wait is: \n");
	Serial.print(this->steeringctl.actuatorWait);
	delay(5000);

	if(this->steeringctl.VoltageToTravel < 0) // Sees which direction we are going
	{
		Serial.print("ACTUATOR turning Right \n");
        digitalWrite(turnrightPin, HIGH);   // sets the car to turn right
        delay(this->steeringctl.actuatorWait);                  // waits

        digitalWrite(turnrightPin, LOW);    // stops turning
     }
	

	else if(this->steeringctl.VoltageToTravel > 0)  // Sees which direction we are going
	{
		Serial.print("ACTUATOR turning Left \n");
        digitalWrite(turnleftPin, HIGH);   // sets the car to turn right
        delay(this->steeringctl.actuatorWait);                      // waits
        digitalWrite(turnleftPin, LOW);    // stops turning		
	}
	else
	{
		Serial.print("We do nothing \n");
	}


	
}


void carcore::updateMotors(void)
{
	motors.left.writeMicroseconds(motors.left_setpoint);
	motors.right.writeMicroseconds(motors.right_setpoint);
}



void carcore::stabilizer(float hangle, float cangle, uint16_t deltaTime)
{
	Serial.print("Stabilizer Function Started!!!!\n");
	this->headingAngle = hangle;
	this->currentAngle = cangle;
	Serial.print("HeadingAngle: \n");
	Serial.print(this->headingAngle);
	Serial.print("CurrentAngle:\n ");
	Serial.print(this->currentAngle);
	float correction = xController->calculate(this->headingAngle, this->currentAngle, 10);
	Serial.print("Correction is: \n");
	Serial.print(correction);

	this->motors.left_setpoint = constrain((this->motors.left_presetpoint + correction),MIN_SPEED,MAX_SPEED); //Make sure it is between range
	this->motors.right_setpoint = constrain((this->motors.right_presetpoint - correction),MIN_SPEED,MAX_SPEED);
	Serial.print("Left motor speed after PID:\n");
	Serial.print(this->motors.left_setpoint);
	Serial.print("Right motor speed after PID:\n");
	Serial.print(this->motors.right_setpoint);
	Serial.print("Stabilizer Function has finished !!!!\n"); 
}

void carcore::navigate(int angle)
{
	Serial.print("Navigate Function Started!!! \n");
	this->headingAngle = angle;
	do {
		Serial.print("Car is waiting to start\n");
		this->motors.left.writeMicroseconds(1500);
		this->motors.right.writeMicroseconds(1500);
		} while (digitalRead(goPin) == 1);

	
		if(this->headingAngle  <= 80 )
		{
			this->currentDirection = CAR_DIRECTION_LEFT;
		}
		else if(this->headingAngle > 80 && this->headingAngle < 100 )
		{
			this->currentDirection = CAR_DIRECTION_FORWARD;
		}
		else
		{
			this->currentDirection = CAR_DIRECTION_RIGHT;
		}
	
	switch (this->currentDirection)
	{


	case CAR_DIRECTION_LEFT:
		Serial.print("Car is Moving left\n");
		stopMotors(); // Soft stop instead of instant as written below, also good to test it
		calculateSteeringAngle(this->headingAngle);  
        this->motors.right_presetpoint = constrain((CRUISE_SPEED - ((3.1)*(this->headingAngle))),MIN_SPEED,MAX_SPEED);
        this->motors.left_presetpoint = constrain((CRUISE_SPEED + ((3.1)*(this->headingAngle))),MIN_SPEED,MAX_SPEED);
		break;

	case CAR_DIRECTION_FORWARD:
		Serial.print("Car is Moving forward\n");
		calculateSteeringAngle(this->headingAngle);
		this->motors.left_presetpoint = CRUISE_SPEED; //Make it smoother
		this->motors.right_presetpoint = CRUISE_SPEED; //Make it smoother
		calculateSteeringAngle(this->headingAngle);
		break;

	case CAR_DIRECTION_RIGHT:
		Serial.print("Car is Moving right\n");
		stopMotors(); // Soft stop instead of instant as written below, also good to test it
	    this->motors.right_presetpoint = constrain((CRUISE_SPEED + ((this->headingAngle /*- 99*/)*3.1)),MIN_SPEED,MAX_SPEED);
        this->motors.left_presetpoint = constrain((CRUISE_SPEED - ((this->headingAngle /*- 99*/)*3.1)),MIN_SPEED,MAX_SPEED);
        calculateSteeringAngle(this->headingAngle);
		break;
	
	case CAR_DIRECTION_STOP:
      Serial.print("PIN IS\n");
      Serial.print(digitalRead(readEndPin));
      stopMotors();
      Serial.print(currentDirection);
      delay(5000);
      currentDirection = CAR_DIRECTION_STOP;
      break;	
	}
}






