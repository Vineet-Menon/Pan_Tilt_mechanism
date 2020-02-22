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
#define clk 11        // Step (clk) is given to GPIO pin 7
#define ena 9         // Enable (ena) is given to GPIO pin 13

void setup() 
{
  // put your setup code here, to run once:
  pinMode(dir,OUTPUT);    // Set dir pin as output
  pinMode(clk,OUTPUT);    // Set step pin as output
  pinMode(ena,OUTPUT);    // Set enable pin as output
  pinMode(dm_0,OUTPUT);   // Set Dmode0 pin as output
  pinMode(dm_1,OUTPUT);   // Set Dmode1 pin as output
  pinMode(dm_2,OUTPUT);   // Set Dmode2 pin as output
  Serial.begin(9600);     // Serial communication baud rate set to 9600
  InitTimersSafe();       //initialize all timers except for 0, to save time keeping functions
}

void clockwise(int pls_count, int m)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  float theta= 0;
  digitalWrite(ena,HIGH);
  digitalWrite(dir,HIGH);
  for (int i=1; i<=pls_count; i++)   //fuction to generate a square wave of frequency F_out    
  {
     pwmWrite(clk,128);
     theta= theta + 1.8/(4*m);       // angle increments by 1.8/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(clk,LOW);
  digitalWrite(ena,LOW);
}

void counterclockwise(int pls_count, int m)   // function to rotate the kinect sensor anti-clockwise by a certain degree 'deg'
{
  float theta= 0;
  digitalWrite(ena,HIGH);
  digitalWrite(dir,LOW);
  for (int j=1; j<=pls_count; j++)     // function to generate a square wave of frequency F_out
  {
     pwmWrite(clk,128);
     theta= theta+ 1.8/(4*m);         // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(clk,LOW);
  digitalWrite(ena,LOW);
}

void microstep(int m)
{
  if (m==4)
  {
    digitalWrite(dm_0,LOW);
    digitalWrite(dm_1,HIGH);
    digitalWrite(dm_2,HIGH);       // Set microstepping factor by setting the states of Dmode0,Dmode1 and Dmode2
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int m = 4;
  float rpm = 6;
  float deg = 45;
  microstep(m);
  delay(1);           //Indication time is 1 ms after the release of the standby mode
  
  float f_out = (40*m)*rpm/3;
  float pls_count = 20*(m*deg)/9;       // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  f_out = ceil(f_out);
  Serial.print("Output frequency is :");
  Serial.println(f_out);
  pls_count = ceil(pls_count);
  Serial.print("Pulse count is :");
  Serial.println(pls_count);
  bool success = SetPinFrequencySafe(clk, f_out);      //sets the frequency for the specified pin
  if(success) 
  {
    Serial.println("Ok");    
  }
  for (int k=1; k<=3; k++)
  {
    Serial.print("Round ");
    Serial.println(k);       // print the round number on the Serial monitor
    
    delay(1000);
    Serial.print("Clockwise ");  
    Serial.println(deg,2);
    clockwise(pls_count,m);           // call clockwise function to rotate by 45 degrees 
    
    delay(1000);
    Serial.print("Anti-clockwise ");  
    Serial.println(deg,2);
    counterclockwise(pls_count,m);    // call counterclockwise function to rotate by 45 degrees
    
    delay(1000);
    Serial.print("Anti-clockwise ");  
    Serial.println(deg,2);
    counterclockwise(pls_count,m);    // call counterclockwise function to rotate by 45 degrees
    
    delay(1000);
    Serial.print("Clockwise ");  
    Serial.println(deg,2);
    clockwise(pls_count,m);
  }
  delay(10000);              // wait for 30 seconds
}
