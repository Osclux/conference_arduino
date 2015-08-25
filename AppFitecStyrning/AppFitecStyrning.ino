
#include <Servo.h>

boolean bConnected = false;  // Flag for determining if we have an existing connection or a new connection when we start our loop()

Servo servo;  // create servo object to control a servo 

const long servoStill = 1500;
const long servoFullCounterClockWise = 2100;
const long servoFullClockWise = 900;
const int servoPin = 2;
const long MIN_APP_VALUE = -100;
const long MAX_APP_VALUE = 100;

int oldXValue = -1;

void setup() 
{ 
    // Initialize serial communications at 57600bps
  //Serial.begin(57600);

  // Enable the wake on connect so the Arduino will wake upon BLE connection rather than waiting for serial input
  Bean.enableWakeOnConnect( true );

  
} 
 
 
void loop() 
{   
  // Check BLE connection state  
  if(!Bean.getConnectionState())
  {
    // If we are not connected, just give a heartbeat blink and sleep for 5 minutes
    Bean.setLed( 0, 180, 0 );
    Bean.sleep(100);
    Bean.setLed(0,0,0);
    Bean.sleep(20);
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
    }

    
    long valueFromApp = Bean.readScratchNumber(1);
    // Limit values from -100 to +100
    if (valueFromApp < MIN_APP_VALUE) {
      valueFromApp = MIN_APP_VALUE;
    } else if (valueFromApp > MAX_APP_VALUE) {
      valueFromApp = MAX_APP_VALUE;
    }

    long servoSpeed = servoStill + valueFromApp;
        
    
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
