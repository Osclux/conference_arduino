// BeanBot
// by Ben Harraway - http://www.gourmetpixel.com
// A simple demonsration for the LightBlue Bean by Punch Through Design
// This sketch looks for input into the scratch and moves a servo based on the scratch value
// This example code is in the public domain.

#include <Servo.h> 
 
Servo leftServo;  // create servo object to control a servo 
Servo rightServo;  // create servo object to control a servo 

uint16_t servoStill = 90;
uint16_t parallaxServoStill = 1500;
uint16_t parallaxServoFullCounterClockWise = 1700;
uint16_t parallaxServoFullClockWise = 1300;
uint16_t parallaxServoFullStillDiff = 1700-1500;

uint16_t previousLeftServoSpeed = servoStill/2;  
uint16_t previousRightServoSpeed = servoStill;
uint8_t maxSpeedTests = 10;
uint8_t speedTests = 0;

void setup() 
{ 
  leftServo.attach(2);  // attaches the servo on pin 2 to the servo object 
  rightServo.attach(3);  // attaches the servo on pin 3 to the servo object 
} 
 
 
void loop() 
{   
    long leftServoSpeed = Bean.readScratchNumber(1);
    long rightServoSpeed = Bean.readScratchNumber(2);
    uint8_t ledIntensity = 120;

        // Get the current acceleration with range of ±2g, 
   // and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units. 
   AccelerationReading accel = Bean.getAcceleration();

   // Update LED color
   uint16_t r = (abs(accel.xAxis)) / 4;
   uint16_t g = (abs(accel.yAxis)) / 4;
   uint16_t b = (abs(accel.zAxis)) / 4;
//   Serial.println("r=" + String(r) + " g=" + String(g) + " b" + String(b));
//   Serial.println("x=" + String(accel.xAxis) + " y=" + String(accel.yAxis) + " z" + String(accel.zAxis));
   Bean.setLed((uint8_t)r,(uint8_t)g,(uint8_t)b);
   long parallaxSpeed = 1500;
   if (abs(accel.xAxis) > 20) {
      parallaxSpeed = 1500 + accel.xAxis; 
   } 
//    if (leftServoSpeed != previousLeftServoSpeed) {
//        long parallaxSpeed = 1300 + (parallaxServoFullStillDiff*(leftServoSpeed/(servoStill/2)));
//        Serial.println("servoStill=" + String(servoStill));
//        Serial.println("leftServoSpeed=" + String(leftServoSpeed));
//        Serial.println("parallaxServoFullStillDiff=" + String(parallaxServoFullStillDiff));
//        Serial.println("parallaxSpeed=" + String(parallaxSpeed));
        leftServo.writeMicroseconds(parallaxSpeed);
        delay(10);
//      }
//    if (leftServoSpeed != previousLeftServoSpeed) {
//      leftServo.write(leftServoSpeed);
//      Bean.setLed(ledIntensity, 0, 0);
//    }
//    if (rightServoSpeed != previousRightServoSpeed) {
//      rightServo.write(rightServoSpeed);    
//      Bean.setLed(0, 0, ledIntensity);
//    }
    previousLeftServoSpeed = leftServoSpeed;
    previousRightServoSpeed = rightServoSpeed;
 
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
