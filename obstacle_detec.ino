

//:::::::::::::::stepper motor:::::::::::::::::
#include <Stepper.h>

//::::::::::::::::lidar lite::::::::::::::::
#include <Wire.h>


//#include <LIDARLite.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

//_______________________________________________________________________________________________________________________________
//*******************************************************************************************************************************
#include <NewPing.h>


// trigger and echo pins for each sensor
#define SONAR1 2
#define SONAR2 3
#define SONAR3 4
#define SONAR4 5
#define SONAR5 6
#define SONAR6 7
#define MAX_DISTANCE 500 // maximum distance for sensors
#define NUM_SONAR 6 // number of sonar sensors

  const int numReadings = 10;

  NewPing sonar[NUM_SONAR] = { // array of sonar sensor objects
  NewPing(SONAR1, SONAR1, MAX_DISTANCE),
  NewPing(SONAR2, SONAR2, MAX_DISTANCE),
  NewPing(SONAR3, SONAR3, MAX_DISTANCE),
  NewPing(SONAR4, SONAR4, MAX_DISTANCE),
  NewPing(SONAR5, SONAR5, MAX_DISTANCE),
  NewPing(SONAR6, SONAR6, MAX_DISTANCE)
};

int distance[NUM_SONAR]; // array stores distances for each
                         // sensor in cm


int space;
int m = 300; //maximum set pingable distance


//_______________________________________________________________________________________________________________________________
//*******************************************************************************************************************************

//LIDARLite myLidarLite;

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution


//this hadles the oto_switch
int IR_LED_PIN       = 12;
int IR_DETECTOR_PIN  = 13;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int pos = 0;
//int distance = 0;
int dists[100];
int data[25];


int ang;

int max_dist = 150;

bool myflag = true;
int angle;

void setup()
{
  myStepper.setSpeed(80);



   Serial.begin(115200, SERIAL_8N1);

   Serial3.begin(115200); //uart transmission of the angle to the controlling controller


   pinMode(IR_LED_PIN, OUTPUT);
   pinMode(IR_DETECTOR_PIN, INPUT);
   digitalWrite(IR_LED_PIN, HIGH); // Turn on Opto LED



  Wire.begin(); // join i2c bus

}

