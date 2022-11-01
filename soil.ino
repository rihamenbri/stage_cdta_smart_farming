```arduino

 // DHT22 ******************

#include <Arduino.h>
#include <Adafruit_Sensor.h>  // Library : Adafruit Unified Sensor
#include <DHT.h>
#include <DHT_U.h>  // Library : DHT sensor library
// #include "Wire.h"
// #include "SPI.h"

#define DHTPIN 21// IMPORTANT D2 on NodeMCU is GPIO 4

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    dht.begin();
 
void loop()
{
    // put your main code here, to run repeatedly:
    delay(2000);
    float h = dht.readHumidity();
    //celcius
    float t = dht.readTemperature();
    //fahrenheit
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
     
}

```