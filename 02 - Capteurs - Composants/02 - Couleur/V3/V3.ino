// capteur gauche : variables terminant par g
// capteur droit  : variables terminant par d

#include <TimerOne.h>

#define S0g 6
#define S1g 5
#define S2g 4
#define S3g 3
#define OUTg 2
int Rg = 0;
int Vg = 0;
int Bg = 0;
int g_countg = 0;
int g_arrayg[3];
int g_flagg = 0;
float g_SFg[3];

//#define S0d 10
//#define S1d 11
//#define S2d 12
//#define S3d 9
//#define OUTd 8
//int Rd = 0;
//int Vd = 0;
//int Bd = 0;
//int g_countd = 0;
//int g_arrayd[3];
//int g_flagd = 0;
//float g_SFd[3];

// fonctions pour capteur droit/////////////////////////////////////////////////////////////////
//void TSC_Initd()
//{
//  pinMode(S0d, OUTPUT);
//  pinMode(S1d, OUTPUT);
//  pinMode(S2d, OUTPUT);
//  pinMode(S3d, OUTPUT);
//  pinMode(OUTd, INPUT);
//  digitalWrite(S0d, LOW);
//  digitalWrite(S1d, HIGH);
//}
//void TSC_FilterColord(int Level01, int Level02)
//{
//  if (Level01 != 0)
//    Level01 = HIGH;
//  if (Level02 != 0)
//    Level02 = HIGH;
//  digitalWrite(S2d, Level01);
//  digitalWrite(S3d, Level02);
//}
//void TSC_Countd()
//{
//  g_countd ++;
//}
//void TSC_Callbackd()
//{
//  switch (g_flagd)
//  {
//    case 0:
//      Serial.println("->WB Start");
//      TSC_WBd(LOW, LOW); // Filtre sans rouge
//      break;
//    case 1:
//      Serial.print("->Frequency R=");
//      Serial.println(g_countd);
//      g_arrayd[0] = g_countd;
//      TSC_WBd(HIGH, HIGH); // Filtre sans vert
//      break;
//    case 2:
//      Serial.print("->Frequency V=");
//      Serial.println(g_countd);
//      g_arrayd[1] = g_countd;
//      TSC_WBd(LOW, HIGH); // Filtre sans bleu
//      break;
//    case 3:
//      Serial.print("->Frequency B=");
//      Serial.println(g_countd);
//      Serial.println("->WB End");
//      g_arrayd[2] = g_countd;
//      TSC_WBd(HIGH, LOW); // Pas de filtre
//      break;
//    default:
//      g_countd = 0;
//      break;
//  }
//}
//void TSC_WBd(int Level0, int Level1) // Balance des blancs
//{
//  g_countd = 0;
//  g_flagd ++;
//  TSC_FilterColord(Level0, Level1);
//  Timer1.setPeriod(625000);
//}
//void captLumDroit(){
//  g_flagd = 0;
//  for (int i = 0; i < 3; i++)
//    Serial.println(int(g_arrayd[i] * g_SFd[i]));
//  Rd = int(g_arrayd[0] * g_SFd[0]);
//  Vd = int(g_arrayd[1] * g_SFd[1]);
//  Bd = int(g_arrayd[2] * g_SFd[2]);
//  Serial.print("Couleur détectée à droite : ");
//  if(Rd > 200 && Vd < 160){
//    Serial.println("Rouge");
//  }
//  else if(Rd < 160 && Vd > 200){
//    Serial.println("Vert");
//  }
//  else{
//    Serial.println("Non fiable");
//  }
//}

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
  Timer1.setPeriod(1000000);
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
void captLumGauche(){
  g_flagg = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_arrayg[i] * g_SFg[i]));
  Rg = int(g_arrayg[0] * g_SFg[0]);
  Vg = int(g_arrayg[1] * g_SFg[1]);
  Bg = int(g_arrayg[2] * g_SFg[2]);

  Serial.print("Couleur détectée à gauche : ");
  if(Rg > 2*Vg && Rg > 2*Bg){
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

// main ///////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  
//  TSC_Initd();
//  Timer1.initialize();
//  Timer1.attachInterrupt(TSC_Callbackd);
//  attachInterrupt(0, TSC_Countd, RISING);
//  delay(2500);
//  for (int i = 0; i < 3; i++)
//    Serial.println(g_arrayd[i]);
//  g_SFd[0] = 255.0 / g_arrayd[0]; // valeur R
//  g_SFd[1] = 255.0 / g_arrayd[1] ; // valeur V
//  g_SFd[2] = 255.0 / g_arrayd[2] ; // valeur B
//  Serial.println(g_SFd[0]);
//  Serial.println(g_SFd[1]);
//  Serial.println(g_SFd[2]);

  TSC_Initg();
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callbackg);
  attachInterrupt(0, TSC_Countg, RISING);
  delay(4000);
  for (int i = 0; i < 3; i++)
    Serial.println(g_arrayg[i]);
  g_SFg[0] = 255.0 / g_arrayg[0]; // valeur R
  g_SFg[1] = 255.0 / g_arrayg[1] ; // valeur V
  g_SFg[2] = 255.0 / g_arrayg[2] ; // valeur B
  Serial.println(g_SFg[0]);
  Serial.println(g_SFg[1]);
  Serial.println(g_SFg[2]);
}

void loop()
{
  //captLumDroit();
  captLumGauche();
  

  delay(4000);
}