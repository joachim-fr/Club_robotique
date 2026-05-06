int capteurG1 = 24; //Pin pour Le Capteur Gauche (g1, g4 pas pwm, à tester)
int capteurG2 = 25;
int capteurG3 = 22;
int capteurG4 = 23;

int capteurD1 = 52; //Pin pour Le Capteur Droit
int capteurD2 = 53;
int capteurD3 = 51;
int capteurD4 = 50;


int moteurGA=12,moteurGB=3,moteurDA=13,moteurDB=11; //Pin pour Deux moteurs (GA et GB pour le moteur Gauche, DA et DB pour le moteur de droite)
int capteurG1_val = digitalRead(capteurG1);
int capteurG2_val = digitalRead(capteurG2);
int capteurG3_val = digitalRead(capteurG3);
int capteurG4_val = digitalRead(capteurG4);

int capteurD1_val = digitalRead(capteurD1);
int capteurD2_val = digitalRead(capteurD2);
int capteurD3_val = digitalRead(capteurD3);
int capteurD4_val = digitalRead(capteurD4);

void setup() {

Serial.begin(9550);
pinMode(capteurG1,INPUT);
pinMode(capteurG2,INPUT);
pinMode(capteurG3,INPUT);
pinMode(capteurG4,INPUT);
pinMode(capteurD1,INPUT);
pinMode(capteurD2,INPUT);
pinMode(capteurD3,INPUT);
pinMode(capteurD4,INPUT);  
pinMode(moteurDA,OUTPUT);
pinMode(moteurDB,OUTPUT);
pinMode(moteurGA,OUTPUT);
pinMode(moteurGB,OUTPUT);

}
//on Crée Les Fonctions
void d() {  //Fonction qui permet au robot de tourner a droite
        
      digitalWrite(moteurDA,LOW);
      analogWrite(moteurDB,55);
      analogWrite(moteurGA,55);
      digitalWrite(moteurGB,LOW);
          }
void g() {  //Fonction qui permet au robot de tourner a gauche

      analogWrite(moteurDA,55);
      digitalWrite(moteurDB,LOW);
      digitalWrite(moteurGA,LOW);
      analogWrite(moteurGB,55);
          }
void av() { //Fonction qui permet au robot de continuer tout droit
        
      digitalWrite(moteurDA,LOW);
      analogWrite(moteurDB,55);
      digitalWrite(moteurGA,LOW);
      analogWrite(moteurGB,55);
}
          
void loop() {

  int C1 = digitalRead(capteurG1);
  int C2 = digitalRead(capteurG2);
  int C3 = digitalRead(capteurG3);
  int C4 = digitalRead(capteurG4);
  int C5 = digitalRead(capteurD1);
  int C6 = digitalRead(capteurD2);
  int C7 = digitalRead(capteurD3);
  int C8 = digitalRead(capteurD4);




  if ((C1 + C3 + C4) * (C7 + ~C8) * (C6 + ~C7) * (C5 + ~C6) * (C3 + ~C5 + C6) * (C4 + ~C5) * (C2 + ~C3 + C7) * (C1 + ~C2 + C3) * (C1 + ~C2 + ~C7) * (~C1 + ~C8) * (~C1 + C2 + ~C4) * (~C1 + C3 + ~C4))
  {
    Serial.println("Tourner à gauche");
    g();
  }
  else if (~C1 && ~C2 && ~C3 && ~C4 && ~C5 && ~C6 && C8 + ~C1 && ~C2 && ~C3 && ~C4 && ~C5 && C7 + ~C1 && ~C2 && ~C3 && ~C4 && C6 && ~C8 + ~C1 && ~C2 && ~C3 && ~C4 && C6 && C7 + ~C1 && ~C2 && ~C3 && ~C4 && C5 && ~C7 && ~C8 && + ~C1 && ~C2 && C3 && ~C5 && ~C6 && ~C7 && ~C8 + ~C1 && ~C2 && C3 && C4 && ~C6 && ~C7 && ~C8 + ~C1 && C2 && ~C3 && ~C4 && ~C5 && ~C6 && ~C7 && ~C8 + ~C1 && C2 && C3 && C4 && C5 && C6 && C7 && C8)
  {
    Serial.println("Tourner à droite");
    d();
  }
  else if (~C1 && ~C2 && ~C3 && ~C4 && ~C5 && ~C6 && ~C7 && ~C8 + ~C1 && ~C2 && ~C3 && C4 && C5 && ~C6 && ~C7 && ~C8)
  {
    Serial.println("Continuer tout droit");
    a();
  }
}