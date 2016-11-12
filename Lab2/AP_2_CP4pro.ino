#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SPI.h" 
#define ButtonG  9
#define ButtonY  8
#define ButtonR  7

  uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
  RF24 radio(9, 10);

struct Data
{
  uint8_t c[20]; 
  uint16_t len = 0;
};

Data packet; 
bool packetReceived = false;

void setup() {
  pinMode(ButtonR, INPUT);
  pinMode(ButtonY, INPUT);
  pinMode(ButtonG, INPUT);
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

      for(int i=0; i< packet.len; i++)
      {
        char lightColor = packet.c[i]; 
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

         while(digitalRead(ButtonR)== HIGH || digitalRead(ButtonY) == HIGH || digitalRead(ButtonG) == HIGH)
         {;}
         if ((ButtonRIn == HIGH && lightColor == 'R') || 
          (ButtonYIn == HIGH && lightColor == 'Y') ||
          (ButtonGIn == HIGH && lightColor == 'G'))
         {
          continue;
         }
         else
         {
            //write to Teensy
            uint8_t w = "W"; 
            radio.write((uint8_t*) &w, 1);
         }  
      }
      //write to Teensy
      uint8_t c = "C"; 
      radio.write((uint8_t*) &c, 1);
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
