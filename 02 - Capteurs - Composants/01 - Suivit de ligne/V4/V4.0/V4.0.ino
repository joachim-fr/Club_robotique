#include <QTRSensors.h>

// This example is designed for use with eight RC QTR sensors. These
// reflectance sensors should be connected to digital pins 3 to 10. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The main loop of the example reads the raw sensor values (uncalibrated). You
// can test this by taping a piece of 3/4" black electrical tape to a piece of
// white paper and sliding the sensor across it. It prints the sensor values to
// the serial monitor as numbers from 0 (maximum reflectance) to 2500 (minimum
// reflectance; this is the default RC timeout, which can be changed with
// setTimeout()).

const uint8_t SensorCount = 4;

QTRSensors qtrg;
uint16_t sensorValuesG[SensorCount];

QTRSensors qtrd;
uint16_t sensorValuesD[SensorCount];

void setup()
{
  // configure the sensors
  qtrg.setTypeRC();
  qtrg.setSensorPins((const uint8_t[]){24, 25, 22, 23}, SensorCount);
  qtrg.setEmitterPin(31);

  qtrd.setTypeRC();
  qtrd.setSensorPins((const uint8_t[]){51, 50, 52, 53}, SensorCount);
  qtrd.setEmitterPin(45);



   delay(500);

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  delay(1000); 

}


void loop()
{
  // read raw sensor values
  qtrd.read(sensorValuesD);
  qtrg.read(sensorValuesG);

  // print the sensor values as numbers from 0 to 2500, where 0 means maximum
  // reflectance and 2500 means minimum reflectance
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValuesG[i]);
    Serial.print('\t');
  }
  Serial.print('\t');
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValuesD[i]);
    Serial.print('\t');
  }
  Serial.print('\t');
  Serial.print('\t');
  Serial.print('\t');

  Serial.print(qtrg.readLineBlack(sensorValuesG));

  Serial.print('\t');

  Serial.print(qtrd.readLineBlack(sensorValuesD));
    
  Serial.println();

}
  
