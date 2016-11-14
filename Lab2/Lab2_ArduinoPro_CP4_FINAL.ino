#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

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
bool lost = false; 
  
void setup() {
  // put your setup code here, to run once:
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
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
  {Serial.println("Waiting for Sequence");}
  
  if(radio.available())
{
  while(radio.available())
  {
  Serial.println("Arduino radio available");
    // something is received
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
    ButtonYIn = digitalRead(ButtonY);   
    ButtonGIn = digitalRead(ButtonG);
      
      while(ButtonRIn == LOW && ButtonYIn == LOW && ButtonGIn == LOW)
      {
        Serial.println("Reading Button Presses...");
        ButtonRIn = digitalRead(ButtonR);
        ButtonYIn = digitalRead(ButtonY);
        ButtonGIn = digitalRead(ButtonG);
      }

      while(digitalRead(ButtonR)== HIGH || digitalRead(ButtonY) == HIGH || digitalRead(ButtonG) == HIGH)
      {Serial.println("In Debouncing");}
      
      if ((ButtonRIn == HIGH && lightColor == 'R') || 
          (ButtonYIn == HIGH && lightColor == 'Y') ||
          (ButtonGIn == HIGH && lightColor == 'G'))
      {
        continue;
      }
      else
      {
        uint8_t w = 'W';
        Serial.write(w);
        radio.write((uint8_t*)&w, sizeof(w));
        Serial.println(w);
        packetReceived.len =0;
      }   
  }    
  uint8_t c = 'C';
  radio.write((uint8_t*)&c, sizeof(c));
  Serial.println(c); 
}

