#include "Gyro.h"

Gyro::Gyro() {}

/**
 * Initialize and enable the IMU
 * This must be called during the setup function
 */
void Gyro::setup()
{
  Wire.begin();

  if (!imu.init())
  {
    // Failed to detect the LSM6.
    while (1)
    {
      Serial.println(F("Failed to detect the LSM6."));
      delay(100);
    }
  }
  imu.enableDefault();

  // Set the gyro full scale to 1000 dps because the default
  // value is too low, and leave the other settings the same.
  imu.writeReg(LSM6::CTRL2_G, 0b10001000);

  // Set the accelerometer full scale to 16 g because the default
  // value is too low, and leave the other settings the same.
  imu.writeReg(LSM6::CTRL1_XL, 0b10000100);
  initialize();
}

/**
 * Reset the gyro heading
 * Sets the heading to zero without changing the offset calculations
 */
void Gyro::reset()
{
  heading = 0.0;
}

/**
 * Recalculate the gyro offset.
 * The gyro offset is calculated by sampling gyro values for a few seconds
 * while the robot is motionless at the start of the program. This is done by
 * averaging the non-moving zero rate values and using that average as a
 * baseline for actual readings.
 */
void Gyro::initialize()
{
  // Sample the z velocities
  long sum = 0;
  int samples = 2001;
  for (int i = 0; i < samples; i++)
  {
    imu.readGyro();
    sum += imu.g.z;
    delay(1);
  }
  average = sum / samples;
  heading = 0.0;
  gyroTimer = millis() + 10;
  lastTime = 0;
  Serial.print("Average: ");
  Serial.println(average);
}

/**
 * Return the gyro heading in degrees
 * This only works if loop is called inside the loop function at least every
 * 10 ms. An improvement would be to measure the time since the last loop
 * call and multiplying by the time factor.
 */
float Gyro::getHeading() { return heading; }

/**
 * This function must be called in the program loop function to ensure that
 * the gyro angle continues to be updated by integrating the rates
 */
void Gyro::loop()
{
  if (millis() >= gyroTimer)
  {
    imu.readGyro();
    heading += (imu.g.z - average) * 0.035 / 100;
    gyroTimer = millis() + 10;
  }
}
