#include "SoftwareSerial.h" //Libreria para emular puerto serial
//SoftwareSerial Serial2(5, 6); // RX, TX
SoftwareSerial Serial2(6, 5); // RX, TX
String msg;//recibe la respuesta del wifi en loop
String msg2;//string temporal para respuesta devuelta en respuestaWifi
void setup()
{ 
  //Serial.begin(115200); //Iniciar el monitor serial
  //Serial2.begin(115200); //Iniciar el modulo ESP
  Serial.begin(9600); //Iniciar el monitor serial
  Serial2.begin(9600); //Iniciar el modulo ESP
  //Serial.begin(4800); //Iniciar el monitor serial
  //Serial2.begin(4800); //Iniciar el modulo ESP
  Serial.println("se cargo el sketch wifi");

}
 
void loop()
//Enviar los datos de la consola serial al ESP, y mostrar lo
//enviado por el ESP a nuestra consola.
{
  if (Serial2.available()) // si habla el wifi
  { 
    Serial.println("========hay sobra de respuesta==========");
    msg = Serial2.readString();
    msg.replace("\n", "");
    Serial.println(msg);
  }
  if (Serial.available()) //si escribo comandos
  { 
    //char a = Serial.read();
    //Serial.println("==================");
    //Serial.println(a);
    //Serial2.print(a);
    msg = Serial.readString();
    //Serial.println(msg);
    Serial2.println(msg);
    msg2 = respuestaWifi();
    Serial.println(msg2);
  }
}

String respuestaWifi(){
      int bandera=0;
      while (bandera == 0){
          if (Serial2.available()){
              msg = Serial2.readString();
              bandera=1;
          }
      }
      msg.replace("\n", "");
      return msg;
}
