#include <Servo.h>
Servo myservo;
int pos = 0;
Servo myservo2;
int pos2 = 0;

int trigD = 2;
int echoD= 3;
//pin du capteur ultrason de droite
long lecture_echoD;
long cmD;

int trigG = 4;
int echoG = 6;
//pin du capteur ultrason de gauche
long lecture_echoG;
long cmG;

void setup() {

  myservo.attach(5);
  myservo2.attach(9);

  Serial.begin(9600);

  pinMode(trigD, OUTPUT);   // Droite
  digitalWrite(trigD, LOW);
  pinMode(echoD, INPUT);

  pinMode(trigG, OUTPUT);   // Gauche
  digitalWrite(trigG, LOW);
  pinMode(echoG, INPUT);

}

void loop() {

  digitalWrite(trigD, HIGH);            // Droite
  delayMicroseconds(10);
  digitalWrite(trigD, LOW);
  lecture_echoD = pulseIn(echoD,HIGH);
  cmD = lecture_echoD /58;
  Serial.print("Distance en cm droite :");
  Serial.println(cmD);
  
  digitalWrite(trigG, HIGH);            // Gauche
  delayMicroseconds(10);
  digitalWrite(trigG, LOW);
  lecture_echoG = pulseIn(echoG,HIGH);
  cmG = lecture_echoG /58;
  Serial.print("Distance en cm gauche :");
  Serial.println(cmG);
  
  myservo.write(0);
  myservo2.write(0);
  if ( cmD < 5 && cmG < 5 ){action1();}
  delay(100);

}

void action1() {

  myservo.write(70); 
  myservo2.write(70);

}