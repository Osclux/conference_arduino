
#include <Servo.h>
#include <BeanSandbox.h>

BeanSandbox sbx;  // Object for interacting with LightBlue sandbox controls
boolean bConnected = false;  // Flag for determining if we have an existing connection or a new connection when we start our loop()

Servo servo;  // create servo object to control a servo 

uint16_t servoStill = 90;
uint16_t parallaxServoStill = 1500;
uint16_t parallaxServoFullCounterClockWise = 1700;
uint16_t parallaxServoFullClockWise = 1300;
uint16_t parallaxServoFullStillDiff = 1700-1500;

uint16_t previousLeftServoSpeed = servoStill/2;  
uint16_t previousRightServoSpeed = servoStill;
uint8_t maxSpeedTests = 10;
uint8_t speedTests = 0;
boolean controlModeIsByDragging = false;
int oldXValue = -1;

void setup() 
{ 
    // Initialize serial communications at 57600bps
  Serial.begin(57600);

  // Enable the wake on connect so the Arduino will wake upon BLE connection rather than waiting for serial input
  Bean.enableWakeOnConnect( true );

  // Initialize the touchpad to immediate mode
  sbx.setTouchpadMode( SBX_PADMODE_IMMEDIATE );

  servo.attach(2);  // attaches the servo on pin 2 to the servo object 
} 
 
 
void loop() 
{   
  // Check BLE connection state  
  if(!Bean.getConnectionState())
  {
    // If we are not connected, just give a heartbeat blink and sleep for 5 minutes
    Bean.setLed( 0, 180, 0 );
    Bean.sleep(100);
    Bean.setLed( 0, 0, 0 );
    Bean.sleep(300000);
    bConnected = false;
  }
  else
  {
    // If we are connected, begin processing
    if(!bConnected)
    {
      // If this is our first time through loop since connecting, initialize the control values
      bConnected = true;
      sbx.setTouchpadX(127);
      sbx.initializeValues();
    }

    // Set the touchpad mode based on checkbox 5
    if(sbx.isChecked(5))
    {
      controlModeIsByDragging = false;
    }
    else
    {
      controlModeIsByDragging = true;
    }

    // Set the onboard LED based on slider 5 for red intensity, touchpad X-axis for green intensity and touchpad Y-axis for blue intensity
    byte nRed = sbx.getSliderValue(5);
    byte nGreen = sbx.getTouchpadX();
    byte nBlue = sbx.getTouchpadY();
    Bean.setLed( nRed, nGreen, nBlue );

    byte speedFactor = sbx.getSliderValue(5);
    speedFactor = speedFactor == 0 ? 1 : speedFactor;
    long parallaxSpeed = parallaxServoStill;
    
    int xValue = sbx.getTouchpadX();
    
    
    if (controlModeIsByDragging) {
      if (oldXValue == -1) oldXValue = xValue;
      if (abs(oldXValue - xValue) < 100) {
        parallaxSpeed = parallaxServoStill + (xValue - oldXValue) * 2;
      }
      oldXValue = xValue;
      
    } else {
      // Calculate servo speed 
      // 0-100 is counterclockwise
      // 100-155 is still
      // 135-255 is clockwise
      
      
      if (xValue < 100 && xValue > 0) {
//        parallaxSpeed = parallaxServoFullCounterClockWise - ((xValue*2*speedFactor)/255);
        parallaxSpeed = parallaxServoStill + (100 - xValue);
      } else if (xValue > 155 && xValue < 255) {
//        parallaxSpeed = parallaxServoStill - (((xValue-135)*2*speedFactor)/255);
        parallaxSpeed = parallaxServoStill - (xValue-155);
      }
      
    }
    
    servo.writeMicroseconds(parallaxSpeed);
    // Give the Bean a slight rest
    if (parallaxSpeed == parallaxServoStill) {
      Bean.sleep(50);  
    } else {
      delay(20);  
    }
    
  }
//    long leftServoSpeed = Bean.readScratchNumber(1);
//    long rightServoSpeed = Bean.readScratchNumber(2);
//    uint8_t ledIntensity = 120;

        // Get the current acceleration with range of ±2g, 
   // and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units. 
//   AccelerationReading accel = Bean.getAcceleration();

   // Update LED color
//   uint16_t r = (abs(accel.xAxis)) / 4;
//   uint16_t g = (abs(accel.yAxis)) / 4;
//   uint16_t b = (abs(accel.zAxis)) / 4;
//   Serial.println("r=" + String(r) + " g=" + String(g) + " b" + String(b));
//   Serial.println("x=" + String(accel.xAxis) + " y=" + String(accel.yAxis) + " z" + String(accel.zAxis));
//   Bean.setLed((uint8_t)r,(uint8_t)g,(uint8_t)b);
//   long parallaxSpeed = 1500;
//   if (abs(accel.xAxis) > 20) {
//      parallaxSpeed = 1500 + accel.xAxis; 
//   } 
//    if (leftServoSpeed != previousLeftServoSpeed) {
//        long parallaxSpeed = 1300 + (parallaxServoFullStillDiff*(leftServoSpeed/(servoStill/2)));
//        Serial.println("servoStill=" + String(servoStill));
//        Serial.println("leftServoSpeed=" + String(leftServoSpeed));
//        Serial.println("parallaxServoFullStillDiff=" + String(parallaxServoFullStillDiff));
//        Serial.println("parallaxSpeed=" + String(parallaxSpeed));
//        leftServo.writeMicroseconds(parallaxSpeed);
//        delay(10);
//      }
//    if (leftServoSpeed != previousLeftServoSpeed) {
//      leftServo.write(leftServoSpeed);
//      Bean.setLed(ledIntensity, 0, 0);
//    }
//    if (rightServoSpeed != previousRightServoSpeed) {
//      rightServo.write(rightServoSpeed);    
//      Bean.setLed(0, 0, ledIntensity);
//    }
//    previousLeftServoSpeed = leftServoSpeed;
//    previousRightServoSpeed = rightServoSpeed;
 
//   if (speedTests < maxSpeedTests) {
//      leftServo.writeMicroseconds(1700 - speedTests*20);  // Counter clockwise
//      delay(2000);                      // Wait 2 seconds
//      leftServo.writeMicroseconds(1300 + speedTests*20);  // Clockwise
//      delay(2000);
//      leftServo.writeMicroseconds(1500);  // Stop
//      delay(2000);
//      speedTests++;
//   }
//    delay(100);
//    Bean.sleep(1000);
}
