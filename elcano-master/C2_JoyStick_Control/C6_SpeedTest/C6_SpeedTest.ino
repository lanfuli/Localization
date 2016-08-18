#include <Elcano_Serial.h>

SerialData Results;
long randNumber;

void setup() {
  // put your setup code here, to run once:
  #define MEGA  

  Serial.begin(9600);
  Serial2.begin(9600);

  // Set random seed
  randomSeed(analogRead(0));
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // print a random number from 10 to 19
  randNumber = random(10, 50);
  
  Serial.println(randNumber);
  Results.Clear();
  Results.number = MSG_SENSOR;
  Results.speed_cmPs = randNumber;
  
  Serial.print("Sensing data to C6:: " );
  Serial.print("speed_cmPs=" );
  Serial.print(Results.speed_cmPs);
  Serial.println("");
  
  writeSerial(&Serial2,&Results);
  delay(500);  
   
}

