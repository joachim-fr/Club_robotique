

#include <Servo.h> // librairie pour servomoteur



int ReceptionOctet=0; // variable de stockage des octets reçus par port série
int ReceptionNombre=0; // variable de calcul du nombre reçu par port série
float impulsion=0; // variable pour impulsion - valeur médiane initiale



Servo mon_servo;  // crée un objet servo pour contrôler le servomoteur


void setup()   { // debut de la fonction setup()


mon_servo.attach(2);  // attache l'objet servo à la broche 2

Serial.begin(115200); // initialise connexion série à 115200 bauds
// IMPORTANT : régler le terminal côté PC avec la même valeur de transmission 





} 

void loop(){ // debut de la fonction loop()


if (Serial.available()>0) { // si caractère dans la file d'attente

  //---- lecture du nombre reçu 
  while (Serial.available()>0) { // tant que buffer pas vide pour lire d'une traite tous les caractères reçus

    ReceptionOctet= Serial.read(); // renvoie le 1er octet présent dans la file attente série (-1 si aucun) 
    ReceptionOctet=ReceptionOctet-48; // transfo valeur ASCII en valeur décimale

    if ((ReceptionOctet>=0)&&(ReceptionOctet<=9))     ReceptionNombre = (ReceptionNombre*10)+ReceptionOctet;
    // si valeur reçue correspond à un chiffre on calcule nombre

    delay(15); // pause pour laisser le temps au Serial.available de recevoir les caractères

  } // fin whileattente de 15ms

   Serial.print ("Essai de commande= ");
   Serial.println(ReceptionNombre); // affiche valeur numerique entière ou à virgule au format décimal


   //----- positionnement du servomoteur en fonction de la largeur d'impulsion demandée 
   impulsion= (float(ReceptionNombre)+60.83)/0.099; 

    if (impulsion>2433)impulsion=2433; // pour éviter valeur supérieure à 3000 = position maxi possible servomoteur
    if (impulsion<615)impulsion=615; // pour éviter valeur inférieure à 10 = position maxi possible servomoteur

    mon_servo.writeMicroseconds(impulsion); // génère l'impulsion de la durée voulue en µs pour le servomoteur

    Serial.print ("Impulsion commande servomoteur = ");
    Serial.print(impulsion); // affiche valeur numerique entière 
    Serial.println (" microsecondes ");

    delay (1000); //entre chaque changement de position

    ReceptionNombre=0; // réinitialisation de la variable de calcul du nombre reçu

} // end if


} // fin de la fonction loop() - le programme recommence au début de la fonction loop sans fin
// ********************************************************************************

// --- Fin programme ---
//Etude du servomoteur
//+pince v2
pince v2
