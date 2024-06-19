/**
 *
 * Velocity motion control example
 * Steps:
 * 1) Configure the motor and magnetic sensor
 * 2) Run the code
 * 3) Set the target velocity (in radians per second) from serial terminal
 *
 *
 * By using the serial terminal set the velocity value you want to motor to obtain
 *
 */
#include <SimpleFOC.h>
#include "MT6701.h"
#include "USB.h"
#include <Adafruit_NeoPixel.h>

#define SDA2 6
#define SCL2 5

#define PIN_VL 7
#define PIN_UH 8
#define PIN_WL 15
#define PIN_UL 16
#define PIN_WH 17
#define PIN_VH 18

#define FOC_PID_P 4
#define FOC_PID_I 0
#define FOC_PID_D 0.04
#define FOC_PID_OUTPUT_RAMP 10000
#define FOC_PID_LIMIT 10

#define FOC_VOLTAGE_LIMIT 5


#define LED_PIN    2
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 6

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
USBCDC USBSerial;

MT6701 sensor;

// BLDC motor & driver instance
BLDCMotor motor = BLDCMotor(7);
BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_UH, PIN_UL, PIN_VH, PIN_VL, PIN_WH, PIN_WL);

// velocity set point variable
float target_velocity = 5;
uint32_t last_update_ = 0;
// instantiate the commander
Commander command = Commander(USBSerial);
void doTarget(char* cmd) { command.scalar(&target_velocity, cmd); }

void setup() {

  delay(500);
  USBSerial.begin(115200);USB.begin();
  delay(2000);
  USBSerial.println("MT6701 test!");

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


  initMT6701();
  // link the motor to the sensor
  //motor.linkSensor(&sensor);

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 5;
  driver.voltage_limit = 5;
  driver.pwm_frequency = 32000;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

    // contoller configuration
  // default parameters in defaults.h

  motor.voltage_limit = 3;   // [V]

  // open loop control config
  motor.controller = MotionControlType::velocity_openloop;

  motor.useMonitoring(USBSerial);

  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();

  // add target command T
  //command.add('T', doTarget, "target velocity");

  USBSerial.println(F("Motor ready."));
  USBSerial.println(F("Set the target velocity"));
  colorWipe(strip.Color(0,   200,   0), 500); // Red
  delay(1000);
}

void loop() {
  

  // Motion control function
  // velocity, position or voltage (defined in motor.controller)
  // this function can be run at much lower frequency than loopFOC() function
  // You can also use motor.move() and set the motor.target in the code
  motor.move(target_velocity);

  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  // motor.monitor();

  // user communication
  //command.run();
  colorWipe(strip.Color(255,   0,   0), 0); // Red
}


void initMT6701(void){
  Wire.begin(SDA2, SCL2);
  sensor.initializeI2C();
}

float getSensorAngle(void) {
  uint32_t now = micros();
  float rad = 0;
    if (now - last_update_ > 100) {
      
      rad = sensor.angleRead();   
      last_update_ = now;
      USBSerial.println(rad);
    }
    
    if (rad < 0) {
        rad += 2*PI;
    }
  return rad;
}


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}