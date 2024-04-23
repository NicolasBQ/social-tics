#include "LiquidCrystal.h"
#include <Keypad.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define RELAY1  8  //salida humedad
#define RELAY2  9  //salida temperatura

const int pinDatosDQ = 10;

const int AirValue = 904;   //you need to replace this value with Value_1
const int WaterValue = 822;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);


unsigned long previousMillis = 0;
const long interval = 10000;  // Intervalo de tiempo en milisegundos (10 segundos)


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {36, 34, 32, 30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28, 26, 24, 22}; //connect to the column pinouts of the keypad

byte arbolari[8]   = {B00000, B00001, B00011, B00101, B01111, B11011, B11111, B11111}; // Árbol
byte arbolard[8]   = {B00000, B10000, B11000, B11100, B11110, B10111, B11101, B11111};
byte arbolabi[8]   = {B01111, B00111, B00001, B00001, B00001, B00001, B00011, B11111};
byte arbolabd[8]   = {B11110, B11100, B10000, B10000, B10000, B10000, B11000, B11110};

byte arbollogoari[8]   = {B00000, B11000, B11001, B00101, B00011, B11001, B11001, B00101}; // Árbol Logo
byte arbollogoard[8]   = {B00110, B00110, B11000, B10000, B10011, B10011, B10100, B11000};
byte arbollogoabi[8]   = {B11011, B11001, B00101, B00011, B00001, B00111, B01111, B11111};
byte arbollogoabd[8]   = {B10000, B10110, B10110, B11000, B10000, B11100, B11110, B11111};

byte volver[8]   = {B00001, B00001, B00001, B00101, B01111, B11111, B01111, B00100};

byte termometro[8]   = {B00110, B00100, B00110, B00100, B01010, B11111, B11111, B01110}; //imagen termómetro activación calefacción
byte llave[8]   = {B11100, B01000, B11111, B00000, B01000, B11100, B11100, B01000}; //imagen llave activación riego


byte up[8]   = {B00100, B01110, B11111, B01110, B01110, B01110, B01110, B0000};
byte down[8]   = {B01110, B01110, B01110, B01110, B11111, B01110, B00100, B00000};




int temperatura=0;
int humedad=0;

String temp;
String hum;
String sum;



// initialize the library by providing the nuber of pins to it
//LiquidCrystal lcd(6,7,2,3,4,5);
LiquidCrystal lcd(2,3,4,5,6,7);
Keypad cKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {

//lcd.createChar(0, arbolari);
//lcd.createChar(1, arbolard);
//lcd.createChar(2, arbolabi);
//lcd.createChar(3, arbolabd);
lcd.createChar(4, up);
lcd.createChar(5, down);
lcd.createChar(6, termometro);
lcd.createChar(7, llave);

lcd.createChar(0, arbollogoari);
lcd.createChar(1, arbollogoard);
lcd.createChar(2, arbollogoabi);
lcd.createChar(3, arbollogoabd);



pinMode(RELAY1, OUTPUT);   
pinMode(RELAY2, OUTPUT);   
digitalWrite(RELAY1,1);
digitalWrite(RELAY2,1);

Serial.begin(9600);
Serial1.begin(115200);  // Configura Serial1 para comunicación con el ESP-01
delay(1000);
sensorDS18B20.begin();
lcd.begin(16,2);
// set cursor position to start of first line on the LCD
lcdMenuInit();

  Serial1.println("AT+CWMODE=1");
  delay(1000);
  Serial1.println("AT+CWJAP=\"Galaxy-S9+fcf8\",\"Prueba123\"");
  delay(5000);

  // Solicita y muestra la dirección IP asignada al módulo
  Serial1.println("AT+CIFSR");


}

void loop()
{


if (Serial1.available()) {
    Serial.write(Serial1.read());
  }

//soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
//Serial.println(soilMoistureValue);
//delay(1000);

char tecla = cKeypad.getKey();  

if(tecla=='D'){  
    lcdMenu();
 }
}


void lcdMenu(){
    
    int posDisp=0, opcMenu=0;
    char tecla;
    lcd.clear();


    do{
        tecla = cKeypad.getKey();  
        if(tecla=='A'){
          lcd.clear();
          dispChange(&posDisp);
        }
        
        if(posDisp==0){
          lcd.setCursor(0, 0); lcd.print("1.Temperatura");
          lcd.setCursor(0, 1); lcd.print("2.Humedad");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(5));
          
          switch(tecla){
            case '1': lcdMenu1();
                    break;
            case '2': lcdMenu2();
                    break;
          }

          
        }
        else if(posDisp==1){
          
          lcd.setCursor(0, 0); lcd.print("3.Iniciar");
          lcd.setCursor(0, 1); lcd.print("4.Volver");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(4));
          switch(tecla){
            case '3': lcdIniciar();
                    break;
            case '4': opcMenu=1;
                    break;
          }

          
        }

      
      }
    while(opcMenu != 1);
    lcdMenuInit();    
    


  
  }


 


