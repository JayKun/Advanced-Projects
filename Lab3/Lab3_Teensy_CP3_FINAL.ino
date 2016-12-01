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
  uint32_t batteryReading; 
};

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);
Data packet; 

void initRadio(){
  radio.begin();   
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_8);
  radio.setPayloadSize(7);
  radio.openReadingPipe(1, pipes[1]);
  radio.openWritingPipe(pipes[0]);
  radio.setChannel(23);
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  delay(1000);
  radio.printDetails();
}
  
void setup() {
  Serial.begin(9600); 
  printf_begin();
  initRadio();
}

void loop() {
  radio.startListening();
  if(radio.available())
  {
    while(radio.available()){
      radio.read((Data*)&packet, sizeof(packet)); 
      }
    if(packet.buttonLeft == 'H'){
      Serial.print("Button Left: ");
      Serial.println((char)packet.buttonLeft);
    }
    if(packet.buttonRight == 'H'){
      Serial.print("Button Right: ");
      Serial.println((char)packet.buttonRight);
    }
    if(packet.batteryLevel=='R'){
      digitalWrite(RLED, HIGH);
      delay(100);
    }
    else if(packet.batteryLevel=='Y'){
      digitalWrite(YLED, HIGH);
      delay(100);
    }
    else if (packet.batteryLevel=='G'){
      digitalWrite(GLED, HIGH);
      delay(100);
    }
    Serial.print((float)packet.batteryReading);
  }

}

