// This example reads the raw values from the LSM6DS33
// accelerometer and gyro and prints those raw values to the
// Serial Monitor.

#include <Romi32U4Buttons.h>

#include <Wire.h> // I2C library
#include <LSM6.h>

// to control printout; you'll need the Romi-32u4-utilities library
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;

// the IMU
LSM6 imu;

void setup()
{
  Serial.begin(115200);

  Wire.begin();

  if (!imu.init())
  {
    // Failed to detect the LSM6.
    while(1)
    {
      Serial.println(F("Failed to detect the LSM6. Just smash that reset button."));
      delay(100);
    }
  }

  // TODO: Adjust the ODR and FS here when you get to that part. See LSM6.h/.cpp for options
}

bool showAcc = true;
bool showGyro = false;

void loop()
{
  // while you can print both, it will be messy because the order is not guaranteed
  if(buttonB.getSingleDebouncedPress()) showAcc = !showAcc;
  if(buttonC.getSingleDebouncedPress()) showGyro = !showGyro;

  if(showAcc)
  {
    if(imu.getStatus() & 0x01)
    {
      imu.readAcc();

      Serial.print(imu.a.x);
      Serial.print(' ');
      Serial.print(imu.a.y);
      Serial.print(' ');
      Serial.print(imu.a.z);
      Serial.print('\n');
    }
  }

  if(showGyro)
  {
    if(imu.getStatus() & 0x02)
    {
      imu.readGyro();
  
      Serial.print(imu.g.x);
      Serial.print(' ');
      Serial.print(imu.g.y);
      Serial.print(' ');
      Serial.print(imu.g.z);
      Serial.print('\n');
    }
  }
}
