#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

  uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
  RF24 radio(9, 10);
  
void setup() {
  // put your setup code here, to run once: 
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  printf_begin(); 
  radio.openWritingPipe(pipes[0]); 
  radio.openReadingPipe(1, pipes[1]);
}

void loop() {
    // put your main code here, to run repeatedly:
    char c = 'k'; 
    radio.write((char*)c, 1);
    if(radio.available())
    {
      radio.startListening(); 
      char data ;
      radio.read(&data, 1);
      radio.stopListening(); 
      Serial.println(data);
     }
}
