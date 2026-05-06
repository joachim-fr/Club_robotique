// Moteur 1
int IN1 = 5;
int IN2 = 6;

// Moteur 2
int IN3 = 10;
int IN4 = 11;

void setup() {

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

}

void loop() {

  avant();// Marche Avant

  delay(2000);

}



void avant () {

  analogWrite(IN1, 100);
  digitalWrite(IN2, 0);
  analogWrite(IN3, 100);
  digitalWrite(IN4, 0);

}

void arriere () {

  digitalWrite(IN1, 0);
  analogWrite(IN2, 100);
  digitalWrite(IN3, 0);
  analogWrite(IN4, 100);
  
}

void droite () {

  analogWrite(IN1, 100);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  analogWrite(IN4, 100);

}

void gauche () {

  digitalWrite(IN1, 0);
  analogWrite(IN2, 100);
  analogWrite(IN3, 100);
  digitalWrite(IN4, 0);

}

