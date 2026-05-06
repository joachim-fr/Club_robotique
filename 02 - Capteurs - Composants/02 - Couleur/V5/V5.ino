#include <TimerOne.h>

const int S0g = 7;   
const int S1g = 6;   
const int S2g = 5;
const int S3g = 4;   
const int OUTg= 2;
int temps = 2000;
//int periode = 500000;
int Rg = 0;
int Vg = 0;
int Bg = 0;
int g_countg = 0;
int g_arrayg[3];
int g_flagg = 0;
float g_SFg[3];

const int S0d = 11; 
const int S1d = 10;  
const int S2d = 9;
const int S3d = 8;  
const int OUTd= 3;
int Rd = 0;
int Vd = 0;
int Bd = 0;
int g_countd = 0;
int g_arrayd[3];
int g_flagd = 0;
float g_SFd[3];

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

void captLumD(){
  g_flagd = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayd[i] * g_SFd[i]));
  Rd = int(g_arrayd[0]); //* g_SFd[0]);
  Vd = int(g_arrayd[1]); //  * g_SFd[1]);
  Bd = int(g_arrayd[2]);// * g_SFd[2]);
  
  Serial.print("Couleur détectée à droite : ");
  //if(Rd > Vd && Rd > Bd){
  if(Rd > 1.5*Vd && Rd > 1.5*Bd){
    Serial.println("Rouge");
  }
  else if(Vd > Rd && Vd > Bd){
    Serial.println("Vert");
  }
  else if(Bd > Rd && Bd > Vd){
    Serial.println("Bleu");
  }
  else{
    Serial.println("Non fiable");
  }
}
void setupd(){
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

void mesured(){
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
void captLumG(){
  g_flagg = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayg[i] * g_SFg[i]));
  Rg = int(g_arrayg[0] );//* g_SFg[0]);
  Vg = int(g_arrayg[1] );//* g_SFg[1]);
  Bg = int(g_arrayg[2] );//* g_SFg[2]);

  Serial.print("Couleur détectée à gauche : ");
  //if(Rg > Vg && Rg > Bg){
  if(Rg > 1.5*Vg && Rg > 1.5*Bg){
    Serial.println("Rouge");
  }
  else if(Vg > Rg && Vg > Bg){
    Serial.println("Vert");
  }
  else if(Bg > Rg && Bg > Vg){
    Serial.println("Bleu");
  }
  else{
    Serial.println("Non fiable");
  }
}
void setupg(){
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

void mesureg(){
    setupg();
    captLumG();
}

// main ///////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  TSC_Initg();
  TSC_Initd();
}

void loop()
{
  mesured();
  mesureg();
}
