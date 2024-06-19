/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include "USB.h"
#include <Wire.h>
#define SDA2 9
#define SCL2 10

USBCDC USBSerial;

void setup() {
  Wire.begin(SDA2, SCL2);
  delay(500);
  USBSerial.begin(115200);USB.begin();
  delay(2000);
  USBSerial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  USBSerial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      USBSerial.print("I2C device found at address 0x");
      if (address<16) {
        USBSerial.print("0");
      }
      USBSerial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      USBSerial.print("Unknow error at address 0x");
      if (address<16) {
        USBSerial.print("0");
      }
      USBSerial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    USBSerial.println("No I2C devices found\n");
  }
  else {
    USBSerial.println("done\n");
  }
  delay(5000);          
}