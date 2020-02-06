/*
 * Author: Vineet V. Menon
 * Started on: 06/02/2020
 * Components: Arduino Uno/Mega, Toshiba TB67S249FTG stepper motor driver   
 * Power supply voltage: 12V 
 * Resolution of stepper motor= 1.8 degree per pulse
 * 
 * 
 */
const int dir= 8;          // Direction (CW/CCW) is given to GPIO pin 6 
const int clk= 7;          // Step (clk) is given to GPIO pin 5
//const int ena= 4;          // Enable pin is connected to GPIO pin 4
const int dm_0= 11;        // Dmode0 pin is connected to GPIO pin 11
const int dm_1= 10;        // Dmode1 pin is connected to GPIO pin 10
const int dm_2= 9;         // Dmode2 pin is connected to GPIO pin 9

void setup() 
{
  pinMode(dir,OUTPUT);    // Set dir pin as output
  pinMode(clk,OUTPUT);    // Set step pin as output
//  pinMode(ena,OUTPUT);    // Set enable pin as output
  pinMode(dm_0,OUTPUT);    // Set Dmode0 pin as output
  pinMode(dm_1,OUTPUT);    // Set Dmode1 pin as output
  pinMode(dm_2,OUTPUT);    // Set Dmode2 pin as output
  Serial.begin(9600);      // Serial communication baud rate set to 9600
//  digitalWrite(ena,HIGH);  // Set Enable pin as high to activate the driver
  digitalWrite(dm_0,HIGH);
  digitalWrite(dm_1,LOW);
  digitalWrite(dm_2,LOW); // Set microstepping factor by setting the states of Dmode0,Dmode1 and Dmode2
}

void clockwise(float deg)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  int i,freq_pulse;
  float theta= 0;
  freq_pulse = (deg)/0.9;   // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Clockwise ");
  Serial.println(deg,2);
//  digitalWrite(dir,HIGH);
  for (i=1; i<=freq_pulse; i++)   //fuction to generate a square wave of period 2t    
  {
     digitalWrite(clk,HIGH);
     delayMicroseconds(250);     // the largest value that will produce an accurate delay is 16383
     digitalWrite(clk,LOW);
     delayMicroseconds(50);
     theta= theta + (0.9);       // angle increments by 1.8/4 everytime this for loop ends
     Serial.println(theta);
  }
}

void counterclockwise(float deg)   // function to rotate the kinect sensor anti-clockwise by a certain degree 'deg'
{
  int j, freq_pulse;
  float theta= 0;
  freq_pulse = (deg)/0.9;     // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Anti-clockwise ");  
  Serial.println(deg,2);
  digitalWrite(dir,LOW);
  for (j=1; j<=freq_pulse; j++)     // function to generate a square wave of rate 2t
  {
     digitalWrite(clk,HIGH);
     delayMicroseconds(250);
     digitalWrite(clk,LOW);
     delayMicroseconds(50);
     theta= theta+ (0.9);         // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
}

void loop() 
{
  int k;
  for (k=1; k<=10; k++)
  {
    delay(3000);
    Serial.print("Round ");
    Serial.println(k);       // print the round number on the Serial monitor
    clockwise(360);           // call clockwise function to rotate by 45 degrees 
    delay(2000);
//    counterclockwise(360);    // call counterclockwise function to rotate by 45 degrees
//    delay(2000);
  }
  delay(30000);              // wait for 30 seconds
}