//FUNCIÓN MENÚ TEMPERATURA
void lcdMenu1(){
    int posDisp=0, opcMenu=0;
    char tecla;  
    lcd.clear();


    do{
        tecla = cKeypad.getKey();  
        if(tecla=='A'){
          lcd.clear();
          dispChange(&posDisp);
        }
        
        if(posDisp==0){
          lcd.setCursor(0, 0); lcd.print("1.Ajuste Temp.");
          lcd.setCursor(0, 1); lcd.print("2.Tem. Actual");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(5));
          
          switch(tecla){
            case '1': lcdMenuTemp();
                    break;
            case '2': lcdleerTemp();
                    break;
          }

          
        }
        else if(posDisp==1){
          
          lcd.setCursor(0, 0); lcd.print("3.Volver");
          //lcd.setCursor(0, 1); lcd.print("4.Volver");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(4));
          switch(tecla){
            case '3': opcMenu=1;
                    break;
            case '4': 
                    break;
          }

          
        }

      
      }
    while(opcMenu != 1);
  }


void lcdMenu2(){
    int posDisp=0, opcMenu=0;
    char tecla;
    lcd.clear();


    do{
        tecla = cKeypad.getKey();  
        if(tecla=='A'){
          lcd.clear();
          dispChange(&posDisp);
        }
        
        if(posDisp==0){
          lcd.setCursor(0, 0); lcd.print("1.Ajuste Hum.");
          lcd.setCursor(0, 1); lcd.print("2.Hum. Actual");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(5));
          
          switch(tecla){
            case '1': lcdMenuHum();
                    break;
            case '2': lcdleerHum();
                    break;
          }

          
        }
        else if(posDisp==1){
          
          lcd.setCursor(0, 0); lcd.print("3.Volver");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(4));
          switch(tecla){
            case '3': opcMenu=1;
                    break;
            case '4': 
                    break;
          }

          
        }

      
      }
    while(opcMenu != 1);
  }







void lcdleerTemp(){
    bool flag=true;
    char tecla;  
    lcd.clear();
    float tempReal=0;
    tempReal= leerTemp();


    do{
          tecla = cKeypad.getKey();
          lcd.setCursor(0, 0); lcd.print("T.Real");
          lcd.setCursor(7, 0); lcd.print("Obj.");
          lcd.setCursor(0,1);  lcd.print((int)tempReal); lcd.print("C");lcd.setCursor(7,1); lcd.print(temperatura); lcd.print("C");
          lcd.setCursor(14, 0);lcd.print("D");lcd.print("<");
          if(tecla=='D') {flag=false;}
      
      }
    while(flag);
    lcd.clear();
  }


void lcdleerHum(){
    bool flag=true;
    char tecla;  
    lcd.clear();
    float humedadReal=0;
    humedadReal=leerHum();




    do{
          tecla = cKeypad.getKey();
          lcd.setCursor(0, 0); lcd.print("H.Real");
          lcd.setCursor(7, 0); lcd.print("Obj.");
          lcd.setCursor(0,1);  lcd.print((int)humedadReal); lcd.print("%");lcd.setCursor(7,1); lcd.print(humedad); lcd.print("%");  
          lcd.setCursor(14, 0);lcd.print("D");lcd.print("<");
          if(tecla=='D') {flag=false;}
      }
    while(flag);
    lcd.clear();
  }








  

//FUNCIÓN MENÚ HUMEDAD
void lcdMenuHum(){
  int humInt;
  int digito=0;
  int opcMenu=0;
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("D para confirmar ");
  lcd.setCursor(8,0);
  lcd.print("%");
  lcd.setCursor(0,0);
  lcd.print("Hum.= ");
  do{
    char tecla = cKeypad.getKey();  
    
    lcd.cursor();
    if(tecla!=0){
      switch(digito){
      case 0: lcd.print(tecla);
              humedad=((int)tecla-48)*10;
              digito+=1;
              break;
      case 1: lcd.print(tecla);
              humedad=humedad+((int)tecla-48);
              digito+=1;
              break;
      case 2: if(tecla=='D'){
              opcMenu=1;
              }
              break;
              
      }
    
    
    lcd.noCursor();
    }
    
    }
  while(opcMenu==0);

   lcd.clear();
    }



void lcdMenuTemp(){
  int digito=0;
  int opcMenu=0;
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("D para confirmar ");
  lcd.setCursor(0,0);
  lcd.print("Temp.= ");
  do{
    char tecla = cKeypad.getKey();  
    
    lcd.cursor();
    if(tecla!=0){
      switch(digito){
      case 0: lcd.print(tecla);
              temperatura=((int)tecla-48)*10;
              digito+=1;
              break;
      case 1: lcd.print(tecla);
              temperatura=temperatura+((int)tecla-48);
              digito+=1;
              break;
      case 2: if(tecla=='D'){
              opcMenu=1;
              }
              break;
              
      }
    
    
    lcd.noCursor();
    }
    
    }
  while(opcMenu==0);

   lcd.clear();
  
  }


