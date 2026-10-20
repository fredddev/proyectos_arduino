#include <SoftwareSerial.h>
String respuestaWifi;
char caracterMonitor;
String recibidoMonitor;
int vueltas; 
SoftwareSerial esp8266(10, 11);

void setup() {
  iniciarPantallaMonitor();
  iniciarComunicacionRxTxWifi();
  iniciarComandosAT();
  vueltas=0;
}

void loop() {
  if (esp8266.available()) {
    Serial.write(esp8266.read());
  }
  if (Serial.available()) {
    recibidoMonitor = Serial.readStringUntil('\n');
    //Serial.println(recibidoMonitor.length()); //esto muestra en el monitor
    enviarServer(recibidoMonitor);
  }
  
}




void iniciarPantallaMonitor(){
 Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Monitor iniciado");
  }

void iniciarComunicacionRxTxWifi(){
  Serial.println("Estableciendo comunicacion con el Wifi");
  esp8266.begin(115200);
  while (!esp8266) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Comunicacion con modulo wifi iniciado");
  }

void enviarServer(String mensaje){
  esp8266.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",2828\r\n");
  delay(50);

  String stringLength="AT+CIPSEND=";
  stringLength +=String(mensaje.length());
  stringLength +="\r\n";
  esp8266.print(stringLength);
  //esp8266.println(cad2);
  delay(50);
  esp8266.print(mensaje+"\r\n");
 }

  
void iniciarComandosAT(){
   esp8266.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",2828\r\n");
  delay(100);
  esp8266.write("AT+CIPSEND=18\r\n");
  delay(100);
  esp8266.write("hola desde arduino\r\n");
  }

    
void temp(){
    // set the data rate for the SoftwareSerial port
  esp8266.begin(115200);
  
  //esp8266.write("AT\r\n"); 
  //esp8266.write("AT+CIPSTATUS\r\n"); // aparentemen \r\n indica fin de linea 

  esp8266.write("AT+CIPSTART=\"TCP\",\"192.168.1.4\",2828\r\n");
  delay(1000);
  esp8266.write("AT+CIPSEND=18\r\n");
  delay(1000);
  esp8266.write("hola desde arduino\r\n");
  }
  
