// ARDUTRIP
// Amartinb, 2015

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <SoftwareSerial.h>  // Para comunicarme con el GPS

SoftwareSerial mySerial(0, 1); // RX, TX 
//0 al TX del GPS
//1 al RX del GPS

// Intento para usar un sólo Arduino. Bloquear TX/RX para poder usar esos pines si es 0
int MOD_PIN_SERIAL = 0;

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

int c_Fdo = BLACK;     // Color de Fondo
int c_Txt = WHITE;     // Color del Texto

/******************* UI details */
/* En rotacion 3 da una idea de lo que cabe por tamaño de letra
 * tft.fillScreen(BLACK);
    pintaString("ABCDEFGHIJKL",4,10,10);
    pintaString("ABCDEFGHIJKLMNOPQ",3,10,50);
    pintaString("ABCDEFGHIJKLMNOPQRSTUVWXY",2,10,70);
    pintaString("ABCDEFGHIJKLMNOPQRSTUVWXYABCDEFGHIJKLMNOPQRSTUVWXY",1,10,90);
    pintaString("ABCDEFGHIJ",5,10,110);
    pintaString("ABCDEFGH",6,10,150);
    pintaString("ABCDEFG",7,10,180);
    delay(1000);
    tft.fillScreen(BLACK);
    pintaString("12",12,10,10);
 */

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define STATUS_X 20
#define STATUS_Y 220

// Campos y variables para la trama GPS
String cadena;
String campo1;
String campo2;
String campo3;
String campo4;
String campo5;
String campo6;
String campo7;
String campo8;
String campo9;
String campo10;
String campo11;
String campo12;
String campo13;
String campo14;

String GPS_status;          // OK/Void
String Lat;                 // Latitud
String Lon;                 // Longitud
String UTC;                 // Fecha y hora UTC

int contador = 0;
int wCont = 0;
String vCad;        //Para informar del avance
int wGPS_OK = 0;    //Contador para tramas OK. En la inicialización, con 3 seguidas, out
int wOp = 0;        //Opción elegida del menú
int wLog = 0;       //Log en SD=1
int wx = 0;         //pos x pulsada
int wy = 0;         //pos y pulsada
String wCad = "";   //Cadenas Temporales
String wCad2 = "";

#define MINPRESSURE 10
#define MAXPRESSURE 1000


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

