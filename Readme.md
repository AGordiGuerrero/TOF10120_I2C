# TOF10120 module I2C usage

Simple codes to read distance measurements (in mm) from a [TOF10120 LIDAR module](https://www.banggood.com/search/tof10120.html?from=nav). Based on [VL53L0X chip](https://www.banggood.com/search/vl53lox.html?from=nav). Both of them quite cheap.



Required libraries:
  - Wire.h 

Codes:
  - TOF10120_i2c_V1: 
    - Version with delays for defining timing. Can be used for different i2c addresses.
  - TOF10120_i2c_V2:
    - Version with `millis()` for defining timing.
  - TOF10120_i2c_discardstranges_V1: 
    - Discard values out of a given range.
    - Try to do some lectures before giving the message of "out of range value".
  - TOF10120_i2c_average_discardstranges_V1: 
    - Discard values out of a given range.
    - Try to do some lectures before giving the message of "out of range value".
    - Perform the average of `NMEASURES` in range measures, giving standard deviation and standard error using 95% Student's coefficient.

Adapted from code of [https://surtrtech.com](https://surtrtech.com/2019/03/18/easy-use-of-tof-10120-laser-rangefinder-to-measure-distance-with-arduino-lcd/).

You can find the whole information (in Spanish) at [Smart Open Lab](https://www.smartopenlab.com/proyecto/tutorial-de-uso-del-sensor-lidar-tof10120/)
