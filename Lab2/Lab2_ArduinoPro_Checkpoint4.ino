#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

/* #define ButtonRS 10
#define ButtonYS 11
#define ButtonGS 12 */
#define ButtonG  6
#define ButtonY  7
#define ButtonR  8
#include<stdio.h>

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);

//init obj with length and array
struct Data
{
  uint8_t c[20]; 
  uint16_t len = 0; 
};

Data packetReceived; //send yes or no back to teensy
  
void setup() {
  // put your setup code here, to run once:
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  //printf_begin(); 
  radio.openWritingPipe(pipes[1]); 
  radio.openReadingPipe(1, pipes[0]);

  pinMode(ButtonR, INPUT);
  pinMode(ButtonY, INPUT);
  pinMode(ButtonG, INPUT);

  Serial.println("Arduino setup done");
}

void loop() {
  // put your main code here, to run repeatedly:

  radio.startListening();

  while(!radio.available())
  {}
  
  if(radio.available())
{
  Serial.println("Arduino radio available");
  while(radio.available()) // data sent by Teensy read in frames, must read all frames to have all data for round
  {
    // something is received
    //Serial.readBytes((char*) &packetReceived, sizeof(Data));
    radio.read((Data*)&packetReceived, sizeof(packetReceived));
  }
}
  radio.stopListening();

  Serial.println(packetReceived.len);
  
  for(int i = 0; i < packetReceived.len; i++)
  {
    char lightColor = packetReceived.c[i]; 
    Serial.println(lightColor);
    char buttonColor; 
    int ButtonRIn;
    int ButtonYIn;
    int ButtonGIn;
  
    ButtonRIn = digitalRead(ButtonR);
    //delay(300); 
    ButtonYIn = digitalRead(ButtonY);
    //delay(300); 
    ButtonGIn = digitalRead(ButtonG);
    //delay(300); 
      
      while(ButtonRIn == LOW && ButtonYIn == LOW && ButtonGIn == LOW)
      {
        ButtonRIn = digitalRead(ButtonR);
        //delay(100); 
        ButtonYIn = digitalRead(ButtonY);
        //delay(100); 
        ButtonGIn = digitalRead(ButtonG);
        //delay(100);
      }

      while(digitalRead(ButtonR) == HIGH || digitalRead(ButtonY) == HIGH || digitalRead(ButtonG) == HIGH)
      {}
      
      if ((ButtonRIn == HIGH && lightColor == 'R') || 
          (ButtonYIn == HIGH && lightColor == 'Y') ||
          (ButtonGIn == HIGH && lightColor == 'G'))
      {
        continue;
      }
      else
      {
        //Serial.write('W');
        uint8_t w = 'W';
        radio.write((uint8_t*)&w, sizeof(w));
        Serial.println(w);
      }   
  }    
  //Serial.write('C');
  uint8_t c = 'C';
  radio.write((uint8_t*)&c, sizeof(c));
  Serial.println(c); 
}

