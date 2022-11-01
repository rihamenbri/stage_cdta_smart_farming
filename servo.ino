#include <Servo.h>

Servo myservo;  

int pos = 10;   
void setup() {
  pinMode(3, OUTPUT);
  myservo.attach(3);  }

void loop() {
  
  for (pos = 110; pos >= 10; pos -= 1){ 
    myservo.write(pos);              
    delay(50);}
    
  for (pos = 10; pos <= 110; pos += 1) {
    myservo.write(pos);              
    delay(50);                      
  }
    delay(600);                   
}