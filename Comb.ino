#include <LiquidCrystal.h>
#include <Filters.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sensorTA12 = A0;
float nVPP;
float nCurrThruResistorPP;
float nCurrThruResistorRMS;
float nCurrentThruWire;

float testFrequency = 50;                     
float windowLength = 40.0/testFrequency;     
int Sensor = 0; 
float intercept = -0.04; 
float slope = 0.0405; 
float current_Volts; 
unsigned long printPeriod = 1000; 
unsigned long previousMillis = 0;

int pin = 13;
float rads = 57.29577951;
float degree = 360;
float frequency = 50;
float nano = 1 * pow (10,-6);
float pf;
float angle;
float pf_max = 0;
float angle_max = 0;
int ctr; 
void setup() 
{
  Serial.begin(9600); 
  pinMode(sensorTA12, INPUT);
  pinMode(pin, INPUT);
  lcd.begin(16, 2);
}

void loop() 
{
  nVPP = getVPP();
  nCurrThruResistorPP = (nVPP/200.0) * 1000.0;
  nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;
  nCurrentThruWire = nCurrThruResistorRMS * 1000;

  RunningStatistics inputStats;                
  inputStats.setWindowSecs( windowLength );
  while( true ) 
  {   
    Sensor = analogRead(A1);  
    inputStats.input(Sensor); 
    if((unsigned long)(millis() - previousMillis) >= printPeriod) 
    {
      previousMillis = millis();
      current_Volts = intercept + slope * inputStats.sigma(); 
      current_Volts= current_Volts*(85.3231);                
    }
  }

  for (ctr = 0; ctr <= 4; ctr++)
  {
     angle = ((((pulseIn(pin, HIGH)) * nano)* degree)* frequency);
     if (angle > angle_max) // Test if the angle is maximum angle
     {
      angle_max = angle; // If maximum record in variable "angle_max"
      pf_max = cos(angle_max / rads); // Calc PF from "angle_max"
     }
   }
   if (angle_max > 360) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
   if (angle_max == 0) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
   Serial.print(nCurrentThruWire,3);
   Serial.print( current_Volts );
   Serial.print(pf_max, 2);
}

float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorTA12);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
   }
   
   // Convert the digital data to a voltage
   result = (maxValue * 5.0)/1024.0;
  
   return result;
 }
