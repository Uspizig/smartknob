//Sleeps and wake ups on movement by interrupt
//https://github.com/adafruit/Adafruit_MPU6050/issues/32
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "USB.h"


#define SDA2 9
#define SCL2 10
#define LED_PIN    2
#define LED_COUNT 6
#define INTERRUPTPin 11

USBCDC USBSerial;
Adafruit_MPU6050 mpu;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int val = 0;
int int_trigger = 0;

void Read_MPU_ISR() {
  //int z = mpu.getAngleZ();
  //USBSerial.print("Z_Angle from int: ");
  //USBSerial.println(z);
  int_trigger = 1;
}

void setup(void) {
  
  
  Wire.begin(SDA2, SCL2);
  delay(500);
  USBSerial.begin(115200);USB.begin();
  delay(2000);
  USBSerial.println("Adafruit MPU6050 test!");
  pin_auslesen(INTERRUPTPin);
  // Try to initialize!
  if (!mpu.begin()) {
    USBSerial.println("Failed to find MPU6050 chip");
  }
  else{
    USBSerial.println("MPU6050 Found!");
  }
  print_wakeup_reason();
  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(10); //1
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  //mpu.setInterruptEnabled(true);
  pinMode(INTERRUPTPin, INPUT); 
  //attachInterrupt(digitalPinToInterrupt(INTERRUPTPin), Read_MPU_ISR, CHANGE); // Trigger interrupt on rising edge
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPin), Read_MPU_ISR, FALLING); // Trigger interrupt on rising edge

  USBSerial.println("");
  //delay(100);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {

  if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    /*USBSerial.print("AccelX:");
    USBSerial.print(a.acceleration.x);
    USBSerial.print(",");
    USBSerial.print("AccelY:");
    USBSerial.print(a.acceleration.y);
    USBSerial.print(",");
    USBSerial.print("AccelZ:");
    USBSerial.print(a.acceleration.z);
    USBSerial.print(", ");
    USBSerial.print("GyroX:");
    USBSerial.print(g.gyro.x);
    USBSerial.print(",");
    USBSerial.print("GyroY:");
    USBSerial.print(g.gyro.y);
    USBSerial.print(",");
    USBSerial.print("GyroZ:");
    USBSerial.print(g.gyro.z);
    USBSerial.println("");*/
    
    //colorWipe(strip.Color(  0,   0, 255), 50); // Blue
  
    if (int_trigger == 1){
      USBSerial.println("INT erkannt");
      if (a.acceleration.z > 0) {
        colorWipe(strip.Color(255,   0,   0), 5); // Red
      }
      else {
        colorWipe(strip.Color(  0, 255,   0), 5); // Green
      }
      int_trigger = 0;
      delay(500);
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_11,0); //1 = High, 0 = Low
      USBSerial.println("Going to sleep now");
      delay(1000);
      esp_deep_sleep_start();
    }
  }

  
  //pin_auslesen(INTERRUPTPin);
}


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : USBSerial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : USBSerial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : USBSerial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : USBSerial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : USBSerial.println("Wakeup caused by ULP program"); break;
    default : USBSerial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void pin_auslesen(int pin){
  val = digitalRead(pin);   // read the input pin
  if (digitalRead(pin) == HIGH) {
  //USBSerial.println("Pin is HIGH");
  }
  else {
 //   USBSerial.println("Pin is LOW");
  }
}