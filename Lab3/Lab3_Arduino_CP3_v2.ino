#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

#define buttonL 2; 
#define buttonR A3; 
#define sensorPin A1; 


struct Data {
  uint8_t buttonLeft; 
  uint8_t buttonRight;
  uint8_t batteryLevel; 
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
  radio.setPayloadSize(3);
  radio.begin();
}


void setup() {
  init(); 
  pinMode(sensorPin, INPUT);
}

void loop() {
    int leftReading = digitalRead(buttonL); 
    int rightReading = digitalRead(buttonR);
    int batteryReading = analogRead(sensorPin); 
    float voltageReading = batteryReading*(3.3/1023); 
    
      if(leftReading == HIGH){
           packet.buttonLeft = 'H'; 
        }
      if(rightReading == HIGH){
           packet.buttonRight = 'H'; 
        }
        
      if(voltageReading >= 3.9 )
      packet.batteryLevel = 'G';

      else if (voltageReading >=3.7)
      packet.batteryLevel ='Y';

      else
      packet.batteryLevel = 'R';
    
    radio.write((Data*)&packet, sizeof(packet));
    
    packet.buttonRight ='L'; 
    packet.buttonLeft ='L';
    
}