// Get a measurement from the LIDAR Lite
int lidarGetRange(void)
{
  int val = -1;

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if(2 <= Wire.available()) // if two bytes were received
  {
    val = Wire.read(); // receive high byte (overwrites previous reading)
    val = val << 8; // shift high byte to be high 8 bits
    val |= Wire.read(); // receive low byte as lower 8 bits
  }

  return val;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//begining of main loop

void loop()
{
  updateSonar(); // update the distance array
  //findspace();
  getan();

  init(ang);
  scann();
  getangle(data);
  send_angle(angle);


while(digitalRead(IR_DETECTOR_PIN) == 1){

      myStepper.step(-1);

      if(digitalRead(IR_DETECTOR_PIN) == 0){
        myStepper.step(0);
        }
   }

}

//end of main loop
//_____________________________________________________________________________________________________________________________________________
//*********************************************************************************************************************************************

int angs[9] ={0,0,0,0,0,0,0,0,0};//initiallise array of zeroes to represent spaces
int anga[9] ={30,45,60,75,90,105,120,135,150};

void updateSonar(){

    for (int i = 0; i < NUM_SONAR; i++) {
    distance[i] = sonar[i].ping_median(5); // update distance
   if (distance[i] == 0)
     distance[i] = sonar[i].ping_cm(); // update distance
  }
}

// get the angles that show the spaces using the sonar readSensors
//This is the basis of the sonar reads
void getan(){

   int d;
    int spac;
    int x;
   int temp = distance[0];


  int s;
    for(s = 1; s<6 ;s++){
       if(temp < distance[s]){

          temp = distance[s];
          d =s;
       }
    }

    ang = 15 + 30*d;

}


//find spaces using the rolling window algorithm


void findspace(){

  if ((distance[0]< m) && (distance[1]== m)){

     angs[0] = 30;

   }

   //rule 2
   if((distance[0]< m) && ( distance[1]< m))
   {

     angs[1]= 45;

   }
   // rule 3

   if((distance[1]< m) && (distance[2]== m))
     {

       angs[2] = 60;

     }
   //rule 4
   if((distance[1]< m) && (distance[2]< m))
     {

       angs[3] = 75;

     }
   //rule 5
   if((distance[2]< m) && ( distance[3]< m))
     {


       angs[4] = 90;

     }
   //rule 6
   if((distance[3]< m) && (distance[4]< m))
     {


       angs[5] = 105;

     }
   //rule 7
   if((distance[3]< m) && (distance[4]== m))
     {

       angs[6] = 120;

     }

   if((distance[4]< m) && (distance[5]< m))
     {

       angs[7] = 135;

     }
   // rule 9

    if((distance[4]< m) && (distance[5]== m))
     {


       angs[8] = 150;

     }
/*
for(int j = 0; j<9;j++){
   Serial.print(" ");
   Serial.print(angs[j]);
}
Serial.println("");
*/
}


 //calculate the available space. f more than one, chose the largest space and calcuate
 //the aangle it spans

void calcspace(){
     int i;
       int a1 = 0;
       int a2 = 0;
       bool flag1 = false;
       bool flag2 = false;

       for (i = 0;i < 9;i++){

                      if(angs[i] == 0 && angs[i+1] == 0 && angs[i+2]== 0 && angs[i+3]== 0 && angs[i+4]== 0 && angs[i+5]== 0){
                         if(i>=1 && i<=6){
                         a1 = anga[i-1];
                         a2 = anga[i+6];
                         }
                       }
                       else if(angs[i] == 0 && angs[i+1] == 0 && angs[i+2]== 0 && angs[i+3]== 0 && angs[i+4]== 0){
                         if(i>=1 && i<=6){
                         a1 = anga[i-1];
                         a2 = anga[i+5];
                         }
                       }

                       else if(angs[i] == 0 && angs[i+1] == 0 && angs[i+2]== 0 && angs[i+3]== 0){
                         if(i>=1 && i<=6){
                         a1 = anga[i-1];
                         a2 = anga[i+4];
                         }
                       }

                       else if(angs[i] == 0 && angs[i+1] == 0 && angs[i+2]== 0){
                         if(i>=1 && i<=6){
                         a1 = anga[i-1];
                         a2 = anga[i+3];
                         }
                       }

                       if(angs[i] == 0 && angs[i+1] == 0){
                         if(i>=1 && i<=7){
                            a1 = anga[i-1];
                            a2 = anga[i+2];
                         }
                       }

                      if(angs[i] == 0 ){
                        if(angs[0]==0){
                          a1 = 0;
                          flag2 = true;
                          }

                         if(angs[8]== 0){
                           a2 = 180;

                         }

                         if(angs[i-1] != 0){

                            if (flag2 == false){

                          a1 = anga[i-1];

                           flag2 = true;
                                   }

                           }
                        }

                 if(angs[i] != 0 ) {
                      if(angs[i-1] ==0){

                          if (flag1 == false){
                                a2 = anga[i];
                               flag1 = true;
                              }
                            }
                        }
                }



             ang = a1 + ( a2 - a1)/2;

             for(int d = 0; d<9;d++){
              angs[i]=0;
             }
             Serial.println(ang);
             Serial.end();
             Serial.begin(1000000);


            }

//_____________________________________________________________________________________________________________________________________________
//*********************************************************************************************************************************************

//initialise the stepper motor
void init(int a){
   //int steps;

   pos = (a/1.8)-13;

   if (digitalRead(IR_DETECTOR_PIN) == 0)  //Beam is Seen
  {
   myStepper.step(pos);
   delay(20);
}
}

/*scan through the availed angle to find the optimum space.
25 was used because it covers 45 degrees which is aquater of our
environment at the front*/

void scann(){
int p;
for(p = 0; p < 25; p++)
  {
    myStepper.step(1);

    data[p]= lidarGetRange();
  }

}

//get theh angle that has been passed out

void getangle(int *a){

  int temp = a[0];
  int d;
  int b;


  int s;
    for(s = 0; s<25 ;s++){
       if(a[s] > temp){

          temp = a[s];
          d = s;
       }

    }
  b = pos + d;

  angle = 1.8*b;


//transmit the angle to the second controller
  byte x = angle;
  Wire.beginTransmission(3);
  Wire.write(x);
  Wire.endTransmission();


  //Serial.println(angle);
  //Serial.println("");

}
/*
char str[10];
void send_angle(int a){
  //int value=42; //this would be much more exciting if it was a sensor value

  itoa(a, str, 10); //Turn value into a character array
  Serial3.write(str, 5);
  delay(50);
}
*/
