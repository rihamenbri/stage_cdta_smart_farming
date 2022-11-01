// MQ135

#include <Arduino.h>

#include <Adafruit_Sensor.h> 
#include "MQ135.h"
#include <DHT.h>
#include <DHT_U.h>  // Library : DHT sensor library
#include "Wire.h"
#include "SPI.h"

#define DHTPIN 21

#define DHTTYPE DHT22

const int ANALOGPIN= 14 ;
DHT dht(DHTPIN, DHTTYPE);

MQ135 gasSensor = MQ135(ANALOGPIN);

float temperature ; 
float humidity ;

void setup(){
Serial.begin(9600);
dht.begin();
}

void loop(){
 delay(2000);
humidity = dht.readHumidity();
temperature = dht.readTemperature();

********* calibrage******************
 float rzero = gasSensor.getRZero();
 float correctedRZero =  gasSensor.getCorrectedRZero(temperature, humidity);
 Serial.print("rzero" );
Serial.println(rzero);
 Serial.print("correctedRZero:") ;
 Serial.println(correctedRZero);
********************************************

  float ppm = gasSensor.getPPM();
  float correctedPPM = gasSensor.getCorrectedPPM(temperature, humidity);
Serial.print("temperature : \t ");
Serial.println(temperature);
Serial.print("humidity : \t ");
Serial.println(humidity);
Serial.println("*******");
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");
delay(5000);
}