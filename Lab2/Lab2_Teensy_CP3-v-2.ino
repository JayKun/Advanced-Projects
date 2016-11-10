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
  pinMode(18, OUTPUT);
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  printf_begin(); 
  delay(1000);
  radio.printDetails();
  radio.openReadingPipe(1, pipes[1]);
  radio.openWritingPipe(pipes[0]);

}

void loop() {
    radio.startListening();  
    if(radio.available())
    {
     while(radio.available()) 
      {
      digitalWrite(18, HIGH);
      uint8_t data ;
      radio.read((uint8_t*)&data, sizeof(data));
      Serial.println((char)data);
      }
     }
     
}