Adafruit_GFX_Button buttons[15];
/* create 15 buttons, in classic candybar phone style */
char buttonlabels[15][5] = {"Send", "Clr", "End", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#" };
uint16_t buttoncolors[15] = {ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_ORANGE, ILI9341_BLUE, ILI9341_ORANGE};
                             


int leeTramaGPS(){
  // Lee una trama de GPS que sea GPGGA o GPRMC (inicialmente GGA) y la valida (0-OK) 
  int cCad = 0;      //Contador caracteres cadena     
  int pInc = 0;     //Posicion inicial campo
  int wVal = 1;     //Cadena válida = 0 por el momento
  vCad = "Cadena GPS "; //Texto auxiliar ELIMINABLE
  int pCad = 1;       //Indice de campo en la trama
  
 
    cadena = leeGPS();
//    pintaString(String(cadena),1,20,140);
    // Tratamiento de cadena recibida
    // Inicializo vars auxiliares
    wVal=0;
    cCad=0;
    pCad=1;
    pInc=0;
    while (cCad<cadena.length()&&wVal==0){
      wCad2=cadena.substring(cCad,cCad+1);    
//      Serial.println(wCad2);            
      if (wCad2==","){
          wCad=cadena.substring(pInc,cCad);
//          esperaPulsacion();
//          Serial.println(wCad);         
          switch (pCad){
            case 1:
//              pintaString(wCad,1,180,100);
              if (wCad == "GPGGA" || wCad == "GPRMC") { 
                  campo1 = wCad;
//                  pintaString(campo1,1,20,160);
                  wVal = 0;
                  pCad++;
                    } else {
                      wVal = 1;
                    }
              break;
            case 2:   //En GGA y RMC la hora UTC
              campo2 = wCad;
//              pintaString(campo2,1,20,160);
              wVal = 0;
              pCad++;
                break;
            case 3:   //En GGA latitud. En RMC Activo/Void
              campo3 = wCad;
                  wVal=0;
//              pintaString(campo3,1,20,160);
                  if (campo1 == "GPRMC"){
                      if (campo3!="A"){
                        wVal = 1;
                      } 
                    }
              pCad++;
                break;
             case 4:  //En GGA N o S pero siendo realista, N. En RMC Latitud
              campo4 = wCad;
                  if(campo1 == "GPGGA"){
                    if (campo4 == "N"||campo4 == "S"){
                          wVal=0;
                      } else {
                          wVal=1;
                      }
                  }
//              pintaString(campo4,1,20,160);
              pCad++;
              break;
            case 5:   //En RMC N o S
              campo5 = wCad;
                    wVal=0;
//              pintaString(campo5,1,20,160);
                    if (campo1 == "GPRMC") {
                      if (campo5=="N"||campo5=="S"){
                        wVal = 0;
                      } else {
                        wVal = 1;
                      }
                    }
              pCad++;
              break;
            case 6:   //en GGA W o E
              campo6 = wCad;
                  if(campo1 == "GPGGA"){
                    if (campo6 == "W" || campo6 == "E"){
                      wVal=0;
                      } else {
                          wVal=1;
                      }
                  }
//                pintaString(campo6,1,20,160);
              pCad++;
                break;
            case 7:
              campo7 = wCad;
                      wVal = 0;
                      if (campo1 == "GPGGA"){
                        if (campo7 == "0"){    //Trama inválida si es 0
                    wVal=1;
                  } else {
                    wVal = 0; 
                        }
                      }
                      if (campo1 == "GPRMC"){
                        if (campo7 == "W" || campo7 == "E"){
                          wVal=0;
                        } else {
                          wVal=1;
                        }
                      }
//              pintaString(campo7,1,20,160);
              pCad++;
                break;
            case 8:
              campo8 = wCad;
//              pintaString(campo8,1,20,160);
                    wVal = 0;
                pCad++;
                break;
            case 9:
              campo9 = wCad;
//              pintaString(campo9,1,20,160);
              wVal = 0;
              pCad++;
                break;
            case 10:
              campo10 = wCad;
//              pintaString(campo10,1,20,160);
              wVal = 0;
              pCad++;
                break;
            case 11:
              campo11 = wCad;
//              pintaString(campo11,1,20,160);
              wVal = 0;
              pCad++;
                break;
            case 12:
              campo12 = wCad;
//              pintaString(campo12,1,20,160);
              wVal = 0;
              pCad++;
                break;
            case 13:
              campo13 = wCad;
//              pintaString(campo13,1,20,160);
              wVal = 0;
              pCad++;
                break;
            case 14:
              campo14 = wCad;
//              pintaString(campo14,1,20,160);
              wVal = 0;
              pCad++;
                break;
            default:
                wVal = 0;
            break;
          }
          if (wVal == 1){
            pintaString(String("Trama No valida"),1,20,160);
            delay(100);
            }
          cCad++;
          pInc=cCad;
      } else
        {
          cCad++;
        }
  }
  return(wVal);     //GPS OK
}


int activaGPS(){
  int resultado = 0;
  wCont = 0;
  wGPS_OK=0;
  while (wCont<20 && wGPS_OK<3){
   wCont++; 
   pintaString(String(wCont),1,5,160);
   resultado=leeTramaGPS();
   if (resultado==0) {
      wGPS_OK++;
    } else {
      wGPS_OK=0;
    }
  }
  if (wGPS_OK==3){
    GPS_status = "OK";
    if (campo1=="GPGGA"){
      Lat=campo3+campo4;
      Lon=campo5+campo6;
      UTC=campo2;
    } else {
      Lat=campo4+campo5;
      Lon=campo6+campo7;
      UTC=campo10+" "+campo2;
    }
    status("GPS iniciado. Pulsa una tecla");
    return(0);
  } else {
    GPS_status = "Void";
    Lat="XXXXX";
    Lon="XXXXX";
    UTC="XXXXX";
    status("GPS no activado. Pulsa una tecla");
    return(1);
  }
}

String leeGPS() {
  // leo GPRMC y GPRMA que parece que serán los interesantes
contador = 0;
while(contador<10){ 
 if (mySerial.available()){
    cadena = mySerial.readStringUntil('$');
    if (cadena.startsWith("GPGGA")||cadena.startsWith("GPRMC")){
      return(cadena);
    } 
 }
 contador++;
}
return("No valida");
}

// Print something in the mini status bar with either flashstring
void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_WHITE);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.print(msg);
  esperaPulsacion();
}
// or charstring
void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_WHITE);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.print(msg);
  esperaPulsacion();
}
void pinta(char *msg, int tsize, int pos_x, int pos_y) {
  tft.fillRect(pos_x, pos_y, 240, tsize*8, c_Fdo);
  tft.setCursor(pos_x, pos_y);
  tft.setTextColor(c_Txt);
  tft.setTextSize(tsize);
  tft.print(msg);
}
void pintaInv(char *msg, int tsize, int pos_x, int pos_y) {
  tft.fillRect(pos_x, pos_y, 240, tsize*8, c_Txt);
  tft.setCursor(pos_x, pos_y);
  tft.setTextColor(c_Fdo);
  tft.setTextSize(tsize);
  tft.print(msg);
}
void pintaString(String msg, int tsize, int pos_x, int pos_y) {
  tft.fillRect(pos_x, pos_y, 240, tsize*8, c_Fdo);
  tft.setCursor(pos_x, pos_y);
  tft.setTextColor(c_Txt);
  tft.setTextSize(tsize);
  tft.print(msg);
}

void esperaPulsacion(void) {
  int ws = 0;
  wx=0;
  wy=0;
  while (ws<10000){
    TSPoint p = ts.getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    wx=p.x;
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    wy=p.y;
    ws = 1000001;
   }
   ws++;
  }
