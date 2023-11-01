
### Introductory questions (Section 1.1)

> What IMU chip is used on the Romi?

> What is the I2C address of the chip _as implemented on the Romi Control Board_?

> What is the largest (nominal) acceleration you can measure with the IMU (in _g_)?

### Basic functionality (Section 1.2)

> Indicate the *positive* direction for each axis of the accelerometer and each axis of the gyroscope using one of UP, DOWN, LEFT, RIGHT, FRONT, BACK, STRANGE, CHARM. The +z accelerometer is done for you. 

| Device | +x | +y | +z |
|---|:---:|:---:|:---:| 
| Accelerometer | | |UP| 
| Gyroscope |  | |  | 
|
> Are the coordinate systems for the accelerometer and gyroscope coincident?

> Fill in the table below for your predictions for the accelerometer readings.

| Orientation | a_x | a_y | a_z |
|---|:---:|:---:|:---:| 
| Top pointing up |  | | | 
| Front pointing up |  |  |  | 
| Left pointing up |  |  |  | 
| Right pointing up |  |  |  | 
| Bottom pointing up | | |   | 
| Rear pointing up | | ||
|

### Calibration (Section 1.3)

> Record the default values for the following.

| Sensor | ODR | FS | Sensitivity (from datasheet) |
|:---:|:---:|:---:|:---:|
| Gyro |  | | |
| Acc | |  |  |
|

> Describe your experiment to measure the sensitivity of the accelerometer. Record your results. How do your measurements compare to the datasheet values above? Write the offset in milligravities.

| Axis | Sensitivity | Offset|
|:---:|:---:|:---:|
| x | | |
| y | | |
| z | | |
|

> Record the standard deviation of the accelerometer readings when sitting still.

| Axis | Std. dev. (in _mg_)
|:---:|:---:|
| x | | 
| y | | 
| z | | 
|
> If the Romi were spinning on level ground at 1 revolution per second and the gyroscope was set to a full-scale of 500 dps, what value would the IMU return on each gyro axis? Ignore noise and bias. Use the conversion factor given in Table 3 of the datasheet.


### Gyroscope (Section 1.5)

> Describe your experiment to measure the gyroscope bias and noise and your results.


### The `STATUS` Register (Section 2.1)

> Explain what the following line of code is doing: `if(imu.getStatus() & 0x01)`


### Basic angle calculations (Section 2.3)

> Describe the basic behavior of your angle calculator. Does is get the angle correct over a complete 360?