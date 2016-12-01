#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

#define buttonL 2 
#define buttonR A3
#define sensorPin A0


struct Data {
  uint8_t buttonLeft; 
  uint8_t buttonRight;
  uint8_t batteryLevel; 
  //uint32_t batteryReading; 
};

Data packet; 

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);

void initRadio(){
  radio.begin();
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(3);
  radio.openWritingPipe(pipes[1]); 
  radio.openReadingPipe(1, pipes[0]);
}


void setup() {
  Serial.begin(9600); 
  initRadio();
  printf_begin(); 
  delay(1000); 
  radio.printDetails(); 
}

void loop() {
    int leftReading = digitalRead(buttonL); 
    int rightReading = digitalRead(buttonR);
    int batteryR = analogRead(sensorPin); 
    float voltageReading = batteryR*(3.337/1023); 
    voltageReading = (voltageReading/3.337)*4.2;

      if(leftReading == HIGH){
           packet.buttonLeft = 'H'; 
        }
      if(rightReading == HIGH){
           packet.buttonRight = 'H'; 
        }
        
      if(voltageReading >= 3.9 )
      packet.batteryLevel = 'G';

      else if (voltageReading >= 3.7)
      packet.batteryLevel ='Y';

      else if(voltageReading < 3.7)
      packet.batteryLevel = 'R';
    
    radio.write((Data*)&packet, sizeof(packet));
    
    packet.buttonRight ='L'; 
    packet.buttonLeft ='L';
    
}
