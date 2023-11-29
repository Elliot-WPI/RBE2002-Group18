# openmv-apriltags

Library for receiving apriltag data from an OpenMV camera, including scripts for the OpenMV camera and code for the receiver. Examples are built for the ATmega32U4, as found on the Leonardo or Pololu's Romi.

At a minimum, you will need to connect GND and the communication bus of your choice (there are examples for both I2C and UART). To run "headless," you'll need to power the camera with 3.3 - 5V, as well.

Pinout of camera:
![cam-h7-pinout](https://user-images.githubusercontent.com/68814774/160293164-34f471f1-776b-469a-91a8-5dd6d0f36d50.png)
