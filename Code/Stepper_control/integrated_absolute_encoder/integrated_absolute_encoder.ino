/*
 * Author: Vineet V. Menon
 * Started on: 22/02/2020
 * Components: Atmel 2560 Pro, Toshiba TB67S249FTG stepper motor driver   
 * Power supply voltage: 24V 
 * Resolution of stepper motor= 1.8 degree per pulse
 */

#include <math.h>
#include <SPI.h>

#define dm_0 19        // Dmode0 pin is connected to GPIO pin 11
#define dm_1 17        // Dmode1 pin is connected to GPIO pin 10
#define dm_2 15        // Dmode2 pin is connected to GPIO pin 9
#define dir 13        // Direction (CW/CCW) is given to GPIO pin 12 
#define clk 11        // Step (clk) is given to GPIO pin 7
#define ena 9         // Enable (ena) is given to GPIO pin 13

/* Serial rates for UART */
#define BAUDRATE        115200

/* SPI commands */
#define AMT22_NOP       0x00
#define AMT22_RESET     0x60
#define AMT22_ZERO      0x70

/* Define special ascii characters */
#define NEWLINE         0x0A
#define TAB             0x09

/* We will use this define macros so we can write code once compatible with 12 bit encoders */
#define RES12           12

/* SPI pins */
#define ENC_0           53
#define SPI_MOSI        51
#define SPI_MISO        50
#define SPI_SCLK        52

void setup() 
{
  pinMode(dir,OUTPUT);    // Set dir pin as output
  pinMode(clk,OUTPUT);    // Set step pin as output
  pinMode(ena,OUTPUT);    // Set enable pin as output
  pinMode(dm_0,OUTPUT);   // Set Dmode0 pin as output
  pinMode(dm_1,OUTPUT);   // Set Dmode1 pin as output
  pinMode(dm_2,OUTPUT);   // Set Dmode2 pin as output
  
  //Set the modes for the SPI IO
  pinMode(SPI_SCLK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_MISO, INPUT);
  pinMode(ENC_0, OUTPUT);

  //Initialize the UART serial connection for debugging
  Serial.begin(BAUDRATE);

  //Get the CS line high which is the default inactive state
  digitalWrite(ENC_0, HIGH);

  //set the clockrate. Uno clock rate is 16Mhz, divider of 32 gives 500 kHz.
  //500 kHz is a good speed for our test environment
  SPI.setClockDivider(SPI_CLOCK_DIV32);    // 500 kHz

  //start SPI bus
  SPI.begin();
}

