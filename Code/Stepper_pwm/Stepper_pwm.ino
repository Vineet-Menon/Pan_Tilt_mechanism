/*
 * Author: Vineet V. Menon
 * Started on: 12/02/2020
 * Components: Arduino Uno/Mega, Toshiba TB67S249FTG stepper motor driver   
 * Power supply voltage: 24V 
 * Resolution of stepper motor= 1.8 degree per pulse
 * Microstepping factor and desired rpm included
 * 
 */

#include <math.h>
#include <PWM.h>

#define dm_0 19        // Dmode0 pin is connected to GPIO pin 11
#define dm_1 17        // Dmode1 pin is connected to GPIO pin 10
#define dm_2 15        // Dmode2 pin is connected to GPIO pin 9
#define dir 13        // Direction (CW/CCW) is given to GPIO pin 12 
#define clk 11         // Step (clk) is given to GPIO pin 7
#define ena 9         // Enable (ena) is given to GPIO pin 13

void setup() 
{
  // put your setup code here, to run once:
  pinMode(dir,OUTPUT);    // Set dir pin as output
  pinMode(clk,OUTPUT);    // Set step pin as output
  pinMode(ena,OUTPUT);    // Set enable pin as output
  pinMode(dm_0,OUTPUT);    // Set Dmode0 pin as output
  pinMode(dm_1,OUTPUT);    // Set Dmode1 pin as output
  pinMode(dm_2,OUTPUT);    // Set Dmode2 pin as output
  Serial.begin(9600);      // Serial communication baud rate set to 9600
  InitTimersSafe();        //initialize all timers except for 0, to save time keeping functions
}

void clockwise(float deg, float F_out)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  int i,freq_pulse;
  float theta= 0;
  freq_pulse = 4*(deg)/0.45;   // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Clockwise ");
  Serial.println(deg,2);
  digitalWrite(ena,HIGH);
  digitalWrite(dir,HIGH);
  for (i=1; i<=freq_pulse; i++)   //fuction to generate a square wave of period 2t    
  {
     bool success = SetPinFrequencySafe(clk, F_out);      //sets the frequency for the specified pin
     pwmWrite(clk,127);
     theta= theta + (0.45)/4;       // angle increments by 1.8/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(clk,LOW);
  digitalWrite(ena,LOW);
}

void counterclockwise(float deg, float F_out)   // function to rotate the kinect sensor anti-clockwise by a certain degree 'deg'
{
  int j, freq_pulse;
  float theta= 0;
  freq_pulse = 4*(deg)/0.45;     // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Anti-clockwise ");  
  Serial.println(deg,2);
  digitalWrite(ena,HIGH);
  digitalWrite(dir,LOW);
  for (j=1; j<=freq_pulse; j++)     // function to generate a square wave of rate 2t
  {
     bool success = SetPinFrequencySafe(clk, F_out);      //sets the frequency for the specified pin
     pwmWrite(clk,127);
     theta= theta+ (0.45)/4;         // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(clk,LOW);
  digitalWrite(ena,LOW);
}

void Microstep(int M)
{
  if (M==4)
  {
    digitalWrite(dm_0,LOW);
    digitalWrite(dm_1,HIGH);
    digitalWrite(dm_2,HIGH);       // Set microstepping factor by setting the states of Dmode0,Dmode1 and Dmode2
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int k, M = 4;
  float rpm = 3;
  float deg = 90;
  float F_out;
  Microstep(M);
  F_out = (40*M)*rpm/3;
  F_out = ceil(F_out);
  Serial.println(F_out);
  for (k=1; k<=3; k++)
  {
    Serial.print("Round ");
    Serial.println(k);       // print the round number on the Serial monitor
    delay(1000);
    clockwise(deg,F_out);           // call clockwise function to rotate by 45 degrees 
    delay(1000);
    counterclockwise(deg,F_out);    // call counterclockwise function to rotate by 45 degrees
    delay(1000);
    counterclockwise(deg,F_out);    // call counterclockwise function to rotate by 45 degrees
    delay(1000);
    clockwise(deg,F_out);
  }
  delay(10000);              // wait for 30 seconds
}
