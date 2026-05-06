//MOTEUR
#include <Wire.h>

#define borneENA        10      // On associe la borne "ENA" du L298N à la pin D10 de l'Arduino (PWM possible sur cette broche)
#define borneIN1        22       // On associe la borne "IN1" du L298N à la pin D9 de l'Arduino
#define borneIN2        23       // On associe la borne "IN2" du L298N à la pin D8 de l'Arduino
#define borneIN3        24       // On associe la borne "IN3" du L298N à la pin D7 de l'Arduino
#define borneIN4        25       // On associe la borne "IN4" du L298N à la pin D6 de l'Arduino
#define borneENB        5       // On associe la borne "ENB" du L298N à la pin D5 de l'Arduino (PWM possible sur cette broche)

// Capteur ultra son
int trig = 52;
int echo = 53;
long lecture_echo;
long cm;

//*************************//
// Constantes du programme //
//*************************//

#define delaiChangementVitesse     20       // Défini un petit délai, exprimé en millisecondes, avant tout autre changement de vitesse
#define vitesseMinimale            60       // Rapport cylique minimal du signal PWM, pour faire tourner le moteur au minimum de sa vitesse (en pratique, on évitera de trop approcher la valeur  0)
#define vitesseMaximale            70        // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse

//MOTEUR

const char MARCHE_AVANT   = 'V';            // Défini une constante pour la "marche avant" (peu importe la valeur)
const char MARCHE_ARRIERE = 'R';            // Défini une constante pour la "marche arrière" (peu importe la valeur)

//SUIVI DE LIGNE

const int IN_A0 = A0; // Entrée analogue A0
const int IN_A1 = A1; // Entrée analogue A1



int vitesse; // Spécifie la vitesse de rotation du moteur, entre son minimum (0) et son maximum (255) <= signal PWM



void setup() {
  
//MOTEUR

  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  pinMode(borneIN3, OUTPUT);
  pinMode(borneIN4, OUTPUT);
  pinMode(borneENB, OUTPUT);

  //SUIVI DE LIGNE

  Serial.begin(9600);
  pinMode (IN_A0, INPUT);
  pinMode (IN_A1, INPUT);

  //CAPTEUR ULTRASON
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
}

//differente loop
void avant() {
  
// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_AVANT);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(vitesseMaximale*1.3);
// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_AVANT);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(vitesseMaximale);
}


void tournerdroite() {

// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_ARRIERE);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(0);
// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_ARRIERE);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(vitesseMaximale*2.5);
}


void tournergauche() {
// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_ARRIERE); 
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(vitesseMaximale*2.5);
// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_ARRIERE);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(0);

}

void arretermouvement() {

  
  // On met la vitesse de deux moteur a 0 
  changeVitesseMoteurPontA(0);
  changeVitesseMoteurPontB(0);
}

void configurerSensDeRotationPontA(char sensDeRotation) {

  if(sensDeRotation == MARCHE_ARRIERE) {
    // Configuration du L298N en "marche avant", pour le moteur connecté au pont A. Selon sa table de vérité, il faut que :
    digitalWrite(borneIN1, HIGH);                 // L'entrée IN1 doit être au niveau haut
    digitalWrite(borneIN2, LOW);                  // L'entrée IN2 doit être au niveau bas    
  }
  if(sensDeRotation == MARCHE_AVANT) {
    // Configuration du L298N en "marche arrière", pour le moteur câblé sur le pont A. Selon sa table de vérité, il faut que :
    digitalWrite(borneIN1, LOW);                  // L'entrée IN1 doit être au niveau bas
    digitalWrite(borneIN2, HIGH);                 // L'entrée IN2 doit être au niveau haut
  }
}


void changeVitesseMoteurPontA(int nouvelleVitesse) {
  // Génère un signal PWM permanent, de rapport cyclique égal à "nouvelleVitesse" (valeur comprise entre 0 et 255)
  analogWrite(borneENA, nouvelleVitesse);
}


void configurerSensDeRotationPontB(char sensDeRotation) {
  if(sensDeRotation == MARCHE_ARRIERE) {
    digitalWrite(borneIN3, HIGH);
    digitalWrite(borneIN4, LOW);                 
  }
  if(sensDeRotation == MARCHE_AVANT) {
    digitalWrite(borneIN3, LOW);          
    digitalWrite(borneIN4, HIGH);   
  }
}


void changeVitesseMoteurPontB(int nouvelleVitesse) {
  // Génère un signal PWM permanent, de rapport cyclique égal à "nouvelleVitesse" (valeur comprise entre 0 et 255)
  analogWrite(borneENB, nouvelleVitesse);
}

void loop() {
 
// Lire les valeurs des capteurs
  int value_A0 = analogRead(IN_A0);
  int value_A1 = analogRead(IN_A1);

  // Déterminer la direction du robot
  if (value_A0 < 3000 && value_A1 < 3000) {
    // Le robot est sur du blanc
    avant();
  } 
  else if (value_A0 > 3000 && value_A1<3000) {
    // Le robot est à droite de la ligne noire
    tournergauche();
  } 
  else if (value_A1 > 3000 && value_A0<3000) {
  //   Le robot est à gauche de la ligne noire
    tournerdroite(); 
  }  
  else if (value_A1 > 3000 && value_A0 > 3000) {
   //  Le robot est sur une intersection
    avant();
      
  }
}
