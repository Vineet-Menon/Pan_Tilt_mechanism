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
#define pin 9               // the pin that the LED is attached to
#define sensorPin  7
int32_t frequency = 160;    //frequency (in Hz)
int sensorValue = 0;

void setup()
{
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  //sets the frequency for the specified pin
  bool success = SetPinFrequencySafe(pin, frequency);
  Serial.begin(57600);
  pinMode(pin, OUTPUT);
  
  //if the pin frequency was set successfully, turn pin 13 on
  if(success) 
  {
    Serial.println("Ok");    
  }
}

void loop()
{
  bool success = SetPinFrequencySafe(pin, frequency);
  pwmWrite(pin,127);
  sensorValue = digitalRead(sensorPin);
  Serial.println(sensorValue);
  delay(50);
}
