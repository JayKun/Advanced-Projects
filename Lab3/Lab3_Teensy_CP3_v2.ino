#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

#define RLED 15
#define YLED 18
#define GLED 21


struct Data {
  uint8_t buttonLeft; 
  uint8_t buttonRight;
  uint8_t batteryLevel; 
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
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(3);
  radio.begin(); 
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(GLED, OUTPUT);
}
  
void setup() {
  Serial.begin(); 
  init(); 
}

void loop() {
   radio.startListening();
  if(radio.available())
  {
    while(radio.available()){
      radio.read((Data*)packet, sizeof(packet)); 
      }
   }
    if(packet.buttonLeft == 'H'){
      Serial.print("Button Left: ");
      Serial.println(packet.buttonLeft);
    }
    if(packet.buttonRight == 'H'){
      Serial.print("Button Right: ");
      Serial.println(packet.buttonRight);
    }
    if(packet.batteryLevel='R'){
      Serial.print("Battery Level: ");
      Serial.println(packet.batteryLevel);
      digitalWrite(RLED, HIGH);
//      delay(500);
//      digitalWrite(RLED, LOW);
      delay(100);
    }
    else if(packet.battertLevel='Y'){
      Serial.print("Battery Level: ");
      Serial.println(packet.batteryLevel);
      digitalWrite(YLED, HIGH);
//      delay(500);
//      digitalWrite(YLED, LOW);
      delay(100);
    }
    else if (packet.batteryLevel='R'){
      Serial.print("Battery Level: ");
      Serial.println(packet.batteryLevel);
      digitalWrite(GLED, HIGH);
//      delay(500);
//      digitalWrite(GLED, LOW);
      delay(100);
    }
    
    
  
}
