// Code transfert des données des capteurs de l'esp => rasp via MQTT .

#include <Arduino.h>
#include <Adafruit_Sensor.h>  // Library : Adafruit Unified Sensor
 
#include <DHT.h>
#include <DHT_U.h>  // Library : DHT sensor library
#include "Wire.h"
#include "SPI.h"
#include "MQ135.h"
#include "PubSubClient.h" // Connect and publish to the MQTT broker
#include "WiFi.h" 
//**********************************************

#define DHTPIN 21// IMPORTANT D2 on NodeMCU is GPIO 4
#define DHTTYPE DHT22
// WiFi
const char* ssid = "viveHamadi";                 // Your personal network SSID
const char* wifi_password = "samy1243"; // Your personal network password
//MQTT
const char* mqtt_server = "192.168.137.94";  // IP of the MQTT broker
const char* humidity_topic = "home/livingroom/humidity";
const char* temperature_topic = "home/livingroom/temperature";
const char* soil_topic = "home/livingroom/soil";
const char* lum_topic = "home/livingroom/lum";
const char* CO2_topic = "home/livingroom/CO2";


const char* mqtt_username = "vivealik"; // MQTT username
const char* mqtt_password = "fifisurtoi"; // MQTT password
const char* clientID = "client_livingroom"; // MQTT client ID

const int wet  = 1615;   //you need to replace this value with Value_1
const int dry= 3450;  //you need to replace this value with Value_2

int sMValue = 0;
int soilmoisturepercent=0;

const int ANALOGPIN= 13 ;

  MQ135 gasSensor = MQ135(ANALOGPIN);
  DHT dht(DHTPIN, DHTTYPE);

// Initialise the WiFi and MQTT Client objects
// WiFiClient wifiClient;
// // 1883 is the listener port for the Broker
// PubSubClient client(mqtt_server, 1883, wifiClient); 


void connect_wifi () {

   WiFi.begin(ssid, wifi_password);

   while (WiFi.status() != WL_CONNECTED) {

     delay(500);
     Serial.print(".");
   Serial.println("WiFi connected");
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());} }

 void connect_MQTT(){

   Serial.print("Connecting to ");
   Serial.println(ssid);

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)

   if (client.connect(clientID, mqtt_username, mqtt_password)) {
           Serial.println("Connected to MQTT Broker!"); }
  else {
         Serial.println("Connection to MQTT Broker failed...");}
 }
       //******************************

	void setup() {
    Serial.begin(9600);
    dht.begin();
    connect_wifi ();
    }
    


	void loop() {
   
  connect_MQTT();
  Serial.setTimeout(2000);

  int lightVal = analogRead(14);
    sMValue = analogRead(12);  

  soilmoisturepercent = map(sMValue, dry, wet, 0, 100);

	  if(soilmoisturepercent >= 100) {

            Serial.println("100 %");}

		else if(soilmoisturepercent <=0)

			{Serial.println("0 %");}

    else if(soilmoisturepercent >0 && soilmoisturepercent < 100)

            {Serial.print(soilmoisturepercent);

       Serial.println("%"); }

  // delay(5000);
float humidity = dht.readHumidity();
    //celcius
float temperature = dht.readTemperature();

 if (isnan(humidity) || isnan(temperature) )
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

  float ppm = gasSensor.getPPM();
  float correctedPPM = gasSensor.getCorrectedPPM(temperature, humidity);

    Serial.print("humidité du sol : ");
    Serial.println(sMValue);
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C ");
  
    Serial.print("luminosité : \t");
    Serial.println(lightVal);

    Serial.print("\t PPM: ");
    Serial.print(ppm);
    Serial.print("\t Corrected PPM: ");
    Serial.print(correctedPPM);
    Serial.println("ppm");
    Serial.println("*******");

// MQTT can only transmit strings
     String hs="Hum: "+String((float)humidity)+" % ";
     String ts="Temp: "+String((float)temperature)+" C ";
     String ss="soil: "+String((int)sMValue) +".";
     String ssp="soilp: "+String((int) soilmoisturepercent)+"%";
     String lums="lum: "+String((int)lightVal)+". ";
     String ppms="co2: "+String((float)correctedPPM)+" ppm ";
       
// // UBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  if (client.publish(temperature_topic, String(temperature).c_str())) {
       Serial.println("Temperature sent!");
   }
   else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
     client.connect(clientID, mqtt_username, mqtt_password);
     delay(10); 
     client.publish(temperature_topic, String(temperature).c_str());
   }


   if (client.publish(humidity_topic, String(humidity).c_str())) {
    Serial.println("Humidity sent!");  }
  
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
   client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
   client.publish(humidity_topic, String(humidity).c_str());
  }

 if (client.publish(soil_topic, String(soilmoisturepercent ).c_str())) {
       Serial.println("Soil sent!");}
  
 else {
     Serial.println("soil failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
     delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
     client.publish(soil_topic, String(soilmoisturepercent ).c_str());}


 if (client.publish(lum_topic, String(lightVal).c_str())) {
     Serial.println("lum sent!"); }
  
  else {
     Serial.println("lum failed to send. Reconnecting to MQTT Broker and trying again");
     client.connect(clientID, mqtt_username, mqtt_password);
  delay(10); 
     client.publish(lum_topic, String(lightVal).c_str());}


 if (client.publish(CO2_topic, String(correctedPPM).c_str())) {
     Serial.println("co2 sent!");   }
 
  else {
     Serial.println("co2 failed to send. Reconnecting to MQTT Broker and trying again");
     client.connect(clientID, mqtt_username, mqtt_password);
     delay(10); 
     client.publish(CO2_topic, String(correctedPPM).c_str());
    }

  client.disconnect();  // disconnect from the MQTT broker
 
    delay(2000);
}