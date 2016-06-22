/*
  Temporizador aleatorio de n canales
  Activa relés en base a una progración con rango aleatorio de inicio y fin

  created Jul 2015
  by Alfonso Martin
 */
 
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
int wHoraInicio, wMinutoInicio, wHoraFinal, wMinutoFinal, wActivo;
int wHoraActual, wMinutoActual;
int wAccion ; // 0-No accion, 1-Encender, 2-Apagar
DateTime now;

// the setup function runs once when you press reset or power the board
void setup() {
  randomSeed(analogRead(0));
  // initialize digital pin 13 as an output as monitor.
  pinMode(13, OUTPUT);
  
  // para los relés
  #define RELAY1  6   
  #define RELAY2  7
  pinMode(RELAY1, OUTPUT);
  // Testeo rele
  Serial.println("TEST: Enciendo Rele");
  digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
  delay(2000);
  Serial.println("Apago Rele");
    digitalWrite(RELAY1,HIGH);           // Turns Off Relays 1
  maneja_rele(2);
  
  //Para conectar con el reloj
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
 //Si quitamos el comentario de la linea siguiente, se ajusta la hora a la que queramos
 //RTC.adjust(DateTime(2015,7,8,19,23));
 //Si ponemos la siguiente coge la del ordenador
 RTC.adjust(DateTime(__DATE__, __TIME__));
 leeFechaSerial();
 

 //Inicializar pila
 wActivo = 0;
 wHoraInicio = 19;
 wMinutoInicio = random(1, 10);
 wHoraFinal = 19;
 wMinutoFinal = wMinutoInicio+random(10, 20);

  Serial.println("Hora Inicio---Hora Actual---Hora Final");
   Serial.print(wHoraInicio, DEC);
  Serial.print(":");
  Serial.print(wMinutoInicio, DEC);
  Serial.print("---");
  Serial.print(wHoraActual, DEC);
  Serial.print(":");
  Serial.print(wMinutoActual, DEC);
  Serial.print("---");
  Serial.print(wHoraFinal, DEC);
  Serial.print(":");
  Serial.print(wMinutoFinal, DEC);
  Serial.println();

}

void leeFechaSerial(){
  DateTime now = RTC.now();
 //Imprimimos el dia
 Serial.print("Fecha Actual");
 Serial.print(now.day(), DEC);
 Serial.print('/');
 //Imprimimos el mes
 Serial.print(now.month(), DEC);
 Serial.print('/');
 //Imprimimos el año
 Serial.print(now.year(), DEC);
 Serial.print(' ');
 //Imprimimos la hora
 wHoraActual=now.hour();
 Serial.print(wHoraActual, DEC);
 Serial.print(':');
 //Imprimimos los minutos
 wMinutoActual=now.minute();
 Serial.print(wMinutoActual, DEC);
 Serial.print(':');
 //Imprimimos los segundos
 Serial.print(now.second(), DEC);
 Serial.println();
 
}
void maneja_rele(int wAccion){
  if (wAccion == 1){
    Serial.println("Enciendo Rele");
    digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
  } else{
    digitalWrite(RELAY1,HIGH);          // Turns OFF Relays 1
    Serial.println("Apago Rele");
  }
}
// the loop function runs over and over again forever
void loop() {
  
  //leeFechaSerial();

  // Estructura de control del Temporizador
  //revisar pila
  DateTime now = RTC.now();
  wHoraActual=now.hour();
  wMinutoActual=now.minute();
  Serial.println();
  Serial.print(wHoraActual);
  Serial.print(":");
  Serial.print(wMinutoActual);
  wAccion = 0;
  if (wActivo ==0){   
    //No estaría activo el rele, miramos entonces la hora de inicio
    if (wHoraActual == wHoraInicio && wMinutoActual == wMinutoInicio) {
      wAccion = 1;
    }
  } else {
    // Estaría activo el relé o no pero miremos la hora de final
    if (wHoraActual == wHoraFinal && wMinutoActual == wMinutoFinal) {
      wAccion = 2;
    }
  }
  Serial.println("Accion ");
  Serial.print(wAccion);
  //hay acción a realizar?
  if (wAccion > 0){
    if (wAccion == 1){
      maneja_rele(wAccion);
      wActivo =1;
    } else {
      maneja_rele(wAccion);
      wActivo =0;
    }
  }
  
  //  desplazar la pila
  //hay cambio de dia?
  //SI Calcular acciones del dia y poner en la pila

  // Esperamos un rato
  delay(3000);
}
