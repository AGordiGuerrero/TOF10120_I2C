//
// Simple code to read distance measurements (in mm) from a TOF10120 LIDAR module. Run both on Arduino AVR or ESP32 boards.
//  
//   V2: include better timing using milis()
//
// Thanks to:
//  - Adapted from code from SurfTech (https://surtrtech.com/2019/03/18/easy-use-of-tof-10120-laser-rangefinder-to-measure-distance-with-arduino-lcd/)
//  - Arduino and ESP32 online open community. Thank you all!
// 
// Licensed under Creative Commons (SH-BY-NC). @AGordiGuerrero
//.
// Antonio Gordillo Guerrero
// anto@unex.es
// @AGordiGuerrero
// Smart Open Lab. Escuela Politécnica de Cáceres. Universidad de Extremadura.
// www.smartopenlab.com
// @SolEpcc

 
#include <Wire.h>
 
unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag=0;

// Using different I2C addresses for testing
unsigned int TOF_I2Caddress=82; //(0x52 hex) (default i2c address)
//unsigned int TOF_I2Caddress=2; //(0x5 hex)
//unsigned int TOF_I2Caddress=3; //(0x03 hex)

//timing variable
int previousMillisRead=0;
int readPeriod=100; //in milliseconds


 
void setup() {
  Wire.begin(); 
  Serial.begin(9600); 
 
}
 
void loop() {

    if (millis() - previousMillisRead >= readPeriod){

      previousMillisRead = millis();

      int x=ReadDistance(TOF_I2Caddress);
      Serial.print(x);
      Serial.println(" mm");
    
    }  
}
  
 
void SensorRead(unsigned char* datbuf, int TOF_I2Caddress) 
{
  unsigned short result=0;
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(TOF_I2Caddress); // transmit to device 
  Wire.write(byte(0x00));      // sets distance data address (0x00)
  Wire.endTransmission();      // stop transmitting
  // step 2: wait for readings to happen
  delay(1);                   // datasheet suggests at least 30uS
  // step 3: request reading from sensor
  Wire.requestFrom(TOF_I2Caddress, 2);    // request 2 bytes from slave device #82 (0x52)
  // step 5: receive reading from sensor
  if (2 <= Wire.available()) { // if two bytes were received
    *datbuf++ = Wire.read();  // receive high byte (overwrites previous reading)
    *datbuf++ = Wire.read(); // receive low byte as lower 8 bits
  }
}
 
int ReadDistance(int TOF_I2Caddress){
    SensorRead(i2c_rx_buf, TOF_I2Caddress);
    lenth_val=i2c_rx_buf[0];
    lenth_val=lenth_val<<8;
    lenth_val|=i2c_rx_buf[1];
    return lenth_val;
}
