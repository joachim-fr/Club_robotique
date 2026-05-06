#include <Servo.h>
Servo myservo;
int pos = 0;

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

void setup(){
 myservo.attach(5);
  Serial.begin(9600);

pinMode(trigD, OUTPUT);
digitalWrite(trigD, LOW);
pinMode(echoD, INPUT);
Serial.begin(9600);

pinMode(trigG, OUTPUT);
digitalWrite(trigG, LOW);
pinMode(echoG, INPUT);
Serial.begin(9600);

}

void loop(){

  digitalWrite(trigD, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigD, LOW);
  lecture_echoD = pulseIn(echoD,HIGH);
  cmD = lecture_echoD /58;
  Serial.print("Distance en cm :");
  Serial.println(cmD);

  digitalWrite(trigG, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigG, LOW);
  lecture_echoG = pulseIn(echoG,HIGH);
  cmG = lecture_echoG /58;
  Serial.print("Distance en cm :");
  Serial.println(cmG);

  myservo.write(90);
  
  if ( cmD < 5 && cmG < 5 ){
      action1();
   }
  
   if ( cmD < 20 && cmG > 20 ){
    action2();
   }
   if ( cmD > 20 && cmG < 20 ){
    action3();
   }

  delay(100);
}

void action1() {
  myservo.write(60); 
   }

   
void action2() {
  //attraper balle de face 
  }

void action3 () {
  //attraper balle de face un peu à gauche 
  }