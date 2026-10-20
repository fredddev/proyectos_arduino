#include <SoftwareSerial.h>

SoftwareSerial esp8266(10, 11);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Started");

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

void loop() {
  if (esp8266.available()) {
    Serial.write(esp8266.read());
  }
  if (Serial.available()) {
    esp8266.write(Serial.read());
  }
}
