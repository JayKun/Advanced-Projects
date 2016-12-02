#include <Wire.h> // for communication between Arduino and MPU
#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

struct Data
{
  uint16_t ax, ay, az;
  uint16_t gx, gy, gz;
};

uint64_t pipes[2] = {0xc2c2c2c2c2, 0xe7e7e7e7e7};
RF24 radio(9, 10);
Data packet;

void initRadio() {
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(12);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.setChannel(23);
  delay(1000);
  radio.printDetails();
}

typedef enum : uint8_t
{
  GYRO_PREC_250 = 0,
  GYRO_PREC_500,
  GYRO_PREC_1000,
  GYRO_PREC_2000
} gyro_precision_e;
typedef enum : uint8_t
{
  ACCEL_PREC_2 = 0,
  ACCEL_PREC_4,
  ACCEL_PREC_8,
  ACCEL_PREC_16
} accel_precision_e;

const int MPU_addr = 0x68; 

void setSleep(bool enable)
{
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_addr, 1, true);
  uint8_t power = Wire.read(); // read 0x6B
  power = (enable) ? (power | 0b01000000) : (power & 0b10111111);// If 1, we set the 7th bit 1, If 0, we set the 7th bit to 0

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(power); 
  Wire.endTransmission(true);
}

void getAccelData(uint16_t* ax, uint16_t* ay, uint16_t* az)
{
  // x-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true); // reading two bytes, stored in 3B and 3C
  *ax = Wire.read() << 8 | Wire.read(); // most significant 8 bits from h register, least significant 8 bits from l register

  // y-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3D); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *ay = Wire.read() << 8 | Wire.read(); 

  // z-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3F); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true); 
  *az = Wire.read() << 8 | Wire.read(); 
}

void getGyroData(uint16_t* gx,uint16_t* gy, uint16_t* gz)
{
  // x-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true); 
  *gx = Wire.read() << 8 | Wire.read(); 

  // y-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x45); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true); 
  *gy = Wire.read() << 8 | Wire.read(); 

  // z-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true); 
  *gz = Wire.read() << 8 | Wire.read();
   
  }

void setGyroPrec(uint8_t prec)
{
  prec&= 0b11; // ensure 2-bit value
  prec = prec << 3; // setting 2 precision bits in 3rd and 4th bits
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x1B);
  Wire.write(prec);
  Wire.endTransmission(true);
}

void setAccelPrec(uint8_t prec)
{
  prec&= 0b11;
  prec = prec << 3;
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x1C);
  Wire.write(prec);
  Wire.endTransmission(true);
}
  
void setup() {
  Wire.begin();
  setSleep(false);
  setGyroPrec(GYRO_PREC_1000); 
  setAccelPrec(ACCEL_PREC_2); 
  
  Serial.begin(9600); 
  initRadio();
}

void loop() {

  getAccelData(&(packet.ax), &(packet.ay), &(packet.az)); 
  getGyroData(&(packet.gx), &(packet.gy), &(packet.gz)); 

  radio.write((Data*)&packet, sizeof(packet));

  /* Serial.println("Acceleration Data: "); 
  Serial.print("ax-direction: ");
  Serial.println(ax);
  
  delay(1000); 
  
  Serial.print("ay-direction: ");
  Serial.println(ay);
  
  delay(1000); 
  
  Serial.print("az-direction: ");
  Serial.println(az);
  
  delay(1000); 

  Serial.println("Gyro Data: ");
  Serial.print("gx-direction: ");
  Serial.println(gx);
  
  delay(1000); 
  
  Serial.print("gy-direction: ");
  Serial.println(gy);
  
  delay(1000); 
  
  Serial.print("gz-direction: ");
  Serial.println(gz);
  
  delay(1000); */
}
