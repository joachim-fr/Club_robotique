#include <TimerOne.h>

// affichage debug ////////////////////
bool debug = false;

// Calibration du blanc ///////////////
int Ri = 0;
int Bi = 0;
int Vi = 0;

// Paramètres attachInterrup //////////
int temps = 1200;
int periode = 1000000;

// Capteur gauche /////////////////////
const int S0g = 29;
const int S1g = 28;
const int S2g = 27;
const int S3g = 26;
const int OUTg = 2;

int Rg = 0;
int Vg = 0;
int Bg = 0;
int g_countg = 0;
int g_arrayg[3];
int g_flagg = 0;
float g_SFg[3];
bool isBlueG = false;
bool isRedG = false;
bool isGreenG = false;
bool isWhiteG = false;
bool isBlackG = false;

int Rg0 = 0;
int Vg0 = 0;
int Bg0 = 0;

// Capteur droit /////////////////////
const int S0d = 49;
const int S1d = 48;
const int S2d = 47;
const int S3d = 46;
const int OUTd = 3;
int Rd = 0;
int Vd = 0;
int Bd = 0;
int g_countd = 0;
int g_arrayd[3];
int g_flagd = 0;
float g_SFd[3];
bool isBlueD = false;
bool isRedD = false;
bool isGreenD = false;
bool isWhiteD = false;
bool isBlackD = false;

int Rd0 = 0;
int Vd0 = 0;
int Bd0 = 0;

// détection des couleurs //////////////////////////////////////////////////////////////////////
bool isRed(int R,int V,int B) {
  float rp = float(R)/float(Ri);
  float vp = float(V)/float(Vi);
  float bp = float(B)/float(Bi);
  bool isb = rp < 0.3 && vp < 0.3 && bp < 0.3;
  bool isw = rp > 0.7 && vp > 0.7 && bp > 0.7;
  return (rp > vp && rp > bp && !isb && !isw);
}

bool isGreen(int R,int V,int B) {
  float rp = float(R)/float(Ri);
  float vp = float(V)/float(Vi);
  float bp = float(B)/float(Bi);
  bool isb = rp < 0.3 && vp < 0.3 && bp < 0.3;
  bool isw = rp > 0.7 && vp > 0.7 && bp > 0.7;
  return (vp > rp && vp > bp && !isb && !isw);
}

bool isBlue(int R,int V,int B) {
  float rp = float(R)/float(Ri);
  float vp = float(V)/float(Vi);
  float bp = float(B)/float(Bi);
  bool isb = rp < 0.3 && vp < 0.3 && bp < 0.3;
  bool isw = rp > 0.7 && vp > 0.7 && bp > 0.7;
  return (bp > rp && bp > vp && !isb && !isw);
}

bool isWhite(int R,int V,int B) {
  float rp = float(R)/float(Ri);
  float vp = float(V)/float(Vi);
  float bp = float(B)/float(Bi);
  bool isb = rp < 0.3 && vp < 0.3 && bp < 0.3;
  bool isw = rp > 0.7 && vp > 0.7 && bp > 0.7;
  return (isw);
}

bool isBlack(int R, int V, int B){
  float rp = float(R)/float(Ri);
  float vp = float(V)/float(Vi);
  float bp = float(B)/float(Bi);
  bool isb = rp < 0.3 && vp < 0.3 && bp < 0.3;
  bool isw = rp > 0.7 && vp > 0.7 && bp > 0.7;
  return (isb);
}

// fonctions pour capteur droit/////////////////////////////////////////////////////////////////
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

void TSC_FilterColord(int Level01, int Level02)
{
  if (Level01 != 0)
    Level01 = HIGH;
  if (Level02 != 0)
    Level02 = HIGH;
  digitalWrite(S2d, Level01);
  digitalWrite(S3d, Level02);
}

void TSC_Countd()
{
  g_countd ++;
}

void TSC_WBd(int Level0, int Level1) // Balance des blancs
{
  g_countd = 0;
  g_flagd ++;
  TSC_FilterColord(Level0, Level1);
  Timer1.setPeriod(periode);
}

void TSC_Callbackd()
{
  switch (g_flagd)
  {
    case 0:
      if (debug) {
        Serial.println("->WB Start droite");
      }
      TSC_WBd(LOW, LOW); // Filtre sans rouge
      break;
    case 1:
      if (debug) {
        Serial.print("->Frequency R=");
        Serial.println(g_countd);
      }
      g_arrayd[0] = g_countd;
      TSC_WBd(HIGH, HIGH); // Filtre sans vert
      break;
    case 2:
      if (debug) {
        Serial.print("->Frequency V=");
        Serial.println(g_countd);
      }
      g_arrayd[1] = g_countd;
      TSC_WBd(LOW, HIGH); // Filtre sans bleu
      break;
    case 3:
      if (debug) {
        Serial.print("->Frequency B=");
        Serial.println(g_countd);
        Serial.println("->WB End");
      }
      g_arrayd[2] = g_countd;
      TSC_WBd(HIGH, LOW); // Pas de filtre
      break;
    default:
      g_countd = 0;
      break;
  }
}

