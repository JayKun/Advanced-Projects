#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7};
RF24 radio(9, 10);

void setup() {
  // put your setup code here, to run once:
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  //printf_begin(); 
  //radio.openWritingPipe(pipes[0]); 
  radio.openReadingPipe(1, pipes[1]);
}

void loop() {
    // put your main code here, to run repeatedly:
    radio.startListening(); 
    
    if(radio.available())
    {
      while(radio.available())
      {
        // reading
        char data;
        radio.read(&data, 1); 
        Serial.println(data);

        /* // writing
        char c = 'a'; 
        radio.write((uint8_t*)c, 1); */
      }
     }
     radio.stopListening();
}
