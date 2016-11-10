#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SPI.h" 

  uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
  RF24 radio(9, 10);

  struct Data
{
  uint8_t c[20]; 
  uint16_t len = 0; 
};

void setup() {
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(pipes[]); 
  //radio.openReadingPipe(1, pipes[1]);
}

void loop() {
  char c = "m"; 
  radio.write((char*) &c, 1);
}
