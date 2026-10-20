//========= oficial ==================
#include "SoftwareSerial.h"

SoftwareSerial Serial2(3, 4); // RX, TX //wifi esp8266
String msg;
String msg2;

void setup() {
  // put your setup code here, to run once:
  iniciarMonitorPc();
  iniciarComuWifi();
  conectarRed();  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()){ 
     msg = Serial2.readString();
     procesarMensaje(msg);
  }
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
              Serial.print(msg);
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
  
  
