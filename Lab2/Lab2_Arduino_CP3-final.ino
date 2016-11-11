#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SPI.h" 

  uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
  RF24 radio(9, 10);

struct Data
{
  //uint8_t c[20]; 
  uint16_t len = 0;
};

Data packet; 
bool packetReceived = false;

void setup() {
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(pipes[1]); 
  printf_begin(); 
  radio.printDetails();
  radio.openReadingPipe(1, pipes[0]);
   radio.startListening();
}

void loop() {
   if(radio.available())
    {
      radio.read((Data*)&packet, sizeof(packet));
      Serial.println(packet.len);
      packetReceived = true;
      
       if(packetReceived)
      {
        packet.len++; 
        packetReceived = false;
      }
      radio.stopListening();
      Serial.println(packet.len);
      radio.write((Data*)&packet, sizeof(packet));
    }

}
