#include <TimerOne.h>

#define S0b 12
#define S1b 4
#define S2b 26  
#define S3b 29
#define OUTb 27

#define S0a 6
#define S1a 7
#define S2a 31
#define S3a 30
#define OUTa 33

int R = 0;
int V = 0;
int g_count = 0;
int g_array[3];
int g_flag = 0;
float g_SF[3];

void TSC_Init()
{
  pinMode(S0a, OUTPUT);
  pinMode(S1a, OUTPUT);
  pinMode(S2a, OUTPUT);
  pinMode(S3a, OUTPUT);
  pinMode(OUTa, INPUT);
  digitalWrite(S0a, LOW);
  digitalWrite(S1a, HIGH);

  pinMode(S0b, OUTPUT);
  pinMode(S1b, OUTPUT);
  pinMode(S2b, OUTPUT);
  pinMode(S3b, OUTPUT);
  pinMode(OUTb, INPUT);
  digitalWrite(S0b, LOW);
  digitalWrite(S1b, HIGH);
}

void TSC_FilterColor(int Level01, int Level02)
{
  if (Level01 != 0)
    Level01 = HIGH;
  if (Level02 != 0)
    Level02 = HIGH;
  digitalWrite(S2a, Level01);
  digitalWrite(S3a, Level02);
    digitalWrite(S2b, Level01);
  digitalWrite(S3b, Level02);
}

void TSC_Count()
{
  g_count ++;
}



void TSC_Callback()
{
  switch (g_flag)
  {
    case 0:
      Serial.println("->WB Start");
      TSC_WB(LOW, LOW); // Filtre sans rouge
      break;
    case 1:
      Serial.print("->Frequency R=");
      Serial.println(g_count);
      g_array[0] = g_count;
      TSC_WB(HIGH, HIGH); // Filtre sans vert
      break;
    case 2:
      Serial.print("->Frequency V=");
      Serial.println(g_count);
      g_array[1] = g_count;
      TSC_WB(LOW, HIGH); // Filtre sans bleu
      break;
    case 3:
      Serial.print("->Frequency B=");
      Serial.println(g_count);
      Serial.println("->WB End");
      g_array[2] = g_count;
      TSC_WB(HIGH, LOW); // Pas de filtre
      break;
    default:
      g_count = 0;
      break;
  }
}





void TSC_WB(int Level0, int Level1) // Balance des blancs
{
  g_count = 0;
  g_flag ++;
  TSC_FilterColor(Level0, Level1);
  Timer1.setPeriod(625000);
}
void setup()
{
  TSC_Init();
  Serial.begin(9600);
  Timer1.initialize();
  Timer1.attachInterrupt(TSC_Callback);
  attachInterrupt(0, TSC_Count, RISING);
  delay(2500);
  for (int i = 0; i < 3; i++)
    Serial.println(g_array[i]);
  g_SF[0] = 255.0 / g_array[0]; // valeur R
  g_SF[1] = 255.0 / g_array[1] ; // valeur V
  g_SF[2] = 255.0 / g_array[2] ; // valeur B
  Serial.println(g_SF[0]);
  Serial.println(g_SF[1]);
  Serial.println(g_SF[2]);
}





void loop()
{
  g_flag = 0;
  for (int i = 0; i < 3; i++)
    Serial.println(int(g_array[i] * g_SF[i]));
  R = int(g_array[0] * g_SF[0]);
  V = int(g_array[1] * g_SF[1]);
  // B = int(g_array[0] * g_SF[0]);
  if (R > 43 ) {
    Serial.println("ROUGE");
  }
  if (V > 39) {
    Serial.println("VERT");
 
  }

  delay(1875);
}