void captLumD() {
  g_flagd = 0;
  if (debug) {
    for (int i = 0; i < 3; i++)
      Serial.println(int(g_arrayd[i] * g_SFd[i]));
  }
  Rd = int(g_arrayd[0]); //* g_SFd[0]);
  Vd = int(g_arrayd[1]); //  * g_SFd[1]);
  Bd = int(g_arrayd[2]);// * g_SFd[2]);

  Serial.print("Couleur détectée à droite : ");
  if (isRed(Rd,Vd,Bd)) {
    Serial.println("Rouge");
    isRedD = true;
    isGreenD = false;
    isBlueD = false;
    isWhiteD = false;
    isBlackD = false;
  }
  else if (isGreen(Rd,Vd,Bd)) {
    Serial.println("Vert");
    isRedD = false;
    isGreenD = true;
    isBlueD = false;
    isWhiteD = false;
    isBlackD = false;
  }
  else if (isBlue(Rd,Vd,Bd)) {
    Serial.println("Bleu");
    isRedD = false;
    isGreenD = false;
    isBlueD = true;
    isWhiteD = false;
    isBlackD = false;
  }
  else if (isWhite(Rd,Vd,Bd)) {
    Serial.println("Blanc");
    isRedD = false;
    isGreenD = false;
    isBlueD = false;
    isWhiteD = true;
    isBlackD = false;
  }
  else if(isBlack(Rd,Vd,Bd)){
    Serial.println("Noir");
    isRedD = false;
    isGreenD = false;
    isBlueD = false;
    isWhiteD = false;
    isBlackD = true;
  }
  else {
    Serial.println("Non fiable");
  }
}

void setupd() {
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackd);
  attachInterrupt(digitalPinToInterrupt(OUTd), TSC_Countd, RISING);
  delay(temps);
  if (debug) {
    for (int i = 0; i < 3; i++)
      Serial.println(g_arrayd[i]);
  }
  g_SFd[0] = 255.0 / g_arrayd[0]; // valeur R
  g_SFd[1] = 255.0 / g_arrayd[1] ; // valeur V
  g_SFd[2] = 255.0 / g_arrayd[2] ; // valeur B
  if (debug) {
    Serial.println(g_SFd[0]);
    Serial.println(g_SFd[1]);
    Serial.println(g_SFd[2]);
  }
}

void mesured() {
  setupd();
  captLumD();
}
// fonctions pour capteur gauche///////////////////////////////////////////////////////////////////
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

void TSC_FilterColorg(int Level01, int Level02)
{
  if (Level01 != 0)
    Level01 = HIGH;
  if (Level02 != 0)
    Level02 = HIGH;
  digitalWrite(S2g, Level01);
  digitalWrite(S3g, Level02);
}

void TSC_Countg()
{
  g_countg ++;
}

void TSC_WBg(int Level0, int Level1) // Balance des blancs
{
  g_countg = 0;
  g_flagg ++;
  TSC_FilterColorg(Level0, Level1);
  Timer1.setPeriod(periode);
}

void TSC_Callbackg()
{
  switch (g_flagg)
  {
    case 0:
      if (debug) {
        Serial.println("->WB Start gauche");
      }
      TSC_WBg(LOW, LOW); // Filtre sans rouge
      break;
    case 1:
      if (debug) {
        Serial.print("->Frequency R=");
        Serial.println(g_countg);
      }
      g_arrayg[0] = g_countg;
      TSC_WBg(HIGH, HIGH); // Filtre sans vert
      break;
    case 2:
      if (debug) {
        Serial.print("->Frequency V=");
        Serial.println(g_countg);
      }
      g_arrayg[1] = g_countg;
      TSC_WBg(LOW, HIGH); // Filtre sans bleu
      break;
    case 3:
      if (debug) {
        Serial.print("->Frequency B=");
        Serial.println(g_countg);
        Serial.println("->WB End");
      }
      g_arrayg[2] = g_countg;
      TSC_WBg(HIGH, LOW); // Pas de filtre
      break;
    default:
      g_countg = 0;
      break;
  }
}

void captLumG() {
  g_flagg = 0;
  if (debug) {
    for (int i = 0; i < 3; i++)
      Serial.println(int(g_arrayg[i] * g_SFg[i]));
  }
  Rg = int(g_arrayg[0] );//* g_SFg[0]);
  Vg = int(g_arrayg[1] );//* g_SFg[1]);
  Bg = int(g_arrayg[2] );//* g_SFg[2]);

  Serial.print("Couleur détectée à gauche : ");
  if (isRed(Rg,Vg,Bg)) {
    Serial.println("Rouge");
    isRedG = true;
    isGreenG = false;
    isBlueG = false;
    isWhiteG = false;
    isBlackG = false;
  }
  else if (isGreen(Rg,Vg,Bg)) {
    Serial.println("Vert");
    isRedG = false;
    isGreenG = true;
    isBlueG = false;
    isWhiteG = false;
    isBlackG = false;
  }
  else if (isBlue(Rg,Vg,Bg)) {
    Serial.println("Bleu");
    isRedG = false;
    isGreenG = false;
    isBlueG = true;
    isWhiteG = false;
    isBlackG = false;
  }

  else if (isWhite(Rg,Vg,Bg)) {
    Serial.println("Blanc");
    isRedG = false;
    isGreenG = false;
    isBlueG = false;
    isWhiteG = true;
    isBlackG = false;
  }
  else if (isBlack(Rg,Vg,Bg)) {
    Serial.println("Blanc");
    isRedG = false;
    isGreenG = false;
    isBlueG = false;
    isWhiteG = false;
    isBlackG = true;
  }
  else {
    Serial.println("Non fiable");
  }
}

void setupg() {
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackg);
  attachInterrupt(digitalPinToInterrupt(OUTg), TSC_Countg, RISING);
  delay(temps);
  if (debug) {
    for (int i = 0; i < 3; i++)
      Serial.println(g_arrayg[i]);
  }
  g_SFg[0] = 255.0 / g_arrayg[0]; // valeur R
  g_SFg[1] = 255.0 / g_arrayg[1] ; // valeur V
  g_SFg[2] = 255.0 / g_arrayg[2] ; // valeur B
  if (debug) {
    Serial.println(g_SFg[0]);
    Serial.println(g_SFg[1]);
    Serial.println(g_SFg[2]);
  }
}

void mesureg() {
  setupg();
  captLumG();
}

// main ///////////////////////////////////////////////////////////////////////////////////////
void calibration() {
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackd);
  attachInterrupt(digitalPinToInterrupt(OUTd), TSC_Countd, RISING);
  delay(2 * temps);
  Rd0 = g_arrayd[0]; // valeur R
  Vd0 = g_arrayd[1] ; // valeur V
  Bd0 = g_arrayd[2] ; // valeur B
  Timer1.detachInterrupt();
  detachInterrupt(OUTd);
  Serial.println("Calibration droite");
  Serial.println(Rd0);
  Serial.println(Vd0);
  Serial.println(Bd0);

  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackg);
  attachInterrupt(digitalPinToInterrupt(OUTg), TSC_Countg, RISING);
  delay(2 * temps);
  Timer1.detachInterrupt();
  detachInterrupt(OUTg);
  Serial.println("Calibration gauche");
  Rg0 = g_arrayg[0]; // valeur R
  Vg0 = g_arrayg[1] ; // valeur V
  Bg0 = g_arrayg[2] ; // valeur B
  Serial.println(Rg0);
  Serial.println(Vg0);
  Serial.println(Bg0);
  Ri = min(Rg0, Rd0);
  Vi = min(Vg0, Vd0);
  Bi = min(Bg0, Bd0);
  Serial.println("Calibration blanc :");
  Serial.println(Ri);
  Serial.println(Vi);
  Serial.println(Bi);
}

void setup()
{
  Serial.begin(9600);
  TSC_Initg();
  TSC_Initd();
  calibration();
}

void loop()
{
  mesured();
  delay(temps);
  mesureg();
  delay(temps);
// Logique de mouvement basée sur les capteurs
  if (D1 && D2 && D3 && D4 && G1 && G2 && G3 && G4) {
    Serial.println("STOP");
    arreter();
    mesured();
    mesureg();
    mesured();
    mesureg();
    if(isGreenD && isWhiteG){
      tournerdroiteextremite(80);
    }
    else if(isGreenG && isWhiteD){
     tournergaucheextremite(80); 
    }
    else if(isGreenD && isGreenG){
      avancer(30);
      delay(500);
        tournergaucheextremite(80);
      delay(500);
        avancer(30);
      delay(500);
        tournergaucheextremite(80);
      delay(500);
        avancer(30);
      delay(500);
        tournergaucheextremite(80);
      delay(500);
        avancer(30);
      delay(500);
        tournerdroiteextremite(80);
      delay(500);
        avancer(100);
    }
    else if(isWhiteD && isWhiteG){
      avancer(100);
    }    
 }  
  // CREER FONCTION DEMI-TOUR:
  // if (isGreenD && isGreenG) {
  //      Serial.println("je fais demi-tour ;)");
  //    }

  
