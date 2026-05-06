#include <Wire.h>

#define borneENA        10      // On associe la borne "ENA" du L298N à la pin D10 de l'Arduino (PWM possible sur cette broche)
#define borneIN1        9       // On associe la borne "IN1" du L298N à la pin D9 de l'Arduino
#define borneIN2        8       // On associe la borne "IN2" du L298N à la pin D8 de l'Arduino
#define borneIN3        7       // On associe la borne "IN3" du L298N à la pin D7 de l'Arduino
#define borneIN4        6       // On associe la borne "IN4" du L298N à la pin D6 de l'Arduino
#define borneENB        5       // On associe la borne "ENB" du L298N à la pin D5 de l'Arduino (PWM possible sur cette broche)

//*************************//
// Constantes du programme //
//*************************//
#define delaiChangementVitesse     20      // Défini un petit délai, exprimé en millisecondes, avant tout autre changement de vitesse
#define vitesseMinimale            120       // Rapport cylique minimal du signal PWM, pour faire tourner le moteur au minimum de sa vitesse (en pratique, on évitera de trop approcher la valeur  0)
#define vitesseMaximale            255      // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse
#define vitesseRotation            255      // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse (utilisé lors de la rotation du robot avec une seule chenille)

const int IN_A0 = A0; // analog input
const int IN_D0 = 11; // digital input

const char MARCHE_AVANT   = 'V';            // Défini une constante pour la "marche avant" (peu importe la valeur)
const char MARCHE_ARRIERE = 'R';            // Défini une constante pour la "marche arrière" (peu importe la valeur)

int pas = 1;
int vitesseActuelleA = 0;
int vitesseActuelleB = 0;

int capteurG1 = 23; //Pin pour Le Capteur Gauche (g1, g4 pas pwm, à tester)
int capteurG2 = 22;
int capteurG3 = 25;
int capteurG4 = 24;

int capteurD1 = 51; //Pin pour Le Capteur Droit
int capteurD2 = 50;
int capteurD3 = 53;
int capteurD4 = 52;

int moteurGA = 12, moteurGB = 3, moteurDA = 13, moteurDB = 11; //Pin pour Deux moteurs (GA et GB pour le moteur Gauche, DA et DB pour le moteur de droite)

void setup() {

  Serial.begin(9600);
  pinMode(capteurG1, INPUT);
  pinMode(capteurG2, INPUT);
  pinMode(capteurG3, INPUT);
  pinMode(capteurG4, INPUT);
  pinMode(capteurD1, INPUT);
  pinMode(capteurD2, INPUT);
  pinMode(capteurD3, INPUT);
  pinMode(capteurD4, INPUT);
  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  pinMode(borneIN3, OUTPUT);
  pinMode(borneIN4, OUTPUT);
  pinMode(borneENB, OUTPUT);
}

void loop() {

  int C1 = digitalRead(capteurG1);
  int C2 = digitalRead(capteurG2);
  int C3 = digitalRead(capteurG3);
  int C4 = digitalRead(capteurG4);
  int C5 = digitalRead(capteurD1);
  int C6 = digitalRead(capteurD2);
  int C7 = digitalRead(capteurD3);
  int C8 = digitalRead(capteurD4);

  if ((C1 || C2) && !C8) {
    Serial.println("Tourner à gauche");
    tournergauche();
  } else if (!C1 && (C7 || C8)) {
    Serial.println("Tourner à droite");
    tournerdroite();
  } else if (C3 && C6) {
    Serial.println("Continuer tout droit");
    avant();
  } else {
    Serial.println("Arrêt");
    mouvementSTOP();
  }
}

void mouvementSTOP() {
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, LOW);
  digitalWrite(borneIN3, LOW);
  digitalWrite(borneIN4, LOW);
  analogWrite(borneENA, 0);
  analogWrite(borneENB, 0);
}

void arriere() {
  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseMaximale);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseMaximale);
}

void avant() {
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseMaximale);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseMaximale);
}

void tournergauche() {
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseRotation);
  digitalWrite(borneIN3, LOW);
  digitalWrite(borneIN4, LOW);
  analogWrite(borneENB, 0);
}

void tournerdroite() {
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, LOW);
  analogWrite(borneENA, 0);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseRotation);
}

void configurerSensDeRotationPontA(char sensDeRotation) {
  if (sensDeRotation == MARCHE_AVANT) {
    digitalWrite(borneIN1, HIGH);
    digitalWrite(borneIN2, LOW);
  } else {
    digitalWrite(borneIN1, LOW);
    digitalWrite(borneIN2, HIGH);
  }
}

void changeVitesseMoteurPontA(int vitesse) {
  analogWrite(borneENA, vitesse);
}

void configurerSensDeRotationPontB(char sensDeRotation) {
  if (sensDeRotation == MARCHE_AVANT) {
    digitalWrite(borneIN3, HIGH);
    digitalWrite(borneIN4, LOW);
  } else {
    digitalWrite(borneIN3, LOW);
    digitalWrite(borneIN4, HIGH);
  }
}

void changeVitesseMoteurPontB(int vitesse) {
  analogWrite(borneENB, vitesse);
}
