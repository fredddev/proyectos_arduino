#include <SoftwareSerial.h>

//rfid
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

//fin rfid
String respuestaWifi;
char caracterMonitor;
String recibidoMonitor;
String tarjetaRfid;
byte ActualUID[4]; //almacenará el código del Tag leído
//SoftwareSerial esp8266(10, 11);
SoftwareSerial esp8266(3, 4);

void setup() {
  iniciarRfid();
  iniciarPantallaMonitor();
  iniciarComunicacionRxTxWifi();
  iniciarComandosAT();
}

void loop() {
        if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
          procesarTarjeta();
            
        }
  
 //if (esp8266.available()) {
  //            Serial.write(esp8266.read());
   //  }
  
  if (Serial.available()) {
    recibidoMonitor = Serial.readStringUntil('\n');
    //Serial.println(recibidoMonitor.length()); //esto muestra en el monitor
    enviarServer(recibidoMonitor);
  }


  
}


void iniciarRfid(){
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  Serial.println("Lectura del UID");
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

void procesarTarjeta(){
       if ( mfrc522.PICC_ReadCardSerial()) 
                {
                      // Enviamos serialemente su UID
                      //Serial.print("Card UID:");
                      for (byte i = 0; i < mfrc522.uid.size; i++) {
                              //tarjetaRfid += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
                              //tarjetaRfid += mfrc522.uid.uidByte[i], HEX;
                             //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                              Serial.print(mfrc522.uid.uidByte[i], HEX);   
                              ActualUID[i]=(mfrc522.uid.uidByte[i]);
                      } 
                      tarjetaRfid =  String(ActualUID[0], HEX) + String(ActualUID[1], HEX) + String(ActualUID[2], HEX) + String(ActualUID[3], HEX);
                      tarjetaRfid.toUpperCase();
                      enviarServer(tarjetaRfid);

                       while (esp8266.available()) {
                        Serial.write(esp8266.read());
                      }
                      //while (esp8266.read()>0){
                       // Serial.write(esp8266.read());
                        //}
                        
                      
                      // Terminamos la lectura de la tarjeta  actual
                      mfrc522.PICC_HaltA();
                             
                } 
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

  void temp2(){
       // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                 // Serial.print("Card UID:");
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                  } 
                  Serial.println();
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();         
            } 
            enviarServer("rfid");     
            
  } 
    }
  
