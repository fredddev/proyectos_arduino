#include "SoftwareSerial.h" //wifi
#include <Wire.h> //lcd 16x2
#include <LiquidCrystal_I2C.h> //lcd16x2
#include <SPI.h>//rfid
#include <MFRC522.h>//rfid

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
String tarjetaRfid; //RC522
byte ActualUID[4]; //almacenará el código del Tag leído

char miIp [15] = "192.168.1.12";
//char miRed [6] = "FAMILY";
//char cRed [10] = "mitzi1513";
//char iPSer [15] = "192.168.1.4";
char cmdIp [40] = "AT+CIPSTA=\"192.168.1.12\"\r\n";
char cmdRed[40] =  "AT+CWJAP=\"FAMILY\",\"mitzi1513\"\r\n";
char cmdSer[50] = "AT+CIPSTART=\"TCP\",\"192.168.1.4\",2828\r\n";

SoftwareSerial Serial2(3, 4); // RX, TX //wifi esp8266
LiquidCrystal_I2C lcd(0x27,16,2);  //Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
String msg;
String comando;
byte resp;
byte resp2;

void setup() {
  Serial.begin(4800); //Iniciar el monitor serial
  
  lcd.init();// Inicializar el LCD
  lcd.backlight(); //Encender la luz de fondo.

  Serial2.begin(4800);// iniciar Serial2 para wifi
  while (!Serial2) { // iniciar Serial2 para wifi
    ; // iniciar Serial2 para wifi
  } // iniciar Serial2 para wifi

  SPI.begin();        // Iniciamos  el MFRC522
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  
  
  reconectar();
}

void loop() {
  if ( mfrc522.PICC_IsNewCardPresent()){  //si hay tarjeta en el lector
     procesarTarjeta();
  } 

  if (Serial2.available()){ //si habla el wifi
     msg = Serial2.readString();
     Serial.println(msg);
     procesarMensaje();
  }
}

byte iniciar(){
  lcd.clear();
  lcd.print("Procesando");

  Serial2.write("AT\r\n"); //VER SI ESTA RESPONDIENDO EL WIFI ESP8266
  //imprimir("entrando a respuesta");
  respuestaWifi();
  //imprimir("salio,entro seencuentra");
  resp = seEncuentra();
  //imprimir("fin de seencuentra");
  if (resp==1){
      Serial2.write("AT+CWQAP\r\n"); //desconectar de cualquier red
      respuestaWifi();
      resp = seEncuentra();
      if (resp==1){
          Serial2.write(cmdIp);//establecer ip
          respuestaWifi();
          resp = seEncuentra();
          if (resp==1){
              //imprimir("ip exito");
              Serial2.write(cmdRed); //conectar a la red;
              respuestaWifi();
              respuestaWifi();
              //respuestaWifi();
              resp = seEncuentra();
              if (resp==1){
                 //imprimir("conectado a red");
                 Serial2.write(cmdSer);
                 respuestaWifi();
                 resp = seEncuentra();
                 if (resp==1){
                    //imprimir("conectado a server");
                    mensajeInicio();
                    return(1);
                 }else{
                    imprimirError("err 5");
                    return(0);
                 }
              }else{
                imprimirError("err 4");
                return(0);
              }
              
          }else{
              imprimirError("err 3");
              return(0);
          }
          
      }else{
          imprimirError("err 2");
          return(0);
      }
  }else{
    imprimirError("err 1");
    return(0);
  }
}

void respuestaWifi(){
      byte bandera=0;
      while (bandera == 0){
          if (Serial2.available()){
              delay(100);
              msg = Serial2.readString();
              bandera=1;
          }
      }
      msg.replace("\n", "");
}

byte seEncuentra(){
    Serial.println(msg);
    int pos=msg.indexOf("OK");
    if (pos>=0) {
        Serial.println("exito");
        return 1;
    } else{
      Serial.println("fallo");
      return 0;
    } 
}

 void imprimirError(String cadena){
          lcd.clear();
          lcd.print(cadena);
          delay(3000);
 }


void imprimir(String cadena){//borrar
    lcd.clear();
    lcd.print(cadena);
  }

void reconectar(){
    resp2 = 0;
    while (resp2 == 0){
      resp2 = iniciar();  
    } 
 }  