/*   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<15; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b=0; b<15; b++) {
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }
    
    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!
        
        // if a numberpad button, append the relevant # to the textfield
        if (b >= 3) {
          if (textfield_i < TEXT_LEN) {
            textfield[textfield_i] = buttonlabels[b][0];
            textfield_i++;
      textfield[textfield_i] = 0; // zero terminate
            
           // fona.playDTMF(buttonlabels[b][0]);
          }
        }

        // clr button! delete char
        if (b == 1) {
          
          textfield[textfield_i] = 0;
          if (textfield > 0) {
            textfield_i--;
            textfield[textfield_i] = ' ';
          }
        }

        // update the current text field
        Serial.println(textfield);
        tft.setCursor(TEXT_X + 2, TEXT_Y+10);
        tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(textfield);

        // its always OK to just hang up
        if (b == 2) {
          status(F("Hanging up"));
          //fona.hangUp();
        }
        // we dont really check that the text field makes sense
        // just try to call
        if (b == 0) {
          status(F("Calling"));
          Serial.print("Calling "); Serial.print(textfield);
          
          //fona.callPhone(textfield);
        }
        
      delay(100); // UI debouncing
    }
  }
 */ 
}



int pintaMenu(){
  //Pinta el menú principal y devuelve opción pulsada
  wOp=0;
  //Borra y prepara pantalla
  tft.fillScreen(c_Fdo);
  pinta("Ardutrip. Menu",3,10,20);
  pinta("--------------",3,10,45);
  pinta("Navegacion",3,10,80);
  pinta("GPS",3,10,120);
  pinta("Opciones",3,10,160);
  esperaPulsacion();
  if(wx>0||wy>0){
/*    wCad="Pulsado en ";
    wCad=wCad+wx+":"+wy;
    pintaString(wCad,1,50,200);  */
    if (wx>75 &&wx<140){wOp=1;}
    if (wx>140&&wx<195){wOp=2;}
    if (wx>195&&wx<240){wOp=3;}
  }
//    pintaString(String(wOp),1,180,180);
//     esperaPulsacion();
  return(wOp);
}

void setup(void) {
  // Inicia GPS
  mySerial.begin(9600);
  c_Txt = WHITE;
  c_Fdo = BLACK;

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier==0x0101)
      identifier=0x9341;


  tft.begin(identifier);
  tft.fillScreen(c_Fdo);
  
  // create buttons
 /*
  for (uint8_t row=0; row<5; row++) {
    for (uint8_t col=0; col<3; col++) {
      buttons[col + row*3].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, ILI9341_WHITE, buttoncolors[col+row*3], ILI9341_WHITE,
                  buttonlabels[col + row*3], BUTTON_TEXTSIZE); 
      buttons[col + row*3].drawButton();
    }
  }
  
  // create 'text field'
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, ILI9341_WHITE);
*/
// Cuatro rectángulos en cada esquina
  /* tft.drawRect(0,0,10,10,ILI9341_WHITE);
  tft.drawRect(230,0,10,10,ILI9341_BLUE);
  tft.drawRect(0,310,10,10,ILI9341_RED);
  tft.drawRect(230,310,10,10,ILI9341_GREEN);  */
  //  tft.fillScreen(BLACK);

  
    tft.setRotation(3);
    pintaString("Ardutrip",4,80,80);
    pinta("Vers. 0.1",2,110,150);
    pinta("Pulsa para continuar",1,105,210);
    esperaPulsacion();
    delay(100);
    tft.fillScreen(c_Fdo);
    pinta("Iniciando ..",3,70,80);
    delay(200);
    activaGPS();

 
}
void loop(void) {
  
  // Pinta Menu
  wOp=pintaMenu();
  switch (wOp){
    case 1:
      tft.fillScreen(c_Fdo);
      pinta("Navegacion",3,10,10);
      esperaPulsacion();
      break;
    case 2:
      tft.fillScreen(c_Fdo);
      pinta("GPS",3,10,10);
      if (GPS_status == "OK"){
        //Pinta los valores
        pinta("Coords",2,10,50);
        pinta("UTC",2,10,100);
        pintaString(Lat,2,100,50);
        pintaString(Lon,2,100,70);
        pintaString(UTC,2,100,100);
      } else { 
        pinta("GPS no sincronizado",2,10,50);
        pinta("Sincronizando ...",2,70,80);
        activaGPS();    
      }      
      pintaInv("      Volver",2,1,220);
      pintaInv("Dump",2,180,220);
      wy=0;
      while(wy==0){
        esperaPulsacion();
      }
      if (wy<80){
        tft.fillScreen(c_Fdo);
        pinta("DUMP GPS",3,10,10);
        for(wCont=0; wCont<9; wCont++){
          leeTramaGPS();
          pintaString(String(cadena),1,10,wCont*20+40);
        }
        status(" Pulsa una tecla ");
      }
      break;
    case 3:
      tft.fillScreen(c_Fdo);
      pinta("Opciones",3,10,10);
      esperaPulsacion();
      break;
  }
      
}







