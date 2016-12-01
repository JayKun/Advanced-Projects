#include <Wire.h>

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
  Wire.requestFrom(MPU_addr, 2, true); 
  *ax = Wire.read() << 8 | Wire.read(); 

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

void getGyroData( int16_t* gx,int16_t* gy, int16_t* gz)
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

void setGyroPrec(gyro_precision_e prec)
{
  prec&= 0b11;
  prec = prec << 3;
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x1B);
  Wire.write(prec);
  Wire.endTransmission(true);
}

void setAccelPrec(accel_precision_e prec)
{
  prec&= 0b11;
  prec = prec << 3;
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x1C);
  Wire.write(prec);
  Wire.endTransmission(true);
}
  
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
