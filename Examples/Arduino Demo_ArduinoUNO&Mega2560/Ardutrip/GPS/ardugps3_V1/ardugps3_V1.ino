//Test módulo XM37-1612

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX 
//0 al TX del GPS
//1 al RX del GPS
String cadena;

void setup()  
{
  Serial.begin(9600);
  while (!Serial) { ; } 
  Serial.println("START");
  mySerial.begin(9600);
}

void loop() 
{
// Pinto GPRMC y GPRMA que parece que serán los interesantes
 
 if (mySerial.available()){
    cadena = mySerial.readStringUntil('$');
    if (cadena.startsWith("GPGGA")||cadena.startsWith("GPRMC")){
//    Serial.write(mySerial.read());     //Esto pintaría todo lo que le viene
    Serial.println(cadena);
    }
 }
  if (Serial.available())
    mySerial.write(Serial.read());
}
