// 2DOF Magnetometer simulator with I2C
// P Bridger
// Controller master device
#include <Wire.h>
long X_mag = 0;
long Y_mag = 0;
void setup()
{
 // Open communication channels
 Serial.begin(9600);
 Wire.begin();
}
void loop()
{
 // Simply program to read value and display
 readmagnetometer();
 Serial.print(X_mag);
 Serial.print(" , ");
 Serial.println(Y_mag);
 delay(200);
}
void readmagnetometer()
{
 long magX_B1;
 long magX_B2;
 long magY_B1;
 long magY_B2;

 // Ask for 4 bytes of data from the magnetometer
 Wire.requestFrom(30,4);
 while(Wire.available())
 {
 magX_B1 = Wire.read();
 magX_B2 = Wire.read();
 magY_B1 = Wire.read();
 magY_B2 = Wire.read();
 }

 // Convert bytes to numerical values
 X_mag = magX_B1*256 + magX_B2 - 32768;
 Y_mag = magY_B1*256 + magY_B2 - 32768;
}