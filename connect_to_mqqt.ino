//Function to connect to MQTT
void connect_MQTT()
{

   Serial.print("Connecting to ");
   Serial.println(ssid);
    //client.connect() : Connect to the IP address and port specified in the 
    //constructor. The return value indicates success or failure
  if (client.connect(clientID)) {
           Serial.println("Connected to MQTT Broker!");
           }
  else {
         Serial.println("Connection to MQTT Broker failed...");}

}  

 void setup() {
    Serial.begin(9600);
    pinMode(trig, OUTPUT); // Sets the trigPin as an Output
    pinMode(echo, INPUT); // Sets the echoPin as an Input
  
 connect_wifi ();//Call the wifi function to connect to wifi  
}
  
void loop() {
   
  connect_MQTT(); //Call function to connect to MQTT broker   
  Serial.setTimeout(2000);

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  t= pulseIn(echo, HIGH);
  
  // Calculate the distance
  h= t * SOUND_SPEED/2;

 // ******* Pression *********** 
if (! baro.begin()) {
Serial.println("Couldnt find sensor"); }

float P1 = baro.getPressure(); // get pressure in pascal


// Convert data to string because MQTT can only transmit strings 
String Ps="pression: "+String((float)P1)+"Pa";
String ts="water_level: "+String((float) h )+" Cm ";
  
//Publish the pression value
if (client.publish(pression_topic, String(P1).c_str())) {
     Serial.println("Pression sent!");
   }
  //Try to reconnect to MQTT client 
  else {
        Serial.println("Pression sensor failed to send. Reconnecting to MQTT Broker and trying again");
        client.connect(clientID);
         delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        client.publish(pression_topic, String(P1).c_str());
   }



if (client.publish( Water_level_topic, String( h ).c_str())) {
       Serial.println("Water_level sent!");
   }
   else {
     Serial.println("Water_level failed to send. Reconnecting to MQTT Broker and trying again");
     client.connect(clientID);
     delay(10); 
     client.publish(Water_level_topic, String(h).c_str());
  }

    delay(2000);

}