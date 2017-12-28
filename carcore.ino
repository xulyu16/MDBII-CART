#include <carcore.h>
#include <PID.h>

byte angle;
byte old_angle = 90;
//
int interruptCtr = 1;
int S = 0;




void setup() 
{
/************************/
  Serial.begin(115200);
  Serial3.begin(115200);
  analogReadResolution(12);
/***********************/
  pinMode(readEndPin, INPUT_PULLUP);
  pinMode(goPin, INPUT_PULLUP);
  
  /* turn on the timer clock in the power management controller */
  pmc_set_writeprotect(false);     // disable write protection for pmc registers
  pmc_enable_periph_clk(ID_TC7);   // enable peripheral clock TC7

  /* we want wavesel 01 with RC */
  TC_Configure(/* clock */TC2,/* channel */1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  TC_SetRC(TC2, 1, 131200);
  TC_Start(TC2, 1);

  // enable timer interrupts on the timer
  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS; // IER = interrupt enable register
  TC2->TC_CHANNEL[1].TC_IDR = ~TC_IER_CPCS; // IDR = interrupt disable register

  /* Enable the interrupt in the nested vector interrupt controller */
  /* TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number (=(1*3)+1) for timer1 channel1 */
  NVIC_EnableIRQ(TC7_IRQn);
}

void readSensor(void)
{
 if(Serial3.available())
  {
    Serial3.flush();
    angle = Serial3.read();
    Serial.print("Angle is: \n");
    Serial.print(angle);
  }
}



carcore* car = new carcore;


void loop() 
{
      readSensor();
      car->navigate(angle); 
      car->stabilizer(angle, old_angle, 10); 
      car->updateMotors();  //motors will be updated every time after calculations
      old_angle = angle;
  
 Serial.print("END \n");

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT HANDLERS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TC7_Handler()
{
  // We need to get the status to clear it and allow the interrupt to fire again
  TC_GetStatus(TC2, 1);
  //Serial.print("I AM IN FIRST");

  if (digitalRead(readEndPin) == 0) 
  {
    Serial.print("INTERRUPT PIN IS \n");
    Serial.print(digitalRead(readEndPin));
    car->currentDirection = CAR_DIRECTION_STOP;
  }


  if ( interruptCtr++ >= 6 )
  {
    interruptCtr = 1;
    S = !S; // are we flashing S or O
    if ( S ) // set time till next interrupt
      TC_SetRC(TC2, 1, 131200); // 131200 / 656000 = .2 seconds
    else
      TC_SetRC(TC2, 1, 656000); // 656000/ 656000 = 1 second
  }
}



