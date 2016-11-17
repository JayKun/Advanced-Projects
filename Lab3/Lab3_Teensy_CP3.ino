#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

struct Data {
  char buttonLeft; 
  char buttonRight;
};

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);
Data packet; 

void init(){
  radio.printDetails();
  radio.openReadingPipe(1, pipes[1]);
  radio.openWritingPipe(pipes[0]);
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.setPayloadSize(2);
  radio.begin(); 
}
  
void setup() {
  Serial.begin(); 
  init(); 
}

void loop() {
  if(radio.available())
  {
    while(radio.available()){
      radio.read((Data*)packet, sizeof(packet)); 
      }
    }
    Serial.print("Button Left: ");
    Serial.println(packet.ButtonLeft);
    Serial.print("Button Right: ");
    Serial.println(packet.ButtonRight);
}
}
