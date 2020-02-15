/*

 Mimics the fade example but with an extra parameter for frequency. It should dim but with a flicker 
 because the frequency has been set low enough for the human eye to detect. This flicker is easiest to see when 
 the LED is moving with respect to the eye and when it is between about 20% - 60% brighness. The library 
 allows for a frequency range from 1Hz - 2MHz on 16 bit timers and 31Hz - 2 MHz on 8 bit timers. When 
 SetPinFrequency()/SetPinFrequencySafe() is called, a bool is returned which can be tested to verify the 
 frequency was actually changed.
 
 This example runs on mega and uno.
 */

#include <PWM.h>

//use pin 11 on the Mega instead, otherwise there is a frequency cap at 31 Hz
int pin = 9;                // the pin that the LED is attached to
int32_t frequency = 10; //frequency (in Hz)
int sensorPin = A0;

void setup()
{
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  //sets the frequency for the specified pin
  bool success = SetPinFrequencySafe(pin, frequency);
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  
  //if the pin frequency was set successfully, turn pin 13 on
  if(success) 
  {
    Serial.println("Ok");    
  }
}

void loop()
{
  int i=1;
  for (i=1;i<=400;i++)
  {
    bool success = SetPinFrequencySafe(pin, frequency);
    if (i<=100)
    {
    pwmWrite(pin,4);   //use this functions instead of analogWrite on 'initialized' pins 
    delay(50);
    }
    if (i>100 && i<=200)
    {
    pwmWrite(pin,4);   //use this functions instead of analogWrite on 'initialized' pins 
    delay(50);
    }
    if (i>200 && i<=300)
    {
    pwmWrite(pin,4);   //use this functions instead of analogWrite on 'initialized' pins 
    delay(50);
    }
    if (i>300 && i<=400)
    {
    pwmWrite(pin,4);   //use this functions instead of analogWrite on 'initialized' pins 
    delay(50);
    }
    if (i==100)
    {
    Serial.println("Case 1");
    }
    if (i==200)
    {
    Serial.println("Case 2");
    }
    if (i==300)
    {
    Serial.println("Case 3");
    }
    if (i==400)
    {
    Serial.println("Case 4");
    }
  }
}
