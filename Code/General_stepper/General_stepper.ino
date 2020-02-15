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

const int dm_0= 12;        // Dmode0 pin is connected to GPIO pin 11
const int dm_1= 10;        // Dmode1 pin is connected to GPIO pin 10
const int dm_2= 8;         // Dmode2 pin is connected to GPIO pin 9
const int dir= 6;          // Direction (CW/CCW) is given to GPIO pin 12 
const int clk= 4;          // Step (clk) is given to GPIO pin 7
const int ena= 2;          // Enable (ena) is given to GPIO pin 13
char rpm = 0;

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
}

void clockwise(float deg)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  int i,freq_pulse;
  float theta= 0;
  freq_pulse = 4*(deg)/0.9;   // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Clockwise ");
  Serial.println(deg,2);
  digitalWrite(ena,HIGH);
  digitalWrite(dir,HIGH);
  for (i=1; i<=freq_pulse; i++)   //fuction to generate a square wave of period 2t    
  {
     digitalWrite(clk,HIGH);
     delayMicroseconds(3125);     // the largest value that will produce an accurate delay is 16383
     digitalWrite(clk,LOW);
     delayMicroseconds(3125);
     theta= theta + (0.9)/4;       // angle increments by 1.8/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(ena,LOW);
}

void counterclockwise(float deg)   // function to rotate the kinect sensor anti-clockwise by a certain degree 'deg'
{
  int j, freq_pulse;
  float theta= 0;
  freq_pulse = 4*(deg)/0.9;     // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Anti-clockwise ");  
  Serial.println(deg,2);
  digitalWrite(ena,HIGH);
  digitalWrite(dir,LOW);
  for (j=1; j<=freq_pulse; j++)     // function to generate a square wave of rate 2t
  {
     digitalWrite(clk,HIGH);
     delayMicroseconds(3125);
     digitalWrite(clk,LOW);
     delayMicroseconds(3125);
     theta= theta+ (0.9)/4;         // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(ena,LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int k, M = 4;
  float F_out, T_out;
  float rpm = 12;
  F_out = (40*M)*rpm/3;
  T_out = 1/F_out;
  T_out = T_out*pow(10,6);     //Time period in microseconds
  T_out = ceil(T_out);
  Serial.println(F_out);
  Serial.println(T_out);
  delay(1000);
//  for (k=1; k<=3; k++)
//  {
//    Serial.print("Round ");
//    Serial.println(k);       // print the round number on the Serial monitor
//    delay(2000);
//    clockwise(90);           // call clockwise function to rotate by 45 degrees 
//    delay(2000);
//    counterclockwise(180);    // call counterclockwise function to rotate by 45 degrees
//    delay(2000);
//    clockwise(90);
//  }
//  delay(10000);              // wait for 30 seconds
}
