//ESTA VERSION FUNCIONA PERFECTAMENTE ESTOY VIENDO COMO HACER FUNCIONAR BIEN LA PARTE DE CONECTARRED... YA QUE NO RECIBO BIEN LA RESPUESTA SERIAL DEL WIFI A LOS COMANDOS 
#include "SoftwareSerial.h" //Libreria para emular puerto serial
SoftwareSerial Serial2(3, 4); // RX, TX //wifi esp8266
String msg;
String msg2;

void setup()
{ 
  Serial.begin(115200); //Iniciar el monitor serial
  iniciarComuWifi();
  conectarRed();
}
 
void loop()
{
  if (Serial2.available())
  { 
    msg = Serial2.readString();
    //Serial.print(msg);
    if (seEncuentra(msg,"OK")){
      Serial.print("exito");
    }else{
      Serial.print("fallo");
    }
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

void conectarRed(){
    delay(2000);
    Serial.println("Iniciando conexion");
    Serial.println("-------DESCONECTAR DE CUALQUIER RED--------");
    Serial2.write("AT+CWQAP\r\n"); //desconectar de cualquier red
    msg2 = respuestaWifi();
    if (seEncuentra(msg2,"OK")){
      Serial.println("Exito");
    }
    else{
      Serial.println("fallo"); 
    }
    
    //Serial.println(msg2); 
    //delay(1000);
    //Serial.println(respuestaWifi()); 
    
    //delay(50);
    Serial.println("---------------");
    //Serial2.write("AT+CIPSTA=\"192.168.1.12\"\r\n"); //establece el ip
    //Serial.println(respuestaWifi());
    //delay(50);
   //Serial.println("---------------");
    //Serial2.write("AT+CWJAP=\"FAMILY\",\"mitzi1513\"\r\n"); //se conecta a la red
    //Serial.println(respuestaWifi());
    //Serial.println("--------------");
    //delay(5000);
    //Serial2.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",2828\r\n"); //me conecto al servidor
    //Serial.println(respuestaWifi());
    //Serial.println("--------------");
    //delay(50);
    //Serial.println("Conectado al servidor");
  }
  
  String respuestaWifi(){
      int bandera=0;
      //msg = "";
      while (bandera == 0){
          if (Serial2.available()){
              //delay(1000); 
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


