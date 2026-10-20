#include "SoftwareSerial.h" //Libreria para emular puerto serial
SoftwareSerial Serial2(6, 5); // RX, TX

void setup()
{ 
  delay(100);
  Serial.begin(9600); //Iniciar el monitor serial
  delay(100);
  Serial2.begin(9600); //Iniciar el modulo ESP
  Serial.println("==== fin de carga v1");
  
  //Serial.begin(9600); //Iniciar el monitor serial
  //Serial2.begin(9600); //Iniciar el modulo ESP
  //Serial.begin(4800); //Iniciar el monitor serial
  //Serial2.begin(4800); //Iniciar el modulo ESP
}
 
void loop()
//Enviar los datos de la consola serial al ESP, y mostrar lo
//enviado por el ESP a nuestra consola.
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
}
