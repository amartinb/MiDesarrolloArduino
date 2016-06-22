// Programa del Arduino 1 (transmisor) Maestro
// En mi caso el Arduino 1 es el Nano v3 conectado al usb
const int boton2 = 2;      // pin 2 conectado al pulsador (conectado a masa y una resistencia+5v)
const int led9 =  9;       // pin 9 conectado a un led + resistencia +GND

// variable que cambiara:
int estadoBoton = 0;       // variable para ver el estado del boton 
int contador = 0;          // cuando sea 2 paso b

void setup() 
{
  Serial.begin(9600);      // Inicia la velocidad de transmision para Tx y Rx
  pinMode(led9, OUTPUT);   // pin 9 (led) modo SALIDA 
  pinMode(boton2, INPUT);  // pin 2 (boton) modo ENTRADA   
}

void loop()
{
  // lee si el pulsador esta suelto 0 o pulsado 1
  estadoBoton = digitalRead(boton2);

  if (estadoBoton == HIGH) 
  {                              // si el boton ha sido pulsado:    
    digitalWrite(led9, HIGH);    // encendemos el led
    if (contador == 2){
      Serial.print('b');          // envío 'b' para blinking
      contador = 0;
      } else
      {
        Serial.print('a');           // Enviamos 'a' por Tx (puerto serie)   
        contador += 1;
    }
    
    delay(500);                  // Retardo de 0,5 s
  } 
  else 
  {                              // si el boton no ha sido pulsado
    digitalWrite(led9, LOW);     // apagamos el led
    Serial.print('o');           // Enviamos 'o' (OFF) por Tx
    delay(500);                  // Retardo de 0,5s
  }
}    
  
