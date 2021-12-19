// 2DOF Magnetometer simulator with I2C for Assignment
// P Bridger
// Magnetometer slave device
#include <Wire.h>
#define PI 3.14159
#define potPin A0
// Constants for magnetometer - set as required
// Transmission limited from -32768 to +32767 (2 bytes)
#define X_OFFSET 300
#define Y_OFFSET 200
#define X_SENS 1000
#define Y_SENS 1000
#define NOISE 0
// Other variables
int potentiometer = 0;
int heading = 0;
int dirr = 0;
void setup()
{
 Serial.begin(9600);
 Wire.begin(30); // set magnetometer address as 30
 Wire.onRequest(transmitvalues); // function to read and send values
}
void loop()
{
 delay(100);
}
void transmitvalues()
{
 // To replicate the compass, take the potentiometer
 // reading and convert to a heading
 // 0 deg -> 360 deg over pot rotation
 int potentiometer = analogRead(A0);
 int heading = potentiometer*360.0/1023.0;

 // Trigonometry used to calculate equivalent magnetometer values for
 // given heading, values scaled and offset at user's discretion
 long magX = sin(heading*PI/180)*X_SENS + X_OFFSET + random(-NOISE,NOISE);
 long magY = cos(heading*PI/180)*Y_SENS + Y_OFFSET + random(-NOISE,NOISE);
 // Ensure values are positive
 magX += 32768;
 magY += 32768;

 // Convert each number to 2 bytes
 int magX_B1 = magX/256;
 int magX_B2 = magX%256;
 int magY_B1 = magY/256;
 int magY_B2 = magY%256;

 // Transmit values
 Wire.write(magX_B1);
 Wire.write(magX_B2);
 Wire.write(magY_B1);
 Wire.write(magY_B2);
 heading = 180 - heading;
 Serial.print(heading);
 Serial.println("");
}