#include <Wire.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

#define buttonL 2
#define buttonR A3
#define sensorPin A0

struct Data
{
 int16_t ax, ay, az;
 uint8_t buttonLeft; 
 uint8_t buttonRight;
 // uint8_t batteryLevel; 
};
 
Data packet; 
uint64_t pipes[2] ={0xc2c2c2c2c2, 0xe7e7e7e7e7}; 
RF24 radio(9, 10);

void initRadio()
{
  radio.begin();
  radio.setChannel(23);
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(8);
  radio.openWritingPipe(pipes[1]); 
  radio.openReadingPipe(1, pipes[0]);
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

void getAccelData(int16_t* ax, int16_t* ay, int16_t* az)
{
  // x-direction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); 
  *ax = Wire.read() << 8 | Wire.read(); 
  *ay = Wire.read() << 8 | Wire.read(); 
  *az = Wire.read() << 8 | Wire.read(); 
}

void getGyroData(int16_t* gx,int16_t* gy, int16_t* gz)
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
  prec&= 0b11;
  prec = prec << 3;
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

int16_t cal_ax = 0, cal_ay = 0, cal_az = 0; 
int16_t ax = 0, ay = 0, az = 0; 

void sensorCalibration() {
  int16_t calibration_ax = 0;
  int16_t calibration_ay = 0;
  int16_t calibration_az = 0;
  
  for (int i = 0; i < 100; i++) {
    getAccelData(&calibration_ax, &calibration_ay, &calibration_az);
    cal_ax += calibration_ax;
    cal_ay += calibration_ay;
    cal_az += calibration_az;
  }

  cal_ax /= 100;
  cal_ay /= 100;
  cal_az /= 100;
}


void setup() {
  Wire.begin();
  setSleep(false);
  setGyroPrec(GYRO_PREC_1000); 
  setAccelPrec(ACCEL_PREC_2); 
  Serial.begin(9600); 
  initRadio(); 
  printf_begin(); 
  radio.printDetails();
  getAccelData(&cal_ax, &cal_ay, &cal_az); 
  //sensorCalibration();
}


void loop() {

  // button presses

  int leftReading = digitalRead(buttonL); 
  int rightReading = digitalRead(buttonR);

  if (leftReading == HIGH) {
    packet.buttonLeft = 'H';
  }

  if (rightReading == HIGH) {
    packet.buttonRight = 'H';
  }

  // mouse moving
  
  getAccelData(&ax, &ay, &az); 
  
  ax = ax - cal_ax; 
  ay = ay - cal_ay; 
  az = az - cal_az; 

  
  packet.ax = ax; 
  packet.ay = ay;
  packet.az = az; 
  
  Serial.print("Acceleration Data (x,y,z): "); 
  Serial.print(ax);
  Serial.print(" "); 
  Serial.print(ay);
  Serial.print(" "); 
  Serial.println(az);
  radio.write((Data*)&packet, sizeof(packet));

  packet.buttonLeft = 'L';
  packet.buttonRight = 'L';
}
