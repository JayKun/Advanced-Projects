#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SPI.h" 
#define RLED 12
#define YLED 16
#define GLED 15
//0 RX, 1 TX

  uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
  RF24 radio(9, 10);
  
  struct Data
{
  //uint8_t c[20]; 
  uint16_t len = 0;
};

char array[3] = {'R', 'Y', 'G'}; 
Data packet; 

char s[sizeof(Data)];

// Adds one random character to the existing sequence
void genChar(Data& packet)
{
  int randNum = random(0,3);
  packet.c[packet.len] =  array[randNum]; 
  packet.len ++; //increase length per round
}

void reset(Data& packet)
{
  packet.len = 0;
}

void setup() {
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  radio.begin(); 
  Serial.begin(9600); 
  randomSeed(analogRead(0)); //
  genChar(packet);  //
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  printf_begin();   
  delay(1000);
  radio.printDetails();
  radio.openReadingPipe(1, pipes[1]);
  radio.openWritingPipe(pipes[0]);
}

void loop() { 
   radio.stopListening(); 

   // 1. Send bytes
   radio.write((Data*)&packet, sizeof(packet));
  
   radio.startListening(); 
   delay(1000); 
   if(radio.available())
   {
      // 2. Flash each corresponding LED to Player 1
      for(int k = 0 ; k < packet.len; k++)
      {
        Serial.println("In loop");
        char color = packet.c[k]; 
        if (color == 'R')
        {
           digitalWrite(RLED, HIGH);
           delay(500);
           digitalWrite(RLED, LOW);
           delay(100);
        }
        else if (color == 'Y')
        {
           digitalWrite(YLED, HIGH);
           delay(500);
           digitalWrite(YLED, LOW);
           delay(100);
        }
        else if (color == 'G')
        {
           digitalWrite(GLED, HIGH);
           delay(500);
           digitalWrite(GLED, LOW);
           delay(100);
        }
      }
      Serial.println("Pass color flashing");
      
      char res;
      // Goes into while loop to wait for Arduino to respond
      while(radio.available())
      {
          //digitalWrite(18, HIGH);
          res = radio.read((Data*)&packet, sizeof(packet));
          Serial.println(packet.len);
          //Serial.println("In While loop");
          Serial.println(res);
          if (res == 'C') //correct
          {
              digitalWrite(GLED, HIGH);
              delay(5000);
              digitalWrite(GLED, LOW);
              delay(500);
              genChar(packet);  
              break;
          }
     
          if (res == 'W') //lose
          {
              digitalWrite(RLED, HIGH);
              delay(5000);
              digitalWrite(RLED, LOW);
              delay(500);
              reset(packet); 
              break;
          }
      }      
   }
}

