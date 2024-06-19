// Basic demo for accelerometer readings from Adafruit MPU6050

#include "USB.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define SDA2 9
#define SCL2 10

Adafruit_MPU6050 mpu;
USBCDC USBSerial;
void setup(void) {
  Wire.begin(SDA2, SCL2);
  delay(500);
  USBSerial.begin(115200);USB.begin();
  delay(2000);
  USBSerial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    USBSerial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  USBSerial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  USBSerial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    USBSerial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    USBSerial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    USBSerial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    USBSerial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  USBSerial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    USBSerial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    USBSerial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    USBSerial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    USBSerial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  USBSerial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    USBSerial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    USBSerial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    USBSerial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    USBSerial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    USBSerial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    USBSerial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    USBSerial.println("5 Hz");
    break;
  }

  USBSerial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  USBSerial.print("Acceleration X: ");
  USBSerial.print(a.acceleration.x);
  USBSerial.print(", Y: ");
  USBSerial.print(a.acceleration.y);
  USBSerial.print(", Z: ");
  USBSerial.print(a.acceleration.z);
  USBSerial.println(" m/s^2");

  USBSerial.print("Rotation X: ");
  USBSerial.print(g.gyro.x * 57.2958);
  USBSerial.print(", Y: ");
  USBSerial.print(g.gyro.y * 57.2958);
  USBSerial.print(", Z: ");
  USBSerial.print(g.gyro.z * 57.2958);
  USBSerial.println(" rad/s");

  USBSerial.print("Temperature: ");
  USBSerial.print(temp.temperature);
  USBSerial.println(" degC");

  USBSerial.println("");
  delay(500);
}