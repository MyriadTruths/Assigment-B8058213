// Assignment
// Paul Bridger B8058213

#define LmotorPinA 11
#define LmotorPinB 6
#define RmotorPinA 3
#define RmotorPinB 2
#define inputPin A0
#define LED 5
#define LtrigPin 10
#define LechoPin 7
#define FtrigPin 13
#define FechoPin 12
#define RtrigPin 9
#define RechoPin 8
#define BUTTON 4
#define PI 3.14159
#define potPin A1
#define X_OFFSET 300
#define Y_OFFSET 200
#define X_SENS 1000
#define Y_SENS 1000

int reading = 0;
int upperThreshold = 120;
int lowerThreshold = 50;
int reqSpeed = 100;
int lightlevel = 100;
int outValue1 = 0;
int outValue2 = 0;
int outValue3 = 0;
int outValue4 = 0;
int outValue5 = 0;
// Variables for magnetometer
long magX = 0;
long magY = 0;
int potentiometer = 0;
int heading = 0;
// Basic interrupt program for collision detector
volatile bool buttonFlag = 0;
// Variables for Ultrasonic
long duration,distance,RightSensor,FrontSensor,LeftSensor,Lduration,Fduration,Rduration;

void setup(){
 // Open COMMS channel
 Serial.begin(9600);
 Serial.println();
 Serial.println("Starting squirrel monitoring");

 // Set digital pins for LED control
  pinMode(inputPin, INPUT);
  pinMode(LED, OUTPUT);
 // Read light sensor
 lightlevel = analogRead(inputPin);

 // Switch on lights if required
volatile bool lightFlag = (lightlevel<100);

 // Set digital pins for ultrasonic
 pinMode(LtrigPin, OUTPUT);
 pinMode(LechoPin, INPUT);
 pinMode(FtrigPin, OUTPUT);
 pinMode(FechoPin, INPUT);
 pinMode(RtrigPin, OUTPUT);
 pinMode(RechoPin, INPUT);

 // Set digital pins used for motor control
 pinMode(LmotorPinA, OUTPUT);
 pinMode(LmotorPinB, OUTPUT);
 pinMode(RmotorPinA, OUTPUT);
 pinMode(RmotorPinB, OUTPUT);
 
 // Set digital pins for collision detector
 pinMode(BUTTON, INPUT);
 attachInterrupt(digitalPinToInterrupt(BUTTON), whenpressed, RISING); // setup the interrupt
}
void loop(){
 byte outvalue1;
 byte outvalue2;
 byte outvalue3;
 byte outvalue4;
 byte outvalue5;
  
setheading();
calcvalues();
printvalues();
delay(100);
  
// Squirrel on acorn 
reading = analogRead(inputPin);
  Serial.print("light level ");
  Serial.println(reading);
  if (reading < lowerThreshold) {
    digitalWrite(LED, HIGH);
  }
  else if (analogRead(inputPin) > upperThreshold) {
    digitalWrite(LED,LOW);
  }
delay(500);
  
  // 「Read ultrasonic sensor」 \\
  
SonarSensor(LtrigPin, LechoPin);
LeftSensor = distance;
Lduration = LeftSensor*58.2;
SonarSensor(FtrigPin, FechoPin);
FrontSensor = distance;
Fduration = FrontSensor*58.2;
SonarSensor(RtrigPin, RechoPin);
RightSensor = distance;
Rduration = RightSensor*58.2;

Serial.print(LeftSensor);
Serial.print(" - ");
Serial.print(FrontSensor);
Serial.print(" - ");
Serial.println(RightSensor);
delay(500);

 // Read in data
 while (Serial.available() > 0){
 int input = Serial.read();
 reqSpeed = map(input,65,90,0,255);
 }
  
  // 「Read ultrasonic sensor」 \\

 // Determine required speed
  if(LeftSensor<2 || FrontSensor<2 || RightSensor<2 || buttonFlag==1){
 reqSpeed = 0; 
  }
   
 Serial.print("Req speed: ");
 Serial.println(reqSpeed);
      
 if(buttonFlag){
 
 delay(100);
 buttonFlag = 0;
 }	// Reset flag
  
 // 「Set motor」 \\ 
  
 if(reqSpeed>0){
 // Drive both motors @ positive rpm
 Serial.print("Driving South at: ");
 Serial.println(reqSpeed);
 analogWrite(RmotorPinA, reqSpeed);
 digitalWrite(RmotorPinB, LOW);
 analogWrite(LmotorPinA, reqSpeed);
 digitalWrite(LmotorPinB, LOW);
 }
 else if(reqSpeed<0){
 //Drive both motors @ negative rpm
 Serial.print("Driving North at: ");
 Serial.println(-reqSpeed);
 analogWrite(LmotorPinB, -reqSpeed);
 digitalWrite(LmotorPinA, LOW);
 analogWrite(RmotorPinA, -reqSpeed);
 digitalWrite(RmotorPinB, LOW);
 }
 else{
 // If zero then brake using:
 Serial.print("Braking for: ");
 Serial.println(reqSpeed);
 digitalWrite(LmotorPinA, HIGH);
 digitalWrite(LmotorPinB, HIGH);
 digitalWrite(RmotorPinA, HIGH);
 digitalWrite(RmotorPinB, HIGH);
 }
  
 Serial.write(outValue1);
 Serial.write(outValue2);
 Serial.write(outValue3);
 Serial.println();

 delay(100);    
    
// Output data
outValue1 = map(Lduration,0,19280,65,90);
outValue2 = map(Fduration,0,19280,65,90);
outValue3 = map(Rduration,0,19280,65,90);
outValue4 = map(lightlevel,0,1023,65,90);
    if(lightlevel<100){
outValue5 = 66;
    }
    else{
outValue5 = 65;
Serial.write(outValue1);
Serial.write(outValue2);
Serial.write(outValue3);
Serial.write(outValue4);
Serial.write(outValue5);
Serial.println();
    }
}


// Sensor distance calc loop
void SonarSensor(int trigPin,int echoPin){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
}
  
// 「Magnetometer」 \\

void setheading(){
 // To replicate the compass, take the potentiometer reading and convert to a heading
 // 0 deg -> 360 deg over pot rotation
 potentiometer = analogRead(A1);
 heading = potentiometer*360.0/1023.0;
}
void calcvalues(){
 // Trigonometry used to calculate equivalent magnetometer values for
 // given heading, values scaled and offset at user's discretion
 magX = cos(heading*PI/180)*X_SENS + X_OFFSET;
 magY = sin(heading*PI/180)*Y_SENS + Y_OFFSET;
}
void printvalues(){
 Serial.print(magX);
 Serial.print(" , ");
 Serial.println(magY);
}
// setting buttonFlag to 1 when the button is pressed
void whenpressed(){
 // Pin has gone high so set flag
 buttonFlag = 1;
}