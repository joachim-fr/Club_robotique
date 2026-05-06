#include <TimerOne.h>

const int S0g = 29;
const int S1g = 28;
const int S2g = 27;
const int S3g = 26;
const int OUTg = 2;
int temps = 2000;
//int periode = 500000;
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

int Rg0 = 0;
int Vg0 = 0;
int Bg0 = 0;

int Ri = 0;
int Bi = 0;
int Vi = 0;
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

int Rd0 = 0;
int Vd0 = 0;
int Bd0 = 0;

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
  Timer1.setPeriod(500000);
}
void TSC_Callbackd()
{
  switch (g_flagd)
  {
    case 0:
      Serial.println("->WB Start droite");
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

void captLumD() {
  g_flagd = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayd[i] * g_SFd[i]));
  Rd = int(g_arrayd[0]); //* g_SFd[0]);
  Vd = int(g_arrayd[1]); //  * g_SFd[1]);
  Bd = int(g_arrayd[2]);// * g_SFd[2]);

  Serial.print("Couleur détectée à droite : ");
  //if(Rd > Vd && Rd > Bd){
  if (Rd > 1.5 * Vd && Rd > 1.5 * Bd && Rd < Ri) {
    Serial.println("Rouge");
    isRedD = true;
    isBlueD = false;
    isGreenD = false;
    isWhiteD = false;
  }
  else if (Vd > Rd && Vd > Bd) {
    Serial.println("Vert");
    isRedD = false;
    isBlueD = false;
    isGreenD = true;
    isWhiteD = false;
  }
  else if (Bd > Rd*1.5 && Bd > Vd*1.5) {
    Serial.println("Bleu");
    isRedD = false;
    isBlueD = true;
    isGreenD = false;
    isWhiteD = false;
  }
  else if (Rd > Ri && Bd > Bi && Vd > Vi) {   
    Serial.println("blanc");
    isRedD = false;
    isBlueD = false;
    isGreenD = false;
    isWhiteD = true;
  }
  else {
    Serial.println("Non fiable");
  }
}
void setupd() {
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackd);
  attachInterrupt(1, TSC_Countd, RISING);
  delay(temps);
  for (int i = 0; i < 3; i++)
    Serial.println(g_arrayd[i]);
  g_SFd[0] = 255.0 / g_arrayd[0]; // valeur R
  g_SFd[1] = 255.0 / g_arrayd[1] ; // valeur V
  g_SFd[2] = 255.0 / g_arrayd[2] ; // valeur B
  Serial.println(g_SFd[0]);
  Serial.println(g_SFd[1]);
  Serial.println(g_SFd[2]);
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
  Timer1.setPeriod(500000);
}
void TSC_Callbackg()
{
  switch (g_flagg)
  {
    case 0:
      Serial.println("->WB Start gauche");
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
void captLumG() {
  g_flagg = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayg[i] * g_SFg[i]));
  Rg = int(g_arrayg[0] );//* g_SFg[0]);
  Vg = int(g_arrayg[1] );//* g_SFg[1]);
  Bg = int(g_arrayg[2] );//* g_SFg[2]);

  Serial.print("Couleur détectée à gauche : ");
  //if(Rg > Vg && Rg > Bg){
  if (Rg > 1.5 * Vg && Rg > 1.5 * Bg  && Rg < Ri) {
    Serial.println("Rouge");
    isRedG = true;
    isBlueG = false;
    isGreenG = false;
    isWhiteG = false;
  }
  else if (Vg > Rg && Vg > Bg) {
    Serial.println("Vert");
    isRedG = false;
    isBlueG = false;
    isGreenG = true;
    isWhiteG = false;
  }
  else if (Bg > Rg && Bg > Vg) {
    Serial.println("Bleu");
    isRedG = false;
    isBlueG = true;
    isGreenG = false;
    isWhiteG = false;
  }

  else if (Rg*1.5 > Ri && Bg*1.5 > Bi && Vg*1.5 > Vi) {
    Serial.println("blanc");
    isRedG = false;
    isBlueG = false;
    isGreenG = false;
    isWhiteG = true;
  }
    else {
      Serial.println("Non fiable");
    }
  }
  void setupg() {
    Timer1.initialize();
    Timer1.attachInterrupt(TSC_Callbackg);
    attachInterrupt(0, TSC_Countg, RISING);
    delay(temps);
    for (int i = 0; i < 3; i++)
      Serial.println(g_arrayg[i]);
    g_SFg[0] = 255.0 / g_arrayg[0]; // valeur R
    g_SFg[1] = 255.0 / g_arrayg[1] ; // valeur V
    g_SFg[2] = 255.0 / g_arrayg[2] ; // valeur B
    Serial.println(g_SFg[0]);
    Serial.println(g_SFg[1]);
    Serial.println(g_SFg[2]);
  }

  void mesureg() {
    setupg();
    captLumG();
  }

  // main ///////////////////////////////////////////////////////////////////////////////////////
  void calibration() {
    Timer1.initialize();
    Timer1.attachInterrupt(TSC_Callbackd);
    attachInterrupt(1, TSC_Countd, RISING);
    delay(2*temps);
    Rd0 = g_arrayd[0]; // valeur R
    Vd0 = g_arrayd[1] ; // valeur V
    Bd0 = g_arrayd[2] ; // valeur B
    Timer1.detachInterrupt();
    detachInterrupt(1);
    Serial.println("Calibration droite");
    Serial.println(Rd0);
    Serial.println(Vd0);
    Serial.println(Bd0);

    Timer1.initialize();
    Timer1.attachInterrupt(TSC_Callbackg);
    attachInterrupt(0, TSC_Countg, RISING);
    delay(2*temps);
    Timer1.detachInterrupt();
    detachInterrupt(0);
    Serial.println("Calibration gauche");
    Rg0 = g_arrayg[0]; // valeur R
    Vg0 = g_arrayg[1] ; // valeur V
    Bg0 = g_arrayg[2] ; // valeur B
    Serial.println(Rg0);
    Serial.println(Vg0);
    Serial.println(Bg0);
    Ri = min(Rg0,Rd0);
    Vi = min(Vg0,Vd0);
    Bi = min(Bg0,Bd0);
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
    mesureg();

    if (isGreenD && !isGreenG) {
      Serial.println("je vais à droite");
    }

    if (isGreenG && !isGreenD) {
      Serial.println("je vais à gauche");
    }

    if (isGreenD && isGreenG) {
      Serial.println("je fais demi-tour ;)");
    }

    if (!isGreenD && !isGreenG) {
      Serial.println("je vais tout droit");
    }
  }