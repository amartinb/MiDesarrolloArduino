/*
  Temporizador aleatorio de n canales
  Activa relés en base a una progración con rango aleatorio de inicio y fin

  created Jul 2015
  by Alfonso Martin
 */
 
#include <Wire.h>
#include "RTClib.h"

int wHoraInicio, wMinutoInicio, wHoraFinal, wMinutoFinal, wActivo;
int wHoraActual, wMinutoActual;
int wAccion ; // 0-No accion, 1-Encender, 2-Apagar

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output as monitor.
  pinMode(13, OUTPUT);
  
  // para los relés
  #define RELAY1  6   
  pinMode(RELAY1, OUTPUT);
  maneja_rele(2);
  


}

void maneja_rele(int wAccion){
  if (wAccion == 1){
    //Serial.println("Enciendo Rele");
    digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
  } else{
    digitalWrite(RELAY1,HIGH);          // Turns OFF Relays 1
    //Serial.println("Apago Rele");
  }
}
// the loop function runs over and over again forever
void loop() {
  
  delay(3000);
  maneja_rele(1);
  delay(2000);
  maneja_rele(2);

}
