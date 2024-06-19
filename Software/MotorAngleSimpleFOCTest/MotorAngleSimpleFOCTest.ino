//https://github.com/simplefoc/Arduino-FOC-drivers/tree/master/src/encoders/mt6701

#include "Arduino.h"
#include "SPI.h"
#include "SimpleFOC.h"
#include "SimpleFOCDrivers.h"
//#include "encoders/MT6701/MagneticSensorMT6701SSI.h"
#include "mt6701_sensor.h"

#include "USB.h"


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

#define SENSOR1_CS 4
#define SENSOR1_SCK 5
#define SENSOR1_MOSI 6

MT6701Sensor sensor1 = MT6701Sensor();
USBCDC USBSerial;


// BLDC motor & driver instance
BLDCMotor motor = BLDCMotor(7);
BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_UH, PIN_UL, PIN_VH, PIN_VL, PIN_WH, PIN_WL);


// angle set point variable
float target_angle = 0;



void setup() {
    delay(500);
    USBSerial.begin(115200);USB.begin();
    delay(2000);
    USBSerial.println("MT6701 test!");
    //Wire.begin(SDA2, SCL2);
    //spi.begin(sck miso mosi ss)
    //SPI.begin(SENSOR1_SCK, -1, SENSOR1_MOSI, SENSOR1_CS);
    sensor1.init(); // initialise magnetic sensor hardware
    
    
    motor.linkSensor(&sensor1);// link the motor to the sensor


  // driver config
  driver.voltage_power_supply = 5;// power supply voltage [V]
  driver.voltage_limit = 5;
  driver.pwm_frequency = 32000;
  driver.init();
  
  motor.linkDriver(&driver);// link the motor and the driver

  // contoller configuration
  // default parameters in defaults.h

  motor.voltage_limit = 3;   // [V]

  // set motion control loop to be used
  motor.controller = MotionControlType::angle;
  motor.useMonitoring(USBSerial);

  // velocity PI controller parameters
  motor.PID_velocity.P = 0.2f;
  motor.PID_velocity.I = 20;
  
  // velocity low pass filtering time constant
  // the lower the less filtered
  motor.LPF_velocity.Tf = 0.01f;

  // angle P controller
  motor.P_angle.P = 20;
  // maximal velocity of the position control
  motor.velocity_limit = 40;

   // initialize motor
  motor.init();
  sensor1.update();
  // align sensor and start FOC
  motor.initFOC();

  target_angle = 10;

}

void loop(){
  // main FOC algorithm function
  // the faster you run this function the better
  // Arduino UNO loop  ~1kHz
  // Bluepill loop ~10kHz
  motor.loopFOC();

  // Motion control function
  // velocity, position or voltage (defined in motor.controller)
  // this function can be run at much lower frequency than loopFOC() function
  // You can also use motor.move() and set the motor.target in the code
  motor.move(target_angle);


  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  // motor.monitor();

  USBSerial.println(sensor1.getSensorAngle());
}