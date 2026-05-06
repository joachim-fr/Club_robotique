// B=Droite A=Gauche

//MOTEUR
#include <Wire.h>
#include <TimerOne.h>

#define borneENA        10      // On associe la borne "ENA" du L298N à la pin D10 de l'Arduino (PWM possible sur cette broche)
#define borneIN1        22       // On associe la borne "IN1" du L298N à la pin D9 de l'Arduino
#define borneIN2        23       // On associe la borne "IN2" du L298N à la pin D8 de l'Arduino
#define borneIN3        24       // On associe la borne "IN3" du L298N à la pin D7 de l'Arduino
#define borneIN4        25       // On associe la borne "IN4" du L298N à la pin D6 de l'Arduino
#define borneENB        5       // On associe la borne "ENB" du L298N à la pin D5 de l'Arduino (PWM possible sur cette broche)

#define S0d 6
#define S1d 7
#define S2d 3
#define S3d 2
#define OUTd 0

int Rd = 0;
int Vd = 0;
int g_countd = 0;
int g_arrayd[3];
int g_flagd = 0;
float g_SFd[3];

#define S0g 12
#define S1g 4
#define S2g 8
#define S3g 9
#define OUTg 1

int Rg = 0;
int Vg = 0;
int g_countg = 0;
int g_arrayg[3];
int g_flagg = 0;
float g_SFg[3];
bool rougeDroit, vertDroit, rougeGauche, vertGauche;
 
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

//Capteur de couleur

void TSC_Initd()
{
  pinMode(S0d, OUTPUT);
  pinMode(S1d, OUTPUT);
  pinMode(S2d, OUTPUT);
  pinMode(S3d, OUTPUT);
  pinMode(OUTd, INPUT);
  digitalWrite(S0d, LOW);
  digitalWrite(S1d, HIGH);
}
void TSC_Initg()
{
  pinMode(S0g, OUTPUT);
  pinMode(S1g, OUTPUT);
  pinMode(S2g, OUTPUT);
  pinMode(S3g, OUTPUT);
  pinMode(OUTg, INPUT);
  digitalWrite(S0g, LOW);
  digitalWrite(S1g, HIGH);
}
void TSC_FilterColord(int Level01, int Level02)
{
  if (Level01 != 0)
    Level01 = HIGH;
  if (Level02 != 0)
    Level02 = HIGH;
  digitalWrite(S2d, Level01);
  digitalWrite(S3d, Level02);
}
void TSC_FilterColorg(int Level01, int Level02)
{
  if (Level01 != 0)
    Level01 = HIGH;
  if (Level02 != 0)
    Level02 = HIGH;
  digitalWrite(S2g, Level01);
  digitalWrite(S3g, Level02);
}
void TSC_Countd()
{
  g_countd ++;
}
void TSC_Countg()
{
  g_countg ++;
}
void TSC_Callbackd()
{
  switch (g_flagd)
  {
    case 0:
      Serial.println("->WB Start");
      TSC_WBd(LOW, LOW); // Filtre sans rouge
      break;
    case 1:
      Serial.print("->Frequency R=");
      Serial.println(g_countd);
      g_arrayd[0] = g_countd;
      TSC_WBd(HIGH, HIGH); // Filtre sans vert
      break;
    case 2:
      Serial.print("->Frequency V=");
      Serial.println(g_countd);
      g_arrayd[1] = g_countd;
      TSC_WBd(LOW, HIGH); // Filtre sans bleu
      break;
    case 3:
      Serial.print("->Frequency B=");
      Serial.println(g_countd);
      Serial.println("->WB End");
      g_arrayd[2] = g_countd;
      TSC_WBd(HIGH, LOW); // Pas de filtre
      break;
    default:
      g_countd = 0;
      break;
  }
}
void TSC_Callbackg()
{
  switch (g_flagg)
  {
    case 0:
      Serial.println("->WB Start");
      TSC_WBg(LOW, LOW); // Filtre sans rouge
      break;
    case 1:
      Serial.print("->Frequency R=");
      Serial.println(g_countg);
      g_arrayg[0] = g_countg;
      TSC_WBg(HIGH, HIGH); // Filtre sans vert
      break;
    case 2:
      Serial.print("->Frequency V=");
      Serial.println(g_countg);
      g_arrayg[1] = g_countg;
      TSC_WBg(LOW, HIGH); // Filtre sans bleu
      break;
    case 3:
      Serial.print("->Frequency B=");
      Serial.println(g_countg);
      Serial.println("->WB End");
      g_arrayg[2] = g_countg;
      TSC_WBg(HIGH, LOW); // Pas de filtre
      break;
    default:
      g_countg = 0;
      break;
  }
}
void TSC_WBd(int Level0, int Level1) // Balance des blancs
{
  g_countd = 0;
  g_flagd ++;
  TSC_FilterColord(Level0, Level1);
  Timer1.setPeriod(625000);
}
void TSC_WBg(int Level0, int Level1) // Balance des blancs
{
  g_countg = 0;
  g_flagg ++;
  TSC_FilterColorg(Level0, Level1);
  Timer1.setPeriod(625000);
}

