/* Include the SPI library for the arduino boards */
#include <SPI.h>

/* Serial rates for UART */
#define BAUDRATE        115200

/* SPI commands */
#define AMT22_NOP       0x00
#define AMT22_RESET     0x60
#define AMT22_ZERO      0x70

/* We will use this define macros so we can write code once compatible with 12 bit encoders */
#define RES12           12

/* SPI pins */
#define ENC_0            2
#define SPI_MOSI        11
#define SPI_MISO        12
#define SPI_SCLK        13

void setup() 
{
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
  // put your main code here, to run repeatedly:
  Serial.println("Please set you new home position in 10 seconds...");
  delay(7000);
  Serial.println("Registering position in 3...");
  delay(1000);
  Serial.println("Registering position in 2...");
  delay(1000);
  Serial.println("Registering position in 1...");
  delay(1000);
  setZeroSPI(ENC_0);
  Serial.println("Registered new home position!");
  delay(60000);                        
}

void setZeroSPI(uint8_t encoder)
{
  spiWriteRead(AMT22_NOP, encoder, false);

  //this is the time required between bytes as specified in the datasheet.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3); 
  
  spiWriteRead(AMT22_ZERO, encoder, true);
  delay(250); //250 second delay to allow the encoder to reset
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
  delayMicroseconds(3);               //There is also a minimum time after clocking that CS should remain asserted before we release it
  setCSLine(encoder, releaseLine);                           //if releaseLine is high set it high else it stays low
  
  return data;
}

void setCSLine (uint8_t encoder, uint8_t csLine)
{
  digitalWrite(encoder, csLine);
}
