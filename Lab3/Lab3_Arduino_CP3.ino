#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

#define buttonL 12; 
#define buttonR 13; 


struct Data {
  char buttonLeft; 
  char buttonRight;
};

Data packet; 

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);

void init(){
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(pipes[1]); 
  radio.openReadingPipe(1, pipes[0]);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(2);
  radio.begin();
}


void setup() {
  init(); 
}

void loop() {
    int leftReading = digitalRead(buttonL); 
    int rightReading = digitalRead(buttonR);

      if(leftReading == HIGH){
           packet.buttonLeft = 'H'; 
        }
      if(rightReading == HIGH){
           packet.buttonRight = 'H'; 
        }
    
    radio.write((Data*)&packet, sizeof(packet));
    
    packet.buttonRight ='L'; 
    packet.buttonLeft ='L';
    
}
