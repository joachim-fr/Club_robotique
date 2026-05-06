#include <Wire.h>
#include <QTRSensors.h>

/////////////////////////////////////////////////////////////////////
//    Variables globales et fonctions pour le suivi de ligne       //
/////////////////////////////////////////////////////////////////////

// Définition des broches pour les moteurs
#define borneENA        10      // Broche ENA du L298N connectée à la broche D10 de l'Arduino (PWM possible)
#define borneIN1        9       // Broche IN1 du L298N connectée à la broche D9 de l'Arduino
#define borneIN2        8       // Broche IN2 du L298N connectée à la broche D8 de l'Arduino
#define borneIN3        7       // Broche IN3 du L298N connectée à la broche D7 de l'Arduino
#define borneIN4        6       // Broche IN4 du L298N connectée à la broche D6 de l'Arduino
#define borneENB        5       // Broche ENB du L298N connectée à la broche D5 de l'Arduino (PWM possible)

// Définition des constantes du programme
#define delaiChangementVitesse     00      // Délai en millisecondes avant de changer la vitesse
#define vitesseMinimale            120     // Rapport cyclique PWM minimal pour le moteur
#define vitesseMaximale            255     // Rapport cyclique PWM maximal pour le moteur
#define vitesseRotation            255     // Rapport cyclique PWM maximal pour la rotation

// Définition des broches pour les capteurs
const int capteurG1 = 24; // Capteur gauche 1
const int capteurG2 = 25; // Capteur gauche 2
const int capteurG3 = 22; // Capteur gauche 3
const int capteurG4 = 23; // Capteur gauche 4
const int capteurD1 = 53; // Capteur droit 1
const int capteurD2 = 52; // Capteur droit 2
const int capteurD3 = 50; // Capteur droit 3
const int capteurD4 = 51; // Capteur droit 4

int bcapteurG1 = 0;
int bcapteurG2 = 0;
int bcapteurG3 = 0;
int bcapteurG4 = 0;

int bcapteurD1 = 0;
int bcapteurD2 = 0;
int bcapteurD3 = 0;
int bcapteurD4 = 0;

// Définition des broches pour les moteurs (GA et GB pour le moteur Gauche, DA et DB pour le moteur de droite)
const int moteurGA = 12;
const int moteurGB = 3;
const int moteurDA = 13;
const int moteurDB = 11;

// Définition des capteurs de suivit de ligne en suivant le code de la documentation
const uint8_t SensorCount = 4;

QTRSensors qtrd;
uint16_t sensorValuesD[SensorCount];

QTRSensors qtrg;
uint16_t sensorValuesG[SensorCount];

// Définition des commandes de mouvement
const char MARCHE_AVANT   = 'A';
const char MARCHE_ARRIERE = 'R';
const char STOP           = 'S'; 

int vitesseActuelleGauche = 0; 
int vitesseActuelleDroite = 0;

// Déclaration des fonctions
void configurerSensDeRotationPontA(char sensDeRotation);
void changeVitesseMoteurPontA(int vitesse);
void configurerSensDeRotationPontB(char sensDeRotation);
void changeVitesseMoteurPontB(int vitesse);
void avancer(float facteur);
void reculer(float facteur);
void tournerGauche(float facteur);
void tournerDroite(float facteur);
void tournerGaucheExtremite(float facteur);
void tournerDroiteExtremite(float facteur);
void arreter(); 


/////////////////////////////////////////////////////////////////////
//                      Programme principal                        //
/////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);

  //Configuration des capteurs

  qtrd.setTypeRC();
  qtrd.setSensorPins((const uint8_t[]){capteurD1, capteurD2, capteurD3, capteurD4}, SensorCount);
  qtrd.setEmitterPin(31);
  
  qtrg.setTypeRC();
  qtrg.setSensorPins((const uint8_t[]){capteurG1, capteurG2, capteurG3, capteurG4}, SensorCount);
  qtrg.setEmitterPin(31);


  // Configuration des broches des capteurs en entrée
  pinMode(capteurG1, INPUT);
  pinMode(capteurG2, INPUT);
  pinMode(capteurG3, INPUT);
  pinMode(capteurG4, INPUT);
  pinMode(capteurD1, INPUT);
  pinMode(capteurD2, INPUT);
  pinMode(capteurD3, INPUT);
  pinMode(capteurD4, INPUT);

  // Configuration des broches des moteurs en sortie
  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  pinMode(borneIN3, OUTPUT);
  pinMode(borneIN4, OUTPUT);
  pinMode(borneENB, OUTPUT);


}

void loop() {
 
}

/////////////////////////////////////////////////////////////////////
//                   Fonctions pour les moteurs                    //
/////////////////////////////////////////////////////////////////////

void bettervar() {
  // Lire les valeurs des capteurs gauche et droit
  qtrg.read(sensorValuesG);
  qtrd.read(sensorValuesD);

  // Mettre à jour les variables binaires en fonction des seuils
  bcapteurG1 = (sensorValuesG[0] > 100) ? 1 : 0;
  bcapteurG2 = (sensorValuesG[1] > 100) ? 1 : 0;
  bcapteurG3 = (sensorValuesG[2] > 100) ? 1 : 0;
  bcapteurG4 = (sensorValuesG[3] > 100) ? 1 : 0;

  bcapteurD1 = (sensorValuesD[0] > 100) ? 1 : 0;
  bcapteurD2 = (sensorValuesD[1] > 100) ? 1 : 0;
  bcapteurD3 = (sensorValuesD[2] > 100) ? 1 : 0;
  bcapteurD4 = (sensorValuesD[3] > 100) ? 1 : 0;
}

// Fonction pour arrêter les moteurs
void arreter() {
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, LOW);
  digitalWrite(borneIN3, LOW);
  digitalWrite(borneIN4, LOW);
  analogWrite(borneENA, 0);
  analogWrite(borneENB, 0);
}

void arriere(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseMaximale*facteur);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseMaximale*facteur);
}

void avancer(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseMaximale*facteur);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseMaximale*facteur);
}

void tournergauche(float facteur) {  
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseRotation*facteur);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseRotation*facteur/2);
  
}
void tournergaucheextremite(float facteur) {  
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseRotation * facteur);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseRotation * facteur * 0.7);
}

void tournerdroite(float facteur) {  
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseRotation*facteur/2);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseRotation*facteur);
  
}

void tournerdroiteextremite(float facteur) {  
  facteur = facteur / 100;

  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseRotation * facteur * 0.7);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseRotation * facteur);
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
