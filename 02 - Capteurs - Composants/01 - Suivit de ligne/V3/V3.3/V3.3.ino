#include <Wire.h>

// Définition des broches pour les moteurs
#define borneENA        10      // Broche ENA du L298N connectée à la broche D10 de l'Arduino (PWM possible)
#define borneIN1        9       // Broche IN1 du L298N connectée à la broche D9 de l'Arduino
#define borneIN2        8       // Broche IN2 du L298N connectée à la broche D8 de l'Arduino
#define borneIN3        7       // Broche IN3 du L298N connectée à la broche D7 de l'Arduino
#define borneIN4        6       // Broche IN4 du L298N connectée à la broche D6 de l'Arduino
#define borneENB        5       // Broche ENB du L298N connectée à la broche D5 de l'Arduino (PWM possible)

// Définition des constantes du programme
#define delaiChangementVitesse     20      // Délai en millisecondes avant de changer la vitesse
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

// Définition des broches pour les moteurs (GA et GB pour le moteur Gauche, DA et DB pour le moteur de droite)
const int moteurGA = 12;
const int moteurGB = 3;
const int moteurDA = 13;
const int moteurDB = 11;

// Définition des commandes de mouvement
const char MARCHE_AVANT   = 'A';
const char MARCHE_ARRIERE = 'R';
const char STOP           = 'S'; // Ajout d'une commande STOP

int vitesseActuelleGauche = 0; // Utiliser des noms de variables plus descriptifs
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
void arreter(); // Ajout d'une fonction arreter

void setup() {
  Serial.begin(9600);

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
  // Lecture de l'état des capteurs
  int G4 = digitalRead(capteurG1);
  int G3 = digitalRead(capteurG2);
  int G2 = digitalRead(capteurG3);
  int G1 = digitalRead(capteurG4);
  int D4 = digitalRead(capteurD1);
  int D3 = digitalRead(capteurD2);
  int D2 = digitalRead(capteurD3);
  int D1 = digitalRead(capteurD4);

  // Affichage des valeurs des capteurs pour le débogage
  Serial.print("Capteurs: G4="); Serial.print(G4);
  Serial.print(" G3="); Serial.print(G3);
  Serial.print(" G2="); Serial.print(G2);
  Serial.print(" G1="); Serial.print(G1);
  Serial.print(" D4="); Serial.print(D4);
  Serial.print(" D3="); Serial.print(D3);
  Serial.print(" D2="); Serial.print(D2);
  Serial.print(" D1="); Serial.print(D1);
  Serial.println();

  // Logique de mouvement basée sur les capteurs
  if (D1 && D2 && D3 && D4 && G1 && G2 && G3 && G4) {
    Serial.println("STOP");
    arreter();
  } else if (D4) {
    Serial.println("Tourner à droite 100%");
    tournerDroiteExtremite(100);
  } else if (G4) {
    Serial.println("Tourner à gauche 100%");
    tournerGaucheExtremite(100);
  } else if (G3) {
    Serial.println("Tourner à droite 90%");
    tournerDroite(95);
  } else if (D3) {
    Serial.println("Tourner à gauche 90%");
    tournerGauche(95);
  } else if (G2) {
    Serial.println("Tourner à gauche 80%");
    tournerGauche(95);
  } else if (D2) {
    Serial.println("Tourner à droite 80%");
    tournerDroite(95);
  } else if (G1) {
    Serial.println("Tourner à droite 70%");
    tournerGauche(95);
  } else if (D1) {
    Serial.println("Tourner à gauche 70%");
    tournerDroite(95);
  } else if (G1 && D1) {
    Serial.println("Avancer");
    avancer(90);
  } else {
    Serial.println("Avancer");
    avancer(90);
  }
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

// Fonction pour avancer
void avancer(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseMaximale * facteur);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseMaximale * facteur);
}

// Fonction pour reculer
void reculer(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseMaximale * facteur);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseMaximale * facteur);
}

// Fonction pour tourner à gauche
void tournerGauche(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseRotation * facteur);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseRotation * facteur);
}

// Fonction pour tourner à gauche à l'extrême
void tournerGaucheExtremite(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_ARRIERE);
  changeVitesseMoteurPontA(vitesseRotation * facteur);
  configurerSensDeRotationPontB(MARCHE_AVANT);
  changeVitesseMoteurPontB(vitesseRotation * facteur);
}

// Fonction pour tourner à droite
void tournerDroite(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseRotation * facteur / 2); // Réduction de la vitesse du moteur A
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseRotation * facteur);
}

// Fonction pour tourner à droite à l'extrême
void tournerDroiteExtremite(float facteur) {
  facteur = facteur / 100;
  configurerSensDeRotationPontA(MARCHE_AVANT);
  changeVitesseMoteurPontA(vitesseRotation * facteur);
  configurerSensDeRotationPontB(MARCHE_ARRIERE);
  changeVitesseMoteurPontB(vitesseRotation * facteur);
}

// Fonction pour configurer le sens de rotation du pont A
void configurerSensDeRotationPontA(char sensDeRotation) {
  if (sensDeRotation == MARCHE_AVANT) {
    digitalWrite(borneIN1, HIGH);
    digitalWrite(borneIN2, LOW);
  } else if (sensDeRotation == MARCHE_ARRIERE) {
    digitalWrite(borneIN1, LOW);
    digitalWrite(borneIN2, HIGH);
  }
  else {
    digitalWrite(borneIN1, LOW);
    digitalWrite(borneIN2, LOW);
  }
}

// Fonction pour changer la vitesse du moteur du pont A
void changeVitesseMoteurPontA(int vitesse) {
  if (vitesse != vitesseActuelleGauche) {
    delay(delaiChangementVitesse); // Délai avant de changer la vitesse
    vitesseActuelleGauche = vitesse;
    analogWrite(borneENA, vitesse);
  }
}

// Fonction pour configurer le sens de rotation du pont B
void configurerSensDeRotationPontB(char sensDeRotation) {
  if (sensDeRotation == MARCHE_AVANT) {
    digitalWrite(borneIN3, HIGH);
    digitalWrite(borneIN4, LOW);
  } else if (sensDeRotation == MARCHE_ARRIERE) {
    digitalWrite(borneIN3, LOW);
    digitalWrite(borneIN4, HIGH);
  }
  else {
    digitalWrite(borneIN3, LOW);
    digitalWrite(borneIN4, LOW);
  }
}

// Fonction pour changer la vitesse du moteur du pont B
void changeVitesseMoteurPontB(int vitesse) {
  if (vitesse != vitesseActuelleDroite) {
    delay(delaiChangementVitesse); // Délai avant de changer la vitesse
    vitesseActuelleDroite = vitesse;
    analogWrite(borneENB, vitesse);
  }
}
