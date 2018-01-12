#include <carcore.h>
#include <PID.h>


int angle = 90;// initialise the angle to take to be 90 which is stratight
int old_angle= 0;
carcore* car = new carcore();
void setup() {
  Serial3.begin(115200);

}

void loop() {
  
  car->navigate(angle); // 30 will be replaced with hangle when lawrance implemented
  old_angle = angle;
  readAngle();
  car->stabilizer(old_angle, angle, 10); //90 will be replaced with cangle
  car->updateMotors();  //motors will be updated every time after calculations
  delay(20); // 20ms of delay maybe is ok
}

void readAngle(void)
{

  byte a;
if(Serial3.available())
  {
    Serial3.flush();
    a = Serial3.read();

    if (a != 249){       //if undefined, outputs 249;

       angle = (int)a;
    }

}
}
