#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

struct Data
{
  int16_t ax, ay, az;
  uint8_t buttonLeft;
  uint8_t buttonRight;
};

const int moveThreshold = 200;
const float scalingFactor = 0.0035;

uint64_t pipes[2] = {0xc2c2c2c2c2, 0xe7e7e7e7e7};
RF24 radio(9, 10);
Data packet;

void initRadio() {
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(8);
  radio.openReadingPipe(1, pipes[1]);
  radio.openWritingPipe(pipes[0]);
  radio.setChannel(23);
  delay(1000);
  radio.printDetails();
}

void setup() {
  Serial.begin(9600);
  printf_begin();
  initRadio();

  Mouse.begin();
}

void loop() {
  radio.startListening();
  if (radio.available())
  {
    Serial.println("Receiving");
    while (radio.available())
    {
      
      radio.read((Data*)&packet, sizeof(packet));

      // button presses

      if (packet.buttonLeft == 'H') {
        Mouse.set_buttons(1, 0, 0);
   
      }

      if (packet.buttonRight == 'H') 
       Mouse.set_buttons(0, 0, 1);
      
      
      if(packet.buttonRight == 'L' && packet.buttonLeft == 'L')
        Mouse.set_buttons(0,0,0); 

        
      int ax = packet.ay; // switch x and y
      int ay = packet.ax;

      if (abs(ax) < 250)
        ax = 0;
      if (abs(ay) < 250)
        ay = 0;
      Mouse.move(scalingFactor * ax, -(scalingFactor * ay));

    }
  }
}