void procesarTarjeta(){
       if ( mfrc522.PICC_ReadCardSerial()){
             for (byte i = 0; i < mfrc522.uid.size; i++) {
                     //Serial.print(mfrc522.uid.uidByte[i], HEX);   
                     ActualUID[i]=(mfrc522.uid.uidByte[i]);
             } 
             tarjetaRfid =  String(ActualUID[0], HEX) + String(ActualUID[1], HEX) + String(ActualUID[2], HEX) + String(ActualUID[3], HEX);
             Serial.println(tarjetaRfid); 
             // Terminamos la lectura de la tarjeta  actual
             mfrc522.PICC_HaltA();
             enviarServer();
       } 
}
void enviarServer(){
  String stringLength="AT+CIPSEND=";
  stringLength +=String(tarjetaRfid.length());
  stringLength +="\r\n";
  Serial2.print(stringLength); //con este comando le digo cuantos caracteres enviare a servidor...
  respuestaWifi(); 
  resp = seEncuentra();
  if (resp==1){
      Serial2.print(tarjetaRfid+"\r\n"); //enviamos el mensaje al servidor...
  }else{
    //intentar reconectar.... 
    lcd.print("procesando");
    resp2 = iniciar(); 
    Serial.println(resp2);
    if (resp2 == 1) {
       Serial.println("ENTRO EN OK PORQUE DIO POSITIVO");
       enviarServer(); 
    }else{
      Serial.println("TODO MAL JAJAJJAJAJAJAJJJ");
      reconectar();
    }
  }
}

void procesarMensaje(){ 
    Serial.println("=========INGRESO A PROCESAR MENSAJE============");
    String cod = splitCasero(msg, ":", 1);
    Serial.println(cod);
    if (cod=="a"){
        String saldo = splitCasero(msg, ":", 2);
        Serial.println("Que inicie el juego!Su nuevo saldo es:"+saldo);
        mensajeA(saldo);
        delay(3500);
        mensajeInicio();
    }
    else{
        if (cod=="b"){
          String saldo = splitCasero(msg, ":", 2);
          Serial.println("Saldo insuficiente! Su saldo actual:"+saldo);
          mensajeB(saldo);
          delay(3500);
          mensajeInicio();
        } else{
              if (cod=="c"){
                Serial.println("En mantenimiento, disculpe las molestias");
                mensajeC();
                delay(3500);
                mensajeInicio();
              } else{
                    if (cod=="d"){
                      Serial.println("Tarjeta no registrada...");
                      mensajeD();
                      delay(3500);
                      mensajeInicio();
                    } 
              }
        }
    }
}

String splitCasero(String cadena, String delimitador, int nroElemento){
  
    int commaIndex = cadena.indexOf(delimitador);
    int secondCommaIndex = cadena.indexOf(delimitador, commaIndex+1);
    String firstValue = cadena.substring(0, commaIndex); 
    String secondValue = cadena.substring(commaIndex+1, secondCommaIndex);
    String thirdValue = cadena.substring(secondCommaIndex+1); //To the end of the string

    if (nroElemento==0){
      return firstValue;
    }
    else{
      if (nroElemento==1){
          return secondValue;    
       }
       else{
          return thirdValue;
       }  
    }   
}

void mensajeInicio(){
   lcd.clear();
   lcd.print("Bienvenido!");
   lcd.setCursor(0, 1);
   lcd.print("Pase su tarjeta");  
}
void mensajeA(String saldito){
  lcd.clear();
  lcd.print("Muchas gracias!");
  lcd.setCursor(0, 1);
  lcd.print("Su saldo:"+saldito);  
}
void mensajeB(String saldito){
  lcd.clear();
  lcd.print("Saldo insuficien");
  lcd.setCursor(0, 1);
  lcd.print("te! Su saldo:"+saldito);  
}

void mensajeC(){
  lcd.clear();
  lcd.print("Disculpe! Fuera ");
  lcd.setCursor(0, 1);
  lcd.print("de servicio");  
}

void mensajeD(){
  lcd.clear();
  lcd.print("=(");
  lcd.setCursor(0, 1);
  lcd.print("No registrada!");
  //delay(2000);
  //lcd.noBacklight(); // turn off backlight
  //delay(1000);
  //lcd.backlight(); 
}
