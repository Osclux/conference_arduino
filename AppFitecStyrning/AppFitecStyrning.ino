
#include <Servo.h>

boolean bConnected = false;  // Flag for determining if we have an existing connection or a new connection when we start our loop()

Servo servo;  // create servo object to control a servo 

const long servoStill = 1500;
const long servoFullCounterClockWise = 2100;
const long servoFullClockWise = 900;
const int SERVO_PIN = 2;
const long MIN_APP_VALUE = -100;
const long MAX_APP_VALUE = 100;

int oldXValue = -1;

void setup() 
{ 
  // Serial timeout needs to be very short to prevent lag
  Serial.setTimeout(5);
  // Initialize serial communications at 57600bps
  Serial.begin(57600);

  // Enable the wake on connect so the Arduino will wake upon BLE connection rather than waiting for serial input
  Bean.enableWakeOnConnect( true );

  
} 

 void attachServo() {
    // Attach servo pin if not attached
    if (!servo.attached()) {
       servo.attach(SERVO_PIN);
    }
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
      // If this is our first time through loop since connecting 
      // start by sending the stand still value for 10 seconds to be able to calibrate the servo
      attachServo();
      servo.writeMicroseconds(servoStill);
      delay(10000);
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
      Bean.sleep(10);  
    } else {
      //Serial.print("ValueFromApp=");
      //Serial.print(valueFromApp);
      //Serial.print("Servospeed=");
      //Serial.print(servoSpeed);
      //Serial.flush();
      // Light up the led to indicate operation
      if (valueFromApp > 0) {
        Bean.setLed(valueFromApp, 0, 0);
      } else {
        Bean.setLed(0, 0, valueFromApp);
      }
      attachServo();
      // Write the servo speed
      servo.writeMicroseconds(servoSpeed);
      // Reset the scratch data
      Bean.setScratchNumber(1, 0);
      // Delay execution a bit to make the servo operate smoother
      delay(50);  
    }
    
  }

}
