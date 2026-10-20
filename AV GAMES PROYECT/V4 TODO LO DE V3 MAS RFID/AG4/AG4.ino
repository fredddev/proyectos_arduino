//========= oficial ==================
#include "SoftwareSerial.h" //wifi
#include <SPI.h>//rfid
#include <MFRC522.h>//rfid

SoftwareSerial Serial2(3, 4); // RX, TX //wifi esp8266
String msg;//recibe la respuesta del wifi en loop
String msg2;//string temporal para respuesta devuelta en respuestaWifi

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

String tarjetaRfid;
byte ActualUID[4]; //almacenará el código del Tag leído

void setup() {
  // put your setup code here, to run once:
  iniciarMonitorPc();
  iniciarComuWifi();
  iniciarRfid();
  conectarRed();  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()){ 
     msg = Serial2.readString();
     procesarMensaje(msg);
     //Serial.println(msg);
  }

  if ( mfrc522.PICC_IsNewCardPresent()){  
     procesarTarjeta();
  }

   //if (Serial.available()){ 
    //char a = Serial.read();
    //Serial2.print(a);
  //}
}

void iniciarMonitorPc(){
  Serial.begin(115200); //Iniciar el monitor serial
}

void iniciarComuWifi(){
  Serial2.begin(115200);
  while (!Serial2) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void conectarRed(){
    delay(2000);
    Serial.println("Iniciando conexion de wifi");
    Serial.println("-------DESCONECTAR DE CUALQUIER RED--------");
    Serial2.write("AT+CWQAP\r\n"); //desconectar de cualquier red
    msg2 = respuestaWifi();
    Serial.println(msg2);
    Serial.println("--------exito-------");

    Serial.println("-------ESTABLECER IP--------");
    Serial2.write("AT+CIPSTA=\"192.168.1.12\"\r\n"); //establece el ip
    msg2 = respuestaWifi();
    Serial.println(msg2);
    Serial.println("--------exito-------");

    Serial.println("-------CONECTAR A ROUTER--------");
    Serial2.write("AT+CWJAP=\"FAMILY\",\"mitzi1513\"\r\n"); //se conecta a la red
    msg2 = respuestaWifi();
    Serial.println(msg2);
    Serial.println("--------exito-------");
    delay(3000);
    Serial.println("-------CONECTAR A SERVIDOR TCP--------");
    Serial2.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",2828\r\n"); //me conecto al servidor
    msg2 = respuestaWifi();
    Serial.println(msg2);
    Serial.println("--------exito-------");
   
}

String respuestaWifi(){
      int bandera=0;
      while (bandera == 0){
          if (Serial2.available()){
              msg = Serial2.readString();
              Serial.println(msg);
              bandera=1;
          }
      }
      return msg;
}
boolean seEncuentra(String cad1,String cadBuscar){
    int pos=cad1.indexOf(cadBuscar);
    if (pos>=0) {
        return true;
    } else{
      return false;
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

void procesarMensaje(String msg3){ 
    String cod = splitCasero(msg3, ":", 1);
    if (cod=="a"){
        String saldo = splitCasero(msg3, ":", 2);
        Serial.println("Que inicie el juego!Su nuevo saldo es:"+saldo);
    }
    else{
        if (cod=="b"){
          String saldo = splitCasero(msg3, ":", 2);
          Serial.println("Saldo insuficiente! Su saldo actual:"+saldo);
        } else{
              if (cod=="c"){
              Serial.println("En mantenimiento, disculpe las molestias");
              } else{
                    if (cod=="d"){
                    Serial.println("Tarjeta no registrada...");
                    } 
              }
        }
    }
}

void iniciarRfid(){
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  Serial.println("LECTOR RFID INICIADO");
}

void procesarTarjeta(){
       if ( mfrc522.PICC_ReadCardSerial()){
             for (byte i = 0; i < mfrc522.uid.size; i++) {
                     //Serial.print(mfrc522.uid.uidByte[i], HEX);   
                     ActualUID[i]=(mfrc522.uid.uidByte[i]);
             } 
             tarjetaRfid =  String(ActualUID[0], HEX) + String(ActualUID[1], HEX) + String(ActualUID[2], HEX) + String(ActualUID[3], HEX);
             enviarServer(tarjetaRfid);
             Serial.println(tarjetaRfid); 
             // Terminamos la lectura de la tarjeta  actual
             mfrc522.PICC_HaltA();
       } 
}

void enviarServer(String mensaje){
  String stringLength="AT+CIPSEND=";
  stringLength +=String(mensaje.length());
  stringLength +="\r\n";
  Serial2.print(stringLength);
  msg2 = respuestaWifi();
  Serial2.print(mensaje+"\r\n");
  //msg2 = respuestaWifi(); //aca hay un problema porque intercepto el mensaje de ok de enviado y tambien intercepto la respuesta!! eso tendria que ver como capturar una linea o una respuesta a la vez
}
  
  
