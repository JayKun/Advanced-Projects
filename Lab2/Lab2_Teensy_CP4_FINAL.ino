#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

#define RLED 15
#define YLED 18
#define GLED 21

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);

struct Data
{
  uint8_t c[20]; 
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
  // RF module setup
  radio.begin(); 
  Serial.begin(9600); 
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(pipes[0]); 
  radio.openReadingPipe(1, pipes[1]);

  // old game code
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  randomSeed(analogRead(0));
  genChar(packet);  
}

void loop() {
  // 1. Send bytes
  //memcpy(s, &packet, sizeof(Data)); //not necessary, below line was modified
  /* Serial1.write((uint8_t*)&packet, sizeof(Data)); //or cast as char 
  Serial.println(sizeof(Data));
  Serial.println((char*)packet.c); */

  radio.write((Data*)&packet, sizeof(packet));
  
  // 2. Flash each corresponding LED to Player 1
  for(int k = 0; k < packet.len; k++)
  {
    //Serial.println("In loop");
    char color = packet.c[k];
    Serial.println(color); 
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

    // Goes into while loop to wait for Arduino to respond
   while(true)
   {
    radio.startListening();

    if(radio.available())
    {
      Serial.println("Teensy radio available");
      while(radio.available())
      {
        // reading
        uint8_t charPassed;
        radio.read((uint8_t*)&charPassed, sizeof(charPassed)); 
        Serial.println((char)charPassed);

        if ((char)charPassed == 'C') //correct
        {
          digitalWrite(GLED, HIGH);
          delay(5000);
          digitalWrite(GLED, LOW);
          delay(500);
          genChar(packet);
          goto next; 
        }
     
        if ((char)charPassed == 'W') //lose
        {
          digitalWrite(RLED, HIGH);
          delay(5000);
          digitalWrite(RLED, LOW);
          delay(500);
          reset(packet); 
          genChar(packet); //restart
          goto next;
        }
      }
    }
  }
  next:
  {
    Serial.println("next");
    radio.stopListening();
  }
}      
