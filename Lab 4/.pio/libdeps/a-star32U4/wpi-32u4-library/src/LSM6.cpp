#include <Wire.h>
#include <LSM6.h>
#include <math.h>

// Defines ////////////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define DS33_SA0_HIGH_ADDRESS 0b1101011
#define DS33_SA0_LOW_ADDRESS 0b1101010

#define TEST_REG_ERROR -1

#define DS33_WHO_ID 0x69

// Constructors ////////////////////////////////////////////////////////////////

LSM6::LSM6(void)
{
  _device = device_auto;

  io_timeout = 0; // 0 = no timeout
  did_timeout = false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Did a timeout occur in readAcc(), readGyro(), or read() since the last call to timeoutOccurred()?
bool LSM6::timeoutOccurred()
{
  bool tmp = did_timeout;
  did_timeout = false;
  return tmp;
}

void LSM6::setTimeout(uint16_t timeout)
{
  io_timeout = timeout;
}

uint16_t LSM6::getTimeout()
{
  return io_timeout;
}

void LSM6::setFullScaleGyro(GYRO_FS gfs)
{
  uint8_t settings = readReg(LSM6::CTRL2_G);
  settings &= 0xf0; //clear sensitivity bits; can't use 125 setting
  switch (gfs)
  {
  case GYRO_FS245:
    writeReg(LSM6::CTRL2_G, settings | 0b00000000);
    mdps = 8.75;
    break;
  case GYRO_FS500:
    writeReg(LSM6::CTRL2_G, settings | 0b00000100);
    mdps = 17.5;
    break;
  case GYRO_FS1000:
    writeReg(LSM6::CTRL2_G, settings | 0b00001000);
    mdps = 35;
    break;
  case GYRO_FS2000:
    writeReg(LSM6::CTRL2_G, settings | 0b00001100);
    mdps = 70;
    break;
  default:
    Serial.println("Error setting gyro sensitivity!");
  }
}

void LSM6::setFullScaleAcc(ACC_FS afs)
{
  uint8_t settings = readReg(LSM6::CTRL1_XL);
  settings &= 0xf0; //clear sensitivity bits
  switch (afs)
  {
  case ACC_FS2:
    writeReg(LSM6::CTRL1_XL, 0b00000000);
    mg = 0.061;
    break;
  case ACC_FS4:
    writeReg(LSM6::CTRL1_XL, 0b00001000); 
    mg = 0.122;
    break;
  case ACC_FS8:
    writeReg(LSM6::CTRL1_XL, 0b00001100);
    mg = 0.244;
    break;
  case ACC_FS16:
    writeReg(LSM6::CTRL1_XL, 0b00000100);
    mg = 0.488;
    break;
  default:
    Serial.println("Error setting acc sensitivity!");
  }
}

void LSM6::setGyroDataOutputRate(ODR rate)
{
  if(rate < 0 || rate > ODR166k) 
  {
    Serial.println(F("Illegal gyro ODR"));
    return;
  }
  uint8_t settings = readReg(LSM6::CTRL2_G);
  settings &= 0x0f; //clear ODR bits
  writeReg(LSM6::CTRL2_G, settings | (rate << 4));
}

void LSM6::setAccDataOutputRate(ODR rate)
{
  if(rate < 0 || rate > ODR166k) 
  {
    Serial.println(F("Illegal acc ODR"));
    return;
  }

  uint8_t settings = readReg(LSM6::CTRL1_XL);
  settings &= 0x0f; //clear ODR bits
  writeReg(LSM6::CTRL1_XL, settings | (rate << 4));
}

bool LSM6::init(deviceType device, sa0State sa0)
{
  // perform auto-detection unless device type and SA0 state were both specified
  if (device == device_auto || sa0 == sa0_auto)
  {
    // check for LSM6DS33 if device is unidentified or was specified to be this type
    if (device == device_auto || device == device_DS33)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && testReg(DS33_SA0_HIGH_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
      {
        sa0 = sa0_high;
        if (device == device_auto)
        {
          device = device_DS33;
        }
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && testReg(DS33_SA0_LOW_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
      {
        sa0 = sa0_low;
        if (device == device_auto)
        {
          device = device_DS33;
        }
      }
    }

    // make sure device and SA0 were successfully detected; otherwise, indicate failure
    if (device == device_auto || sa0 == sa0_auto)
    {
      return false;
    }
  }

  _device = device;

  switch (device)
  {
  case device_DS33:
    address = (sa0 == sa0_high) ? DS33_SA0_HIGH_ADDRESS : DS33_SA0_LOW_ADDRESS;
    break;
  default:;
  }

  return true;
}

/*
Enables the LSM6's accelerometer and gyro. Also:
- Sets sensor full scales (gain) to default power-on values, which are
  +/- 2 g for accelerometer and 245 dps for gyro
- Selects 1.66 kHz (high performance) ODR (output data rate) for accelerometer
  and 1.66 kHz (high performance) ODR for gyro. (These are the ODR settings for
  which the electrical characteristics are specified in the datasheet.)
- Enables automatic increment of register address during multiple byte access
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void LSM6::enableDefault(void)
{
  if (_device == device_DS33)
  {
    // Accelerometer

    // 0x80 = 0b10000000
    // ODR = 1000 (1.66 kHz (high performance)); FS_XL = 00 (+/-2 g full scale)
    //writeReg(CTRL1_XL, 0x80);
    setFullScaleAcc(ACC_FS2);

    // Gyro

    // 0x80 = 0b010000000
    // ODR = 1000 (1.66 kHz (high performance)); FS_XL = 00 (245 dps)
    //writeReg(CTRL2_G, 0x80);
    setFullScaleGyro(GYRO_FS245);
    // Common

    // 0x04 = 0b00000100
    // IF_INC = 1 (automatically increment register address)
    writeReg(CTRL3_C, 0x04);
  }
}

void LSM6::writeReg(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  last_status = Wire.endTransmission();
}

uint8_t LSM6::readReg(uint8_t reg)
{
  uint8_t value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  last_status = Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM6::readAcc(void)
{
  Wire.beginTransmission(address);
  // automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
  Wire.write(OUTX_L_XL);
  Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t)6);

  uint16_t millis_start = millis();
  while (Wire.available() < 6)
  {
    if (io_timeout > 0 && ((uint16_t)millis() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }

  uint8_t xla = Wire.read();
  uint8_t xha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t zla = Wire.read();
  uint8_t zha = Wire.read();

  // combine high and low bytes
  a.x = (int16_t)(xha << 8 | xla);
  a.y = (int16_t)(yha << 8 | yla);
  a.z = (int16_t)(zha << 8 | zla);
}

// Reads the 3 gyro channels and stores them in vector g
void LSM6::readGyro(void)
{
  Wire.beginTransmission(address);
  // automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
  Wire.write(OUTX_L_G);
  Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t)6);

  uint16_t millis_start = millis();
  while (Wire.available() < 6)
  {
    if (io_timeout > 0 && ((uint16_t)millis() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }

  uint8_t xlg = Wire.read();
  uint8_t xhg = Wire.read();
  uint8_t ylg = Wire.read();
  uint8_t yhg = Wire.read();
  uint8_t zlg = Wire.read();
  uint8_t zhg = Wire.read();

  // combine high and low bytes
  g.x = (int16_t)(xhg << 8 | xlg);
  g.y = (int16_t)(yhg << 8 | ylg);
  g.z = (int16_t)(zhg << 8 | zlg);

  dps.x = g.x * mdps / 1000.;
  dps.y = g.y * mdps / 1000.;
  dps.z = g.z * mdps / 1000.;
}

// Reads all 6 channels of the LSM6 and stores them in the object variables
void LSM6::read(void)
{
  readAcc();
  readGyro();
}

// void LSM6::vector_normalize(vector<float> *a)
// {
//   float mag = sqrt(vector_dot(a, a));
//   a->x /= mag;
//   a->y /= mag;
//   a->z /= mag;
// }

// Private Methods //////////////////////////////////////////////////////////////

int16_t LSM6::testReg(uint8_t address, regAddr reg)
{
  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  if (Wire.endTransmission() != 0)
  {
    return TEST_REG_ERROR;
  }

  Wire.requestFrom(address, (uint8_t)1);
  if (Wire.available())
  {
    return Wire.read();
  }
  else
  {
    return TEST_REG_ERROR;
  }
}