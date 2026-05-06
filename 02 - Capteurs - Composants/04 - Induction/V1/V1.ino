#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

const int SensorPin = 4;
const int LedPin = 13;

void setup() {
  myservo.attach(3);
  Serial.begin(9600);
  Serial.println("Start!");
  pinMode(SensorPin,INPUT);
  pinMode(LedPin,OUTPUT);
  digitalWrite(LedPin,LOW);
    // attaches the servo on pin 9 to the servo object
}


void loop() {

if(digitalRead(SensorPin) == LOW)
myservo.write(70);     
else    myservo.write(0);  
Serial.print("Infrared Switch Status:");  
Serial.println(digitalRead(SensorPin),BIN);
  delay(500);
}