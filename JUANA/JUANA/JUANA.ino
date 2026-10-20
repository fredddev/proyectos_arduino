int wifiOn = 8;
#include "SoftwareSerial.h" //Libreria para emular puerto serial
//SoftwareSerial Serial2(5, 6); // RX, TX
SoftwareSerial Serial2(6, 5); // RX, TX
String msg;
String msg2;//string temporal para respuesta devuelta en respuestaWifi
String comando;



void setup()
{
  pinMode(wifiOn, OUTPUT);  
  Serial.begin(9600); //Iniciar el monitor serial
  Serial2.begin(9600); //Iniciar el modulo ESP
  encenderWifi();
  msg2 = respuestaWifi();
  
  Serial.println("==== fin de carga");
  delay(5000);
  wifiPrimeraVez();
}

void loop()
{
  
   if (Serial2.available())
  { 
    char a = Serial2.read() ;
    Serial.print(a);
  }
  if (Serial.available())
    { 
    char a = Serial.read();
    Serial2.print(a);
  }  
  delay(10000);
  enviarServer("hola desde arduino 1234567");
}

void encenderWifi(){
  digitalWrite(wifiOn, LOW);
  delay (5000);
  digitalWrite(wifiOn, HIGH);
  //delay(10000);
  
 }


String respuestaWifi(){
      int bandera=0;
      while (bandera == 0){
          if (Serial2.available()){
              delay(100);
              msg = Serial2.readString();
              bandera=1;
          }
      }
      //Serial.println("=====respuesta bruta====");
      //Serial.print(msg);
      //Serial.println("=====fin respuesta bruta====");
      msg.replace("\n", "");
      Serial.println(msg);
      return msg;
      
}


void enviarServer(String mensaje){
  Serial.println("=======Enviando mensaje a server====");
  String stringLength="AT+CIPSEND=";
  stringLength +=String(mensaje.length());
  stringLength +="\r\n";
  Serial2.print(stringLength); //con este comando le digo cuantos caracteres enviare a servidor...
  
  msg2 = respuestaWifi(); //respuesta del wifi para el comando numero de digitos que se enviara al servidor
  if (seEncuentra(msg2,"OK")){//si responde OK 
    Serial2.print(mensaje+"\r\n"); //enviamos el mensaje al servidor...
  }
  //else{
      //lcd.clear();
      //lcd.print("Procesando..."); 
      //ACA TENGO QUE VER LA OPCION DE RECONECTAR Y PROSEGUIR O NO HACE NADA...
      //reconectar(mensaje);
   //}
}

void wifiPrimeraVez(){
    Serial2.write("AT\r\n"); //VER SI ESTA RESPONDIENDO EL WIFI ESP8266
    msg2 = respuestaWifi();
    if (seEncuentra(msg2,"OK")){//si responde OK 
    
        Serial2.write("AT+CWQAP\r\n"); //desconectar de cualquier red
        msg2 = respuestaWifi(); //desconectar de cualquier red

        comando = "AT+CIPSTA=\"192.168.1.10\"\r\n";
        Serial2.print(comando);
        msg2 = respuestaWifi();
        if (seEncuentra(msg2,"OK")){//si responde OK 
            
            Serial2.write("AT+CWJAP=\"FAMILY\",\"mitzi1513\"\r\n"); //se conecta a la red
            msg2 = respuestaWifi(); 
            msg2 = respuestaWifi();
            if (seEncuentra(msg2,"OK")){//si responde OK 
                  Serial.println("=======CONECTADO A LA RED=======");  
                  Serial2.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",8050\r\n"); //me conecto al servidor
                  msg2 = respuestaWifi();
                  if (seEncuentra(msg2,"OK")){//si responde OK 
                      Serial.println("======exito server===");
                  }else{
                      Serial.println("=====error server======");
                  }
                        
            }else{
                  Serial.println("=======fallo el router=====");
                   Serial.println("=======CONECTADO A LA RED=======");  
                  Serial2.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",8050\r\n"); //me conecto al servidor
                  msg2 = respuestaWifi();
                  if (seEncuentra(msg2,"OK")){//si responde OK 
                      Serial.println("======exito server===");
                  }else{
                      Serial.println("=====error server======");
                  }
            }
            
        }else{
            Serial.println("=== fallo ip===");
        }
        
    }else{ //SI FALLA EL OK DEL COMANDO AT
        Serial.println("==== fallo todo===");
        
    }
    
    
}

boolean seEncuentra(String cad1,String cadBuscar){
    int pos=cad1.indexOf(cadBuscar);
    if (pos>=0) {
        return true;
    } else{
      return false;
    } 
}