void captLumDroit(){
  g_flagd = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayd[i] * g_SFd[i]));
  Rd = int(g_arrayd[0] * g_SFd[0]);
  Vd = int(g_arrayd[1] * g_SFd[1]);
  //Bd = int(g_arrayd[0] * g_SFd[0]);
  if (Rd > 45 ) {
    Serial.println("ROUGE");
  }
  if (Vd > 39) {
    Serial.println("VERT");
  }
}
void captLumGauche(){
  g_flagg = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayg[i] * g_SFg[i]));
  Rg = int(g_arrayg[0] * g_SFg[0]);
  Vg = int(g_arrayg[1] * g_SFg[1]);
  //Bg = int(g_arrayg[0] * g_SFg[0]);
  if (Rg > 45 ) {
    Serial.println("ROUGE");
  }
  if (Vg > 39) {
    Serial.println("VERT");
  }
}



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

  // CAPTEUR DE COULEUR
  
   TSC_Initd();
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackd);
  attachInterrupt(0, TSC_Countd, RISING);
  delay(2500);
  for (int i = 0; i < 3; i++)
    Serial.println(g_arrayd[i]);
  g_SFd[0] = 255.0 / g_arrayd[0]; // valeur R
  g_SFd[1] = 255.0 / g_arrayd[1] ; // valeur V
  g_SFd[2] = 255.0 / g_arrayd[2] ; // valeur B
  Serial.println(g_SFd[0]);
  Serial.println(g_SFd[1]);
  Serial.println(g_SFd[2]);

  TSC_Initg();
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackg);
  attachInterrupt(0, TSC_Countg, RISING);
  delay(2500);
  for (int i = 0; i < 3; i++)
    Serial.println(g_arrayg[i]);
  g_SFg[0] = 255.0 / g_arrayg[0]; // valeur R
  g_SFg[1] = 255.0 / g_arrayg[1] ; // valeur V
  g_SFg[2] = 255.0 / g_arrayg[2] ; // valeur B
  Serial.println(g_SFg[0]);
  Serial.println(g_SFg[1]);
  Serial.println(g_SFg[2]);
}


void loop() {

// Lire les valeurs des capteurs
  int value_A0 = analogRead(IN_A0);
  int value_A1 = analogRead(IN_A1);

  // Déterminer la direction du robot
  if (value_A0 < 50 && value_A1 < 50) {
    // Le robot est sur du blanc
    avant();
  } 
  else if (value_A0 > 50 && value_A1<50) {
    // Le robot est à droite de la ligne noire
    tournergauche();
  } 
  else if (value_A1 > 50 && value_A0<50) {
    // Le robot est à gauche de la ligne noire
    tournerdroite(); 
  }  
  else if (value_A1 > 50 && value_A0 > 50) {
    captLumDroit();
    if(Rd > 45){
      rougeDroit = 1;
    }
    if(Vd > 39){
      vertDroit = 1;
    }
    captLumGauche();
    if(Rg > 45){
      rougeGauche = 1;
    }
    if(Vg > 39){
      vertGauche = 1;
    }
  delay(1875);
  if (vertDroit = 1 && vertGauche = 1) {
    tourner180°();
  }
  else if (vertDroit = 1) {
    tounerdroite();
  }
  else if (vertGauche = 1)
  }
}

void avant() {
  
// On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
configurerSensDeRotationPontA(MARCHE_AVANT);
// On démarre le moteur raccordé au pont A à la vitesse maximale
changeVitesseMoteurPontA(vitesseMaximale*1.1);
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