void loop() 
{
  int m = 4;
  float rpm = 6;
  float deg = 90;
  float theta= 0;

  microstep(m);
  delay(1);           //Indication time is 1 ms after the release of the standby mode

  //create a 16 bit variable to hold the encoders position
  uint16_t encoderPosition;
  //let's also create a variable where we can count how many times we've tried to obtain the position in case there are errors
  uint8_t attempts;
  
  float f_out = (40*m)*(rpm/3);
  Serial.print("Output frequency is :");
  Serial.println(f_out);

  float pls_count = 20*(m*deg)/9;       // driven pulley would rotate 1/4 times the degree rotated by the driver pulley
  pls_count = ceil(pls_count);
  Serial.print("Pulse count is :");
  Serial.println(pls_count);

  float t_out = (pow(10,6))/f_out;          // time in microseconds
  t_out = ceil(t_out/2);                      //divide time period equally for T_on and T_off
  Serial.print("Time period is :");
  Serial.println(t_out);

  delay(2000);
  
  for(int i=1; i<=pls_count; i++)
  {
    //set attemps counter at 0 so we can try again if we get bad position    
    attempts = 0;

    //this function gets the encoder position and returns it as a uint16_t
    //send the function res12 for your encoders resolution
    encoderPosition = getPositionSPI(ENC_0, RES12);

    Serial.print("i = ");
    Serial.println(i);

    //if the position returned was 0xFFFF we know that there was an error calculating the checksum
    //make 3 attempts for position. we will pre-increment attempts because we'll use the number later and want an accurate count
    while (encoderPosition == 0xFFFF && ++attempts < 3)
    {
      encoderPosition = getPositionSPI(ENC_0, RES12); //try again
    }

    if (encoderPosition == 0xFFFF) //position is bad, let the user know how many times we tried
    {
      Serial.print("Encoder 0 error. Attempts: ");
      Serial.print(attempts, DEC); //print out the number in decimal format. attempts - 1 is used since we post incremented the loop
      Serial.write(NEWLINE);
    }

    else //position was good, print to serial stream
    {
      Serial.print("Encoder 0: ");
      Serial.print(encoderPosition);      //print the position in decimal format
      Serial.write(NEWLINE);
    }

    float theta_enc = (encoderPosition*360)/4096;   // encoder value conversion to degrees
    //Serial.println(theta_enc);
    
    anticlockwise(t_out);
    theta= theta+ (1.8/(4*m));         // angle increments by 0.9/4 everytime this for loop ends
    //Serial.println(theta,4);

    Serial.println(" ");

  }
  delay(5000);
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

uint16_t getPositionSPI(uint8_t encoder, uint8_t resolution)
{
  uint16_t currentPosition;       //16-bit response from encoder
  bool binaryArray[16];           //after receiving the position we will populate this array and use it for calculating the checksum

  //get first byte which is the high byte, shift it 8 bits. don't release line for the first byte
  currentPosition = spiWriteRead(AMT22_NOP, encoder, false) << 8;   

  //this is the time required between bytes as specified in the datasheet.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  //OR the low byte with the currentPosition variable. release line after second byte
  currentPosition |= spiWriteRead(AMT22_NOP, encoder, true);        

  //run through the 16 bits of position and put each bit into a slot in the array so we can do the checksum calculation
  for(int i = 0; i < 16; i++) binaryArray[i] = (0x01) & (currentPosition >> (i));

  //using the equation on the datasheet we can calculate the checksums and then make sure they match what the encoder sent
  if ((binaryArray[15] == !(binaryArray[13] ^ binaryArray[11] ^ binaryArray[9] ^ binaryArray[7] ^ binaryArray[5] ^ binaryArray[3] ^ binaryArray[1]))
          && (binaryArray[14] == !(binaryArray[12] ^ binaryArray[10] ^ binaryArray[8] ^ binaryArray[6] ^ binaryArray[4] ^ binaryArray[2] ^ binaryArray[0])))
    {
      //we got back a good position, so just mask away the checkbits
      currentPosition &= 0x3FFF;
    }
  else
  {
    currentPosition = 0xFFFF; //bad position
  }

  //If the resolution is 12-bits, and wasn't 0xFFFF, then shift position, otherwise do nothing
  if ((resolution == RES12) && (currentPosition != 0xFFFF)) currentPosition = currentPosition >> 2;

  return currentPosition;
}

uint8_t spiWriteRead(uint8_t sendByte, uint8_t encoder, uint8_t releaseLine)
{
  //holder for the received over SPI
  uint8_t data;

  //set cs low, cs may already be low but there's no issue calling it again except for extra time
  setCSLine(encoder ,LOW);

  //There is a minimum time requirement after CS goes low before data can be clocked out of the encoder.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  //send the command  
  data = SPI.transfer(sendByte);
  delayMicroseconds(3); //There is also a minimum time after clocking that CS should remain asserted before we release it
  setCSLine(encoder, releaseLine); //if releaseLine is high set it high else it stays low
  
  return data;
}

void setCSLine (uint8_t encoder, uint8_t csLine)
{
  digitalWrite(encoder, csLine);
}

void clockwise(int T)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  digitalWrite(ena,HIGH);
  digitalWrite(dir,LOW);
  
  digitalWrite(clk,HIGH);
  delayMicroseconds(T);     // the largest value that will produce an accurate delay is 16383
  digitalWrite(clk,LOW);
  delayMicroseconds(T);
  
  digitalWrite(ena,LOW);
}

void anticlockwise(int T)   // function to rotate the motor clockwise by a certain degree 'deg'
{
  digitalWrite(ena,HIGH);
  digitalWrite(dir,HIGH);
  
  digitalWrite(clk,HIGH);
  delayMicroseconds(T);     // the largest value that will produce an accurate delay is 16383
  digitalWrite(clk,LOW);
  delayMicroseconds(T);
  
  digitalWrite(ena,LOW);
}
