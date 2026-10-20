//ESTA VERSION FUNCIONA PERFECTAMENTE ESTOY VIENDO COMO HACER FUNCIONAR BIEN LA PARTE DE CONECTARRED... YA QUE NO RECIBO BIEN LA RESPUESTA SERIAL DEL WIFI A LOS COMANDOS 
#include "SoftwareSerial.h" //Libreria para emular puerto serial
SoftwareSerial Serial2(3, 4); // RX, TX //wifi esp8266
String msg;

void setup()
{ 
  Serial.begin(115200); //Iniciar el monitor serial
  iniciarComuWifi();
}
 
void loop()
{
  if (Serial2.available())
  { 
    msg = Serial2.readString();
    String cod = splitCasero(msg, ":", 1);
    if (cod=="a"){
        String saldo = splitCasero(msg, ":", 2);
        Serial.print("Que inicie el juego!Su nuevo saldo es:"+saldo);
    }
    else{
        if (cod=="b"){
          String saldo = splitCasero(msg, ":", 2);
          Serial.print("Saldo insuficiente! Su saldo actual:"+saldo);
        } else{
              if (cod=="c"){
              Serial.print("En mantenimiento, disculpe las molestias");
              } else{
                    if (cod=="d"){
                    Serial.print("Tarjeta no registrada...");
                    } 
              }
        }
    }
    //Serial.print(cod);
  }
  
  if (Serial.available())
  { 
    char a = Serial.read();
    Serial2.print(a);
  }
}

void iniciarComuWifi(){
 Serial2.begin(115200);
  while (!Serial2) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Comunicacion con wifi establecido");
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
  
 

 


