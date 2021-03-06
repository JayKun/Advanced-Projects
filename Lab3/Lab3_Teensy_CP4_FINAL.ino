#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

struct Data
{
 int16_t ax, ay, az;
 int16_t gx, gy, gz; 
 };

uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);
Data packet; 

void initRadio(){
  radio.begin();   
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_8);
  radio.setPayloadSize(12);
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
  Mouse.screenSize(1920, 1080)
  
}

void loop() {
  radio.startListening();
  if(radio.available())
  {
    while(radio.available())
    {
      radio.read((Data*)&packet, sizeof(packet)); 
   }

  
  Serial.print("Acceleration Data (x,y,z): "); 
  Serial.print(packet.ax);
  Serial.print(" "); 
  Serial.print(packet.ay);
  Serial.print(" "); 
  Serial.println(packet.az);


  Serial.print("Gyroscope Data (x,y,z):    ");
  Serial.print(packet.gx);
  Serial.print(" "); 
  Serial.print(packet.gy);
  Serial.print(" "); 
  Serial.println(packet.gz);
  Serial.println(" ");



  
  }

}
