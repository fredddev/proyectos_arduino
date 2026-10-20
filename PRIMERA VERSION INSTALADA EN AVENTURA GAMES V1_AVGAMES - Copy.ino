#include "SoftwareSerial.h" //wifi
#include <Wire.h> //lcd 16x2
#include <LiquidCrystal_I2C.h> //lcd16x2
#include <SPI.h>//rfid
#include <MFRC522.h>//rfid

SoftwareSerial Serial2(5, 6); // RX, TX //WIFI ESP
LiquidCrystal_I2C lcd(0x27,16,2);  //Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
byte wifiSwitch = 8; //para encender el wifi
int ledOk =3; //Definicion de pin para LED OK
int buzzer=4; //Definicion de pin para Buzzer
byte opto = 2; ////Definicion de pin para optoacoplador

String tarjetaRfid; //RC522
byte ActualUID[4]; //almacenará el código del Tag leído
String msg;
byte resp;
byte resp2;
char miIp [15] = "192.168.1.120";
char cmdIp [40] = "AT+CIPSTA=\"192.168.1.120\"\r\n";
char cmdRed[40] =  "AT+CWJAP=\"agames1\",\"agames987\"\r\n";
char cmdSer[50] = "AT+CIPSTART=\"TCP\",\"192.168.1.10\",8050\r\n";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Iniciar el monitor serial
  pinMode(wifiSwitch, OUTPUT); //pin para encender el wifi
  pinMode(ledOk,OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(opto, OUTPUT);
  Serial2.begin(9600); //Iniciar el modulo ESP
  lcd.init();// Inicializar el LCD
  lcd.backlight(); //Encender la luz de fondo.
  SPI.begin();        // Iniciamos  el MFRC522 LECTOR RFID
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522 LECTOR RFID
  encenderWifi();
  respuestaWifi();
  reconectar();//encender por primera vez
}

void loop() {
  // put your main code here, to run repeatedly:
   if ( mfrc522.PICC_IsNewCardPresent()){  //si hay tarjeta en el lector
     mensaje("Procesando...","Espere por favor",0);
     //digitalWrite(ledOk, HIGH);//encender el led
     digitalWrite(ledOk,HIGH);
     tone(buzzer, 3000);
     procesarTarjeta();
     digitalWrite(ledOk, LOW);//apagar el led
     noTone(buzzer);
    } 

    if (Serial2.available()){ //si habla el wifi
     msg = Serial2.readString();
     //Serial.println(msg);
     procesarMensaje();
    }
}
void encenderWifi(){
  digitalWrite(wifiSwitch, LOW);
  delay (5000);
  digitalWrite(wifiSwitch, HIGH);
  //delay(10000);
  
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
                 Serial.println(msg);
                 resp = seEncuentra2();
                 Serial.println(resp);
                 if (resp==1){
                    //imprimir("conectado a server");
                    mensaje("Bienvenido!","Pase su tarjeta",0);
                    return(1);
                 }else{
                     mensaje("err 5","",3000);
                    return(0);
                 }
              }else{
                 mensaje("err 4","",3000);
                return(0);
              }
              
          }else{
               mensaje("err 3","",3000);
              return(0);
          }
          
      }else{
           mensaje("err 2","",3000);
          return(0);
      }
  }else{
    mensaje("err 1","",3000);
    return(0);
  }
}

void respuestaWifi(){
      byte bandera=0;
      while (bandera == 0){
          if (Serial2.available()){
              delay(200);
              msg = Serial2.readString();
              bandera=1;
          }
      }
      msg.replace("\n", "");
}

byte seEncuentra(){
    //Serial.println(msg);
    int pos=msg.indexOf("OK");
    if (pos>=0) {
       // Serial.println("exito");
        return 1;
    } else{
     // Serial.println("fallo");
      return 0;
    } 
}
//para caso especial de conexion a el servidor tcp por mensaje
byte seEncuentra2(){
    //Serial.println(msg);
    int pos=msg.indexOf("OK");
    if (pos>=0) {
       // Serial.println("exito");
        return 1;
    } else{
        pos=msg.indexOf("ALREADY");
        if (pos>=0) {
           // Serial.println("exito");
            return 1;
        } else{
            
         // Serial.println("fallo");
          return 0;
        } 
    } 
}

