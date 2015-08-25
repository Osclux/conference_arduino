
#include <Servo.h>
#include <BeanSandbox.h>

BeanSandbox sbx;  // Object for interacting with LightBlue sandbox controls
boolean bConnected = false;  // Flag for determining if we have an existing connection or a new connection when we start our loop()

Servo servo;  // create servo object to control a servo 

const long servoStill = 1500;
const long servoFullCounterClockWise = 2100;
const long servoFullClockWise = 900;
const int servoPin = 2;

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
    //Bean.setLed( nRed, nGreen, nBlue );

    byte speedFactor = sbx.getSliderValue(5);
    speedFactor = speedFactor == 0 ? 1 : speedFactor;
    long servoSpeed = servoStill;
    
    int xValue = sbx.getTouchpadX();
    
    
    if (controlModeIsByDragging) {
      if (oldXValue == -1) oldXValue = xValue;
      if (abs(oldXValue - xValue) < 100) {
        servoSpeed = servoStill + (xValue - oldXValue) * 2;
      }
      oldXValue = xValue;
      
    } else {
      // Calculate servo speed 
      // 0-100 is counterclockwise
      // 100-135 is still
      // 135-255 is clockwise
      
      
      if (xValue < 100 && xValue > 0) {
//        parallaxSpeed = parallaxServoFullCounterClockWise - ((xValue*2*speedFactor)/255);
        servoSpeed = servoStill + (100 - xValue);
      } else if (xValue > 135 && xValue < 255) {
//        parallaxSpeed = parallaxServoStill - (((xValue-135)*2*speedFactor)/255);
        servoSpeed = servoStill - (xValue-135);
      }
      
    }
    
    
    if (servoSpeed == servoStill) {
      // Turn off led
      Bean.setLed(0,0,0);
      // Detach servo to make it stand till
      servo.detach();
      // Sleep the bean to save a little power
      Bean.sleep(20);  
    } else {
      // Light up the led to indicate operation
      Bean.setLed(100, 0, 0);
      // Attach servo pin if not attached
      if (!servo.attached()) {
        servo.attach(2);
      }
      // Write the servo speed
      servo.writeMicroseconds(servoSpeed);
      // Delay execution a bit to make the servo operate smoother
      delay(20);  
    }
    
  }

}
