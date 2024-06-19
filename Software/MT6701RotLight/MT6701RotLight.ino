//Adapted from here https://github.com/I-AM-ENGINEER?tab=repositories
#include "USB.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "MT6701.h"

#define SDA2 6
#define SCL2 5
#define PIN        2 
#define NUMPIXELS 2 
#define DELAYVAL 1500 // Time (in milliseconds) to pause between pixels

MT6701 encoder;
USBCDC USBSerial;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  delay(500);
  USBSerial.begin(115200);USB.begin();
  delay(2000);
  USBSerial.println("MT6701 test!");
  Wire.begin(SDA2, SCL2);
  encoder.initializeI2C();
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  float angle = encoder.angleRead();
  USBSerial.println(angle);
  
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color((int)angle, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }
  delay(100);
}
