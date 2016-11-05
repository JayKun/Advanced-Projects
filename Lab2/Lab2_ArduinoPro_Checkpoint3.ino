#include <SPI.h>

#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

RF24 radio(9, 10);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  //radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);

  radio.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
}
