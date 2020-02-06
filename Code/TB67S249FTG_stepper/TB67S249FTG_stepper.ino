/*
 * Author: Vineet V. Menon
 * Started on: 23/02/2019
 * Components: Arduino Uno/Mega, Toshiba TB67S249FTG stepper motor driver   
 * Power supply voltage: 24V 
 * Resolution of stepper motor= 1.8 degree per pulse
 * 
 * 
 */
const int spr= 400;        // Micro-stepping is set to 2
const int dirp=4;          // dir+ is given to GPIO pin 4 
const int pulp=5;         // pul+ is given to GPIO pin 5

void setup() 
{
  pinMode(dirp,OUTPUT);    // Set dir+ pin as output
  pinMode(pulp,OUTPUT);    // Set pul+ pin as output
  Serial.begin(9600);      // Serial communication baud rate set to 9600
}

void clockwise(float deg)   // function to rotate the kinect sensor clockwise by a certain degree 'deg'
{
  int i,freq_pulse;
  float theta= 0;
  freq_pulse = (4*deg)/0.9;   // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Clockwise ");
  Serial.println(deg,2);
  digitalWrite(dirp,HIGH);
  for (i=1; i<=freq_pulse; i++)   //fuction to generate a square wave of period 2t    
  {
     digitalWrite(pulp,HIGH);
//     delayMicroseconds(16000);     // the largest value that will produce an accurate delay is 16383
     delay(20);                      // if required delay is above 16 milli seconds
     digitalWrite(pulp,LOW);
//     delayMicroseconds(16000);
     delay(20);
     theta= theta+ (0.9/4);       // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
}

void counterclockwise(float deg)   // function to rotate the kinect sensor anti-clockwise by a certain degree 'deg'
{
  int j, freq_pulse;
  float theta= 0;
  freq_pulse = (4*deg)/0.9;     // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  Serial.print("Anti-clockwise ");  
  Serial.println(deg,2);
  digitalWrite(dirp,LOW);
  for (j=1; j<=freq_pulse; j++)     // function to generate a square wave of rate 2t
  {
     digitalWrite(pulp,HIGH);
//     delayMicroseconds(16000);
     delay(20);
     digitalWrite(pulp,LOW);
//     delayMicroseconds(16000);
     delay(20);
     theta= theta+ (0.9/4);         // angle increments by 0.9/4 everytime this for loop ends
     Serial.println(theta);
  }
}

void loop() 
{
  int k;
  for (k=1; k<=3; k++)
  {
    delay(2000);
    Serial.print("Round ");
    Serial.println(k);       // print the round number on the Serial monitor
    clockwise(90);           // call clockwise function to rotate by 45 degrees 
    delay(2000);
    counterclockwise(100);    // call counterclockwise function to rotate by 45 degrees
    delay(2000);
    clockwise(40);
  }
  delay(30000);              // wait for 30 seconds
}
