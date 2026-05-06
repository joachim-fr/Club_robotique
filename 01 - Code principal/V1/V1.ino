//MOTEUR
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

#define delaiChangementVitesse     20       // Défini un petit délai, exprimé en millisecondes, avant tout autre changement de vitesse
#define vitesseMinimale            60       // Rapport cylique minimal du signal PWM, pour faire tourner le moteur au minimum de sa vitesse (en pratique, on évitera de trop approcher la valeur  0)
#define vitesseMaximale            200       // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse

//MOTEUR

const char MARCHE_AVANT   = 'V';            // Défini une constante pour la "marche avant" (peu importe la valeur)
const char MARCHE_ARRIERE = 'R';            // Défini une constante pour la "marche arrière" (peu importe la valeur)

//SUIVIT DE LIGNE

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

  //SUIVIT DE LIGNE

  Serial.begin(9600);
  pinMode (IN_A0, INPUT);
  pinMode (IN_A1, INPUT);
}

//SUIVIT DE LIGNE

int value_A0;
int value_A1;

void loop() {

//SUIVIT DE LIGNE

//value_A0 = analogRead(IN_A0); 
//value_A1 = analogRead(IN_A1);
//Serial.print(" Analogue 0 = ");
//Serial.println(value_A0); 
//Serial.print(" Analogue 1 = "); 
//Serial.println(value_A1);
//delay(1000);

//MOTEUR

avant();
arriere();
tournerdroite();
tournergauche();
  
}

void arriere() {

// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_ARRIERE);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(vitesseMaximale);

// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_ARRIERE);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(vitesseMaximale);

// Attendre 3 secondes
delay(3000);

// Arrêter instantanément les moteurs
changeVitesseMoteurPontA(0);
changeVitesseMoteurPontB(0);
}


void avant() {
  
// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_AVANT);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(vitesseMaximale);
// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_AVANT);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(vitesseMaximale);
// Attendre 3 secondes
delay(3000);
// Arrêter instantanément les moteurs
changeVitesseMoteurPontA(0);
changeVitesseMoteurPontB(0);
}


void tournerdroite() {

// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_AVANT);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(10);
// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_AVANT);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(vitesseMaximale);
// Attendre 3 secondes
delay(3000);
// Arrêter instantanément les moteurs
changeVitesseMoteurPontA(0);
changeVitesseMoteurPontB(0);
}


void tournergauche() {

// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_AVANT);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(vitesseMaximale);
// On configure le sens de rotation du moteur branché sur le pont B en "marche avant"
configurerSensDeRotationPontB(MARCHE_AVANT);
// On démarre le moteur raccordé au pont B à la vitesse maximale
changeVitesseMoteurPontB(10);
// Attendre 3 secondes
delay(3000);
// Arrêter instantanément les moteurs
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
