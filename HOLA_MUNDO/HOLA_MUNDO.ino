void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600); // a 9600 bits por segundo, establecemos la comunicacion serial usando la libreria serial
 Serial.println("Hola mundo!"); //imprime Hola mundo con salto de linea
 delay(2000);
 Serial.println("Es bueno estar aqui!!"); 
 delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(0x0d); //limpia la pantalla solo funciona en la pantalla de verdad!
  delay(2000);
  Serial.println("Hello Moto!");
}
