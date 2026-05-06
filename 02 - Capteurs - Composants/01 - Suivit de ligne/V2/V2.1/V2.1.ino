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
#define vitesseMinimale            120       // Rapport cylique minimal du signal PWM, pour faire tourner le moteur au minimum de sa vitesse (en pratique, on évitera de trop approcher la valeur  0)
#define vitesseMaximale            255      // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse
#define vitesseRotation            120      // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse (utilisé lors de la rotation du robot avec une seule chenille)
#define delaiChangementVitesse     0.1

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
int capteurG1_val = digitalRead(capteurG1);
int capteurG2_val = digitalRead(capteurG2);
int capteurG3_val = digitalRead(capteurG3);
int capteurG4_val = digitalRead(capteurG4);

int capteurD1_val = digitalRead(capteurD1);
int capteurD2_val = digitalRead(capteurD2);
int capteurD3_val = digitalRead(capteurD3);
int capteurD4_val = digitalRead(capteurD4);

void setup() {

  Serial.begin(9550);
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


  if (C1 && ~C8)
  {
    Serial.println("Tourner à gauche");
    tournergauche();
  }
  if (~C1 && C8)
  {
    Serial.println("Tourner à droite");
    tournerdroite();
  }
  if (~C1 && ~C8)
  {
    Serial.println("Continuer tout droit");
    avant();
  }
}

void arriere() {

  // On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
  configurerSensDeRotationPontA(MARCHE_AVANT);
  // On démarre le moteur raccordé au pont A à la vitesse maximale
  changeVitesseMoteurPontA(vitesseMaximale, vitesseActuelleA);

  // On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  // On démarre le moteur raccordé au pont B à la vitesse maximale
  changeVitesseMoteurPontB(vitesseMaximale, vitesseActuelleB);

}


void avant() {


  // On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  // On démarre le moteur raccordé au pont A à la vitesse maximale
  changeVitesseMoteurPontA(vitesseMaximale, vitesseActuelleA);

  // On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
  configurerSensDeRotationPontB(MARCHE_AVANT);
  // On démarre le moteur raccordé au pont B à la vitesse maximale
  changeVitesseMoteurPontB(vitesseMaximale, vitesseActuelleB);

}


void tournergauche() {

  // On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  // On démarre le moteur raccordé au pont A à la vitesse maximale
  changeVitesseMoteurPontA(vitesseRotation, vitesseActuelleA);

  // On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  // On démarre le moteur raccordé au pont B à la vitesse maximale
  changeVitesseMoteurPontB(vitesseRotation, vitesseActuelleB);

}


void tournerdroite() {

  // On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
  configurerSensDeRotationPontA(MARCHE_AVANT);
  // On démarre le moteur raccordé au pont A à la vitesse maximale
  changeVitesseMoteurPontA(vitesseRotation, vitesseActuelleA);

  // On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
  configurerSensDeRotationPontB(MARCHE_AVANT);
  // On démarre le moteur raccordé au pont B à la vitesse maximale
  changeVitesseMoteurPontB(vitesseRotation, vitesseActuelleB);

}


//************************************************************************************//
// Fonction : configurerSensDeRotationPontA()                                         //
// But :      Permet de définir le sens de rotation du moteu branché sur le pont A    //
//************************************************************************************//
void configurerSensDeRotationPontA(char sensDeRotation) {

  if (sensDeRotation == MARCHE_AVANT) {
    // Configuration du L298N en "marche avant", pour le moteur connecté au pont A. Selon sa table de vérité, il faut que :
    digitalWrite(borneIN1, HIGH);                 // L'entrée IN1 doit être au niveau haut
    digitalWrite(borneIN2, LOW);                  // L'entrée IN2 doit être au niveau bas
  }

  if (sensDeRotation == MARCHE_ARRIERE) {
    // Configuration du L298N en "marche arrière", pour le moteur câblé sur le pont A. Selon sa table de vérité, il faut que :
    digitalWrite(borneIN1, LOW);                  // L'entrée IN1 doit être au niveau bas
    digitalWrite(borneIN2, HIGH);                 // L'entrée IN2 doit être au niveau haut
  }
}


void changeVitesseMoteurPontA(int nouvelleVitesse, int ancienneVitesse) {

  // Génère un signal PWM permanent, de rapport cyclique égal à "nouvelleVitesse" (valeur comprise entre 0 et 255)
  
  
    if (nouvelleVitesse > ancienneVitesse) {
      for(int vitesse=ancienneVitesse ; vitesse<nouvelleVitesse ; vitesse++) {
        analogWrite(borneENA, vitesse);            // Met à jour la vitesse du moteur, en modifiant le rapport cyclique du signal PWM envoyé
        delay(delaiChangementVitesse);                // ... et on rajoute un petit délai pour que tout n'aille pas trop vite !
      }
    }
    if (nouvelleVitesse < ancienneVitesse) {
      for(int vitesse=ancienneVitesse ; vitesse>nouvelleVitesse ; vitesse--) {
        analogWrite(borneENA, vitesse);            // Met à jour la vitesse du moteur, en modifiant le rapport cyclique du signal PWM envoyé
        delay(delaiChangementVitesse);                // ... et on rajoute un petit délai pour que tout n'aille pas trop vite !
      }
    }
}


void configurerSensDeRotationPontB(char sensDeRotation) {

  if (sensDeRotation == MARCHE_AVANT) {

    digitalWrite(borneIN3, HIGH);
    digitalWrite(borneIN4, LOW);
  }

  if (sensDeRotation == MARCHE_ARRIERE) {

    digitalWrite(borneIN3, LOW);
    digitalWrite(borneIN4, HIGH);
  }
}


void changeVitesseMoteurPontB(int nouvelleVitesse, int ancienneVitesse) {

  // Génère un signal PWM permanent, de rapport cyclique égal à "nouvelleVitesse" (valeur comprise entre 0 et 255)
  
  
    if (nouvelleVitesse > ancienneVitesse) {
      for(int vitesse=ancienneVitesse ; vitesse<nouvelleVitesse ; vitesse++) {
        analogWrite(borneENB, vitesse);            // Met à jour la vitesse du moteur, en modifiant le rapport cyclique du signal PWM envoyé
        delay(delaiChangementVitesse);                // ... et on rajoute un petit délai pour que tout n'aille pas trop vite !
      }
    }
    if (nouvelleVitesse < ancienneVitesse) {
      for(int vitesse=ancienneVitesse ; vitesse>nouvelleVitesse ; vitesse--) {
        analogWrite(borneENB, vitesse);            // Met à jour la vitesse du moteur, en modifiant le rapport cyclique du signal PWM envoyé
        delay(delaiChangementVitesse);                // ... et on rajoute un petit délai pour que tout n'aille pas trop vite !
      }
    }
}