void mensaje(String f1, String f2, int td){ //f1 fila de arriba, f2 fila de abajo, td tiempo de delay
   lcd.clear();
   lcd.print(f1);
   lcd.setCursor(0, 1);
   lcd.print(f2); 
   delay(td) ;
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
      Serial2.print(tarjetaRfid+"\r\n"); //enviamos el mensaje al servidor usando el wifi...
  }else{
    noTone(buzzer);
    mensaje("Reiniciando...","",0);
    reconectar();
    enviarServer();
    //intentar reconectar.... por que fallo
    //lcd.print("Reiniciame");
    //resp2 = iniciar(); 
    //if (resp2 == 1) {
     //  enviarServer(); 
    //}else{
      //reconectar();
    //}
  }
}
void mandarEnters(int vueltas){//para encender el juego
    int i;
    i=0;
     while (i<vueltas){
      digitalWrite(opto, HIGH);
      Serial.println("entre al enter..");
      delay(250);
      digitalWrite(opto, LOW);
      i=i+1;
      delay(1000);
    }
  }

void procesarMensaje(){ 
    //Serial.println("=========INGRESO A PROCESAR MENSAJE============");
    String cod = splitCasero(msg, ":", 1);
    //Serial.println(cod);
    if (cod=="a"){
        String saldo = splitCasero(msg, ":", 2);
        //Serial.println("Que inicie el juego!Su nuevo saldo es:"+saldo);
        //mensajeA(saldo);
        String nroPulsos = splitCasero(msg, ":", 3);
        mandarEnters(nroPulsos.toInt()); 
        mensaje("Exito!","Saldo:"+saldo,3000);
        //delay(3500);
        mensaje("Bienvenido!","Pase su tarjeta",0);
    }
    else{
        if (cod=="b"){
          String saldo = splitCasero(msg, ":", 2);
          //Serial.println("Saldo insuficiente! Su saldo actual:"+saldo);
          //mensajeB(saldo);
          //delay(3500);
          mensaje("Saldo insufu...","Saldo:"+saldo,3000);
          mensaje("Bienvenido!","Pase su tarjeta",0);
        } else{
              if (cod=="c"){
                //Serial.println("En mantenimiento, disculpe las molestias");
                //mensajeC();
                //delay(3500);
                mensaje("En mantenimiento!","Disculpe",3000);
                mensaje("Bienvenido!","Pase su tarjeta",0);
              } else{
                    if (cod=="d"){
                      //Serial.println("Tarjeta no registrada...");
                      //mensajeD();
                      //delay(3500);
                      mensaje("Tarjeta","no registrada",3000);
                      mensaje("Bienvenido!","Pase su tarjeta",0);
                    } 
              }
        }
    }
}
String splitCasero(String cadena, String delimitador, int nroElemento){
    //el elemento 0 es una parte que viene por defecto al usar comandos at y recibir datos asi que no cuenta...
    int commaIndex = cadena.indexOf(delimitador); //indexOf devuelve la posicion...
    int secondCommaIndex = cadena.indexOf(delimitador, commaIndex+1);
    int thirdCommaIndex = cadena.indexOf(delimitador, secondCommaIndex+1);
    
    String firstValue = cadena.substring(0, commaIndex); //cortar 
    String secondValue = cadena.substring(commaIndex+1, secondCommaIndex);
    String thirdValue = cadena.substring(secondCommaIndex+1, thirdCommaIndex); //To the end of the string
    String fourthValue = cadena.substring(thirdCommaIndex+1); //To the end of the string
  
    Serial.print("primero:"+firstValue+" ,segundo:"+secondValue+" ,tercero:"+thirdValue+" ,cuarto:"+fourthValue);
    //Serial.println(secondValue);
    //Serial.println(thirdValue);
    
    if (nroElemento==0){
      return firstValue;
    }
    else{
      if (nroElemento==1){
          return secondValue;    
       }
       else{
          if (nroElemento==2){
              return thirdValue;
           }
           else{
              //elemento 3
              return fourthValue;
           }  
       }  
    }   
}
