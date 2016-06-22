// Programa del Arduino 2 (receptor) Esclavo
// En mi caso el Arduino 2 es el Nano v3 alimentado desde el arduino maestro
int pin9 = 9;        // pin 9 conectado a (+)led(-)+resistencia+GND
int wblink = 0;
int i = 0;

void setup() 
{
  Serial.begin(9600);         // Inicio el puerto serie a 9600 baudios
  pinMode(pin9, OUTPUT);      // pin 9 (led) modo SALIDA
  digitalWrite(pin9, HIGH);   // pin 9 (led) nivel ALTO (encendido)
}

void loop() 
{
  if (wblink ==1) {
    for (i = 1; i<10; i++){
    digitalWrite(pin9,HIGH);
    delay (100);
    digitalWrite(pin9,LOW);
    }
  }
  if (Serial.available()) {            // lee si al puerto Rx le llegan datos
    int byte_entrada = Serial.read();  // almaceno en byte_entrada el dato que llega al puerto
    if (byte_entrada == 'a')           // Si es una 'a'
    {
      digitalWrite(pin9, LOW);         // Apago el led 9 (nivel BAJO)
      wblink = 0;
    }
    if (byte_entrada == 'b')
    {
      wblink = 1;
    }
    if (byte_entrada == 'o')           // Si es una 'o'
    {
      digitalWrite(pin9, HIGH);        // Enciendo el led 9 (nivel ALTO)
      wblink = 0;
    }
  }
}
    