void lcdIniciar(){
  
  lcd.clear();
  bool flag = true, flagtemp=false, flaghum=false;
  int posDisp=0, opcMenu=0;
  

  do{

    float humedadReal=0;
    float tempReal=0;
    char tecla = cKeypad.getKey();
    humedadReal=leerHum();
    tempReal= leerTemp();
    temp=tempReal;
    hum=humedadReal;
    sum=hum + " " + temp; 

    //Serial.println(sum); //lectura analógica
    


    lcd.clear();


    if(tecla=='A'){
          lcd.clear();
          dispChange(&posDisp);
          }

    if(posDisp==0){
          lcd.clear();
          lcd.setCursor(0, 0); lcd.print("T.Real");
          lcd.setCursor(7, 0); lcd.print("Obj.");
          lcd.setCursor(0,1);  lcd.print((int)tempReal); lcd.print("C");lcd.setCursor(7,1); lcd.print(temperatura); lcd.print("C");
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(5));
          lcd.setCursor(14, 0);lcd.print("D");lcd.print("<");
           if(flagtemp){
            lcd.setCursor(12, 0); lcd.write(byte(6));
            }
           
          }
          else if(posDisp==1){
          lcd.clear();
          lcd.setCursor(0, 0); lcd.print("H.Real");
          lcd.setCursor(7, 0); lcd.print("Obj.");
          lcd.setCursor(0,1);  lcd.print((int)humedadReal); lcd.print("%");lcd.setCursor(7,1); lcd.print(humedad); lcd.print("%");  
          lcd.setCursor(14, 1);lcd.print("A");lcd.write(byte(4));
          lcd.setCursor(14, 0);lcd.print("D");lcd.print("<");
          if(flaghum){
            lcd.setCursor(12, 0); lcd.write(byte(7));
            }

          
        }

    
    
    
    //lcd.setCursor(0,1);
    //lcd.print(tempReal); lcd.print(" "); lcd.print(temperatura);  

      if(humedadReal>(humedad+5)){
      digitalWrite(RELAY1,1);
      flaghum=false;
      }
      if(humedadReal<(humedad-5)){
      digitalWrite(RELAY1,0);
      flaghum=true;
      }

      if(tempReal>(temperatura+1)){
      digitalWrite(RELAY2,1);
      flagtemp=false;
      }
      if(tempReal<(temperatura-1)){
      digitalWrite(RELAY2,0);
      flagtemp=true;
      }


      if(tecla=='D'){
      flag=false;
      digitalWrite(RELAY1,1);
      digitalWrite(RELAY2,1);
      }
      
      if (millis() - previousMillis >= interval) {
      sendDataToServer(tempReal, humedadReal);
      previousMillis = millis();
    }
    if (Serial1.available()) {
      Serial.write(Serial1.read());
    }

     
    }
  while(flag);

  lcd.clear();

  
  }



//FUNCIÓN LECTURA HUMEDAD
float leerHum(){
    
    soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
    //Serial.println(soilMoistureValue);
    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
    int hum=analogRead(A1);
    float val = 0;
    if(soilmoisturepercent >= 100)
    {
    val=100;
    }
    else if(soilmoisturepercent <=0)
    {
    val=0;
    }
    else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
    {
    val=soilmoisturepercent; 
    }

    return val;
    //return soilMoistureValue;
  }

//FUNCIÓN LECTURA TEMPERATURA 
float leerTemp(){
    //analogReference(INTERNAL1V1);
    sensorDS18B20.requestTemperatures();
    float cel = sensorDS18B20.getTempCByIndex(0);
    //analogReference(DEFAULT);
    return cel;
    
  }


//FUNCIÓN MENÚ DE INICIO
void lcdMenuInit(){
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.print(" HomeCrop");
  lcd.setCursor(0,1);
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.print(" D para seguir");
  
  }


//FUNCIÓN CAMBIO DE PANTALLA
void dispChange(int *pos){
    if(*pos==0)
    *pos=1;
  else if(*pos==1)
    *pos=0;
  
  }

void sendDataToServer(float temperatura, float humedad) {
  // Formatea los datos en formato URL codificado
  String data = "temperatura=" + String(temperatura, 2) + "&humedad=" + String(humedad, 2);

  // Realiza la solicitud POST al servidor HTTP
  Serial1.println("AT+CIPSTART=\"TCP\",\"18.221.207.68\",12345");
  delay(1000);
  Serial1.print("AT+CIPSEND=");
  Serial1.println(data.length());
  delay(500);
  Serial1.println(data);
  delay(1000);

  // Cierra la conexión
  Serial1.println("AT+CIPCLOSE");
}

