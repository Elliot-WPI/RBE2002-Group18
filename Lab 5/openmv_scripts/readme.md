Edited versions of the example python scripts from OpenMV.

Both uart and i2c versions have been tested with their uC counterparts (`examples/apriltag_finder_x`) for a single tag in the field of view. Untested for detecting multiple tags simultaneously.

The UART version defaults to `UART 1`. The I2C version defaults to `I2C 2`. See the pinout in the root of this repo for connections.

The high resolution (`hires`) versions are experimental. They first find blobs of interest and then 'zoom in' on those areas, which saves memory and processing power (it's easier to find a blob than an apriltag). They have not been updated for the newest AprilTagDatum structure, though.
