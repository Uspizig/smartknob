#include "USB.h"
#include <Wire.h>
#include "MT6701.h"
#define SDA2 6
#define SCL2 5

MT6701 encoder;
USBCDC USBSerial;

void setup() {
  delay(500);
  USBSerial.begin(115200);USB.begin();
  delay(2000);
  USBSerial.println("MT6701 test!");
  Wire.begin(SDA2, SCL2);
  encoder.initializeI2C();
}

void loop() {
  float angle = encoder.angleRead();
  USBSerial.println(angle);
  delay(100);
}
