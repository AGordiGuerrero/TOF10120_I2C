//
// Code to read distance measurements (in mm) from a TOF10120 LIDAR module. Run both on Arduino AVR or ESP32 boards.
//  
//  - Use timing with millis()
//  - Discard values out of a given range.
//  - Try to do "read_count_max" lectures before giving the message of "out of range value".
//  - Perform the mean of NMEASURES in range measures giving standard deviation and standard error using Student's coefficient.
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


#define DEC_POINT  1 // number of decimal digits in the shown values
#define T_STU 1.96   // Student coefficient value for a 95% confidence interval

unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag=0;

// Using different I2C addresses for testing
//unsigned int TOF_I2Caddress=82; //(0x52 hex) (default i2c address)
unsigned int TOF_I2Caddress=2; //(0x5 hex)
//unsigned int TOF_I2Caddress=3; //(0x03 hex)

// timing variable
int previousMillisRead=0;
int readPeriod=1000; //in milliseconds

// range variable
int max_x=400; // maximun allowed distance value
int min_x=40; // minimun allowed distance value
int read_count_max=100; // maximun numbers of measure trials before restarting the process

//statistical variable
int NMEASURES=30; // number of sensor readings to perform the average

 
void setup() {
  
  Wire.begin(); 
  
  Serial.begin(9600); 
 
}
 
void loop() {

    if (millis() - previousMillisRead >= readPeriod){

      previousMillisRead = millis();

      // statistical variable
      int8_t   i;
      float measure[NMEASURES];
      int8_t read_count;
      int good_count=0; // counts the good measures
      float average=0; //sample average
      float std_dev=0; //standard error of the mean 1/sqrt(N)*sqrt(\sum{pow(measure-average,2)}/N)
      float std_error=0; // standard deviation of the mean stdev/sqrt(N)
  
  
      good_count=0; // restart good_count
  
      //read until NMEASURES are obtained, or read_count_max trial measures have been performed
      for (read_count=0; read_count < read_count_max; read_count++){      

    
        int x=ReadDistance(TOF_I2Caddress);

        // in case of a good measure
        if ( (min_x <= x ) && (x <= max_x) ){

          good_count++; // increase read_count

          measure[read_count]= x;
          average+=x; //sample average
        
        }

        // if we have NMEASURES good ones
        if (good_count == NMEASURES){

          average/=(float)NMEASURES;

          for (i=0;i<NMEASURES;i++)
             std_dev+=pow(measure[i]-average,2);
           
          std_dev/=(float)NMEASURES;
          std_dev=sqrt(std_dev);
          std_error=std_dev/sqrt(NMEASURES);
          std_error*=T_STU;

          String av= String(average, DEC_POINT);
          String de= String(std_error, DEC_POINT);
          String dv= String(std_dev, DEC_POINT);


          Serial.print("We have obtained " );
          Serial.print(NMEASURES);
          Serial.print(" good measures after " );
          Serial.print(read_count+1);
          Serial.print(" trials. Distance = " );
          Serial.print(av); Serial.print(" +- ");  Serial.print(de);
          Serial.print(" mm.    Standard deviation of the mean=");  Serial.println(dv);
          break; // stopping forloop


        }
        
        if (read_count == read_count_max -1){

          Serial.print("We cannot obtaint a good measure after " );
          Serial.print(read_count);
          Serial.print(" trials, we only obtained ");
          Serial.print(good_count);
          Serial.println(" good ones. Restarting process...");
          break; // stopping forloop
        
        }

      
      } // end while obtaining a good measure
    
   } // end millis timing  
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
