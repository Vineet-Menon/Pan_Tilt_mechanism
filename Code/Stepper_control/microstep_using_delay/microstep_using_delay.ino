/*
 * Author: Vineet V. Menon
 * Started on: 06/02/2020
 * Components: Arduino Uno/Mega, Toshiba TB67S249FTG stepper motor driver   
 * Power supply voltage: 24V 
 * Resolution of stepper motor= 1.8 degree per pulse
 * 
 * 
 */
#include <math.h>

#define dm_0 19        // Dmode0 pin is connected to GPIO pin 19
#define dm_1 17        // Dmode1 pin is connected to GPIO pin 17
#define dm_2 15        // Dmode2 pin is connected to GPIO pin 15
#define dir 13        // Direction (CW/CCW) is given to GPIO pin 13 
#define clk 11        // Step (clk) is given to GPIO pin 11
#define ena 9         // Enable (ena) is given to GPIO pin 9


void setup() 
{
  pinMode(dir,OUTPUT);    // Set dir pin as output
  pinMode(clk,OUTPUT);    // Set step pin as output
  pinMode(ena,OUTPUT);    // Set enable pin as output
  pinMode(dm_0,OUTPUT);   // Set Dmode0 pin as output
  pinMode(dm_1,OUTPUT);   // Set Dmode1 pin as output
  pinMode(dm_2,OUTPUT);   // Set Dmode2 pin as output
  Serial.begin(9600);     // Serial communication baud rate set to 9600
}

void clockwise(int pls_count, int m, int T)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  float theta= 0;
  digitalWrite(ena,HIGH);
  digitalWrite(dir,HIGH);
  for (int i=1; i<=pls_count; i++)   //fuction to generate a square wave of frequency F_out    
  {
     digitalWrite(clk,HIGH);
     delayMicroseconds(T);     // the largest value that will produce an accurate delay is 16383
     digitalWrite(clk,LOW);
     delayMicroseconds(T);
     theta= theta + (1.8/(4*m));       // angle increments by 1.8/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(ena,LOW);
}

void counterclockwise(int pls_count, int m, int T)   // function to rotate the kinect sensor anti-clockwise by a certain degree 'deg'
{
  float theta= 0;
  digitalWrite(ena,HIGH);
  digitalWrite(dir,LOW);
  for (int j=1; j<=(2*pls_count); j++)     // function to generate a square wave of frequency F_out
  {
     digitalWrite(clk,HIGH);
     delayMicroseconds(T);           // the largest value that will produce an accurate delay is 16383
     digitalWrite(clk,LOW);
     delayMicroseconds(T);
     theta= theta+ (1.8/(4*m));         // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
  digitalWrite(ena,LOW);
}

void microstep(int m)
{
  if (m==2)
  {
    digitalWrite(dm_0,HIGH);
    digitalWrite(dm_1,LOW);
    digitalWrite(dm_2,LOW);         // Set microstepping factor by setting the states of Dmode0,Dmode1 and Dmode2
  }
  if (m==4)
  {
    digitalWrite(dm_0,LOW);
    digitalWrite(dm_1,HIGH);
    digitalWrite(dm_2,HIGH);       // Set microstepping factor by setting the states of Dmode0,Dmode1 and Dmode2
  }
}

void loop() 
{
  int m = 4;
  float rpm = 3;
  float deg = 90;

  microstep(m);

  float f_out = (40*m)*(rpm/3);
  Serial.print("Output frequency is :");
  Serial.println(f_out);

  float pls_count = 20*(m*deg)/9;       // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  pls_count = ceil(pls_count);
  Serial.print("Pulse count is :");
  Serial.println(pls_count);

  float t_out = (pow(10,6))/f_out;          // time in microseconds
  t_out = ceil(t_out/2);                      //divide time period equally for T_on and T_off

  delay(5000);

  for (int k=1; k<=3; k++)
  {
    Serial.print("Round ");
    Serial.println(k);       // print the round number on the Serial monitor
    
    delay(1000);
    Serial.print("Clockwise ");  
    Serial.println(deg,2);
    clockwise(pls_count,m,t_out);           // call clockwise function to rotate by 45 degrees 
    
    delay(1000);
    Serial.print("Anti-clockwise ");  
    Serial.println((2*deg),2);
    counterclockwise(pls_count,m,t_out);    // call counterclockwise function to rotate by 45 degrees
    
    delay(1000);
    Serial.print("Clockwise ");  
    Serial.println(deg,2);
    clockwise(pls_count,m,t_out);
  }
  delay(10000);              // wait for 30 seconds
}
