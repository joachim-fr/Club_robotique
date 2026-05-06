int capteurg1 = 4; //Pin pour les capteurs gauches (g1,g4 pas pwm à tester)
int capteurg2 = 5;
int capteurg3 = 6;
int capteurg4 = 7;

int capteurd1 = 8; //Pin pour les capteurs droits
int capteurd2 = 9;
int capteurd3 = 10;
int capteurd4 = 11;

int moteurga=12,moteurgb=3,moteurda=13,moteurdb=11; //Pin pour deux moteurs : ga et gb pour gauche, da et db pour droite
bool c1,c2,c3,c4,c5,c6,c7,c8;

int poidsgauche, poidsdroit;

void setup() {
  Serial.begin(9550);
  pinMode(capteurg1,INPUT);
  pinMode(capteurg2,INPUT);
  pinMode(capteurg3,INPUT);
  pinMode(capteurg4,INPUT);
  pinMode(capteurd1,INPUT);
  pinMode(capteurd2,INPUT);
  pinMode(capteurd3,INPUT);
  pinMode(capteurd4,INPUT);
  pinMode(capteurd1,INPUT);
  pinMode(capteurd2,INPUT);
  pinMode(capteurg3,INPUT);
  pinMode(capteurg4,INPUT);
}

//On crée les fonctions
void g() //Fonction qui permet au robot de tourner à gauche
{
  analogWrite(moteurda,55);
  digitalWrite(moteurdb,LOW);
  digitalWrite(moteurga,LOW);
  analogWrite(moteurgb,55);
}
 void d() //Fonction qui permet au robot de tourner à droite
{
  digitalWrite(moteurda,LOW);
  analogWrite(moteurdb,55);
  analogWrite(moteurga,55);
  digitalWrite(moteurgb,LOW);
}
void av() //Fonction qui permet au robot de continuer tout droit
{
  digitalWrite(moteurda,LOW);
  analogWrite(moteurdb,55);
  digitalWrite(moteurga,LOW);
  analogWrite(moteurgb,55);
}

int valeurcapteurs(bool a,bool b,bool c,bool d)
{
  int valeur = 0;
  valeur |=(a << 3);
  valeur |=(b << 2);
  valeur |=(c << 1);
  valeur |=d;
  return valeur;
}

void loop()
{
  c1 = digitalRead(capteurg1);
  c2 = digitalRead(capteurg2);
  c3 = digitalRead(capteurg3);
  c4 = digitalRead(capteurg4);
  c5 = digitalRead(capteurd1);
  c6 = digitalRead(capteurd2);
  c7 = digitalRead(capteurd3);
  c8 = digitalRead(capteurd4);

  poidsgauche = valeurcapteurs(c1,c2,c3,c4);
  poidsdroit = valeurcapteurs(c8,c7,c6,c5);

  if(poidsgauche < poidsdroit)
  {
    d();
  }
  if(poidsgauche > poidsdroit)
  {
    g();
  }
  else
  av();
}
