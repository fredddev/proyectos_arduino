//========= oficial ==================
#include "SoftwareSerial.h" //wifi
#include <Wire.h> //lcd 16x2
#include <LiquidCrystal_I2C.h> //lcd16x2
#include <SPI.h>//rfid
#include <MFRC522.h>//rfid

SoftwareSerial Serial2(3, 4); // RX, TX //wifi esp8266
String msg;//recibe la respuesta del wifi en loop
String msg2;//string temporal para respuesta devuelta en respuestaWifi
String miIp = "192.168.1.12";
String miRed = "FAMILY";
String contraRed = "mitzi1513";
String iPServidor = "192.168.1.4";
String comando;


LiquidCrystal_I2C lcd(0x27,16,2);  //Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
String tarjetaRfid; //RC522
byte ActualUID[4]; //almacenará el código del Tag leído

void setup() {
  // put your setup code here, to run once:
  iniciarMonitorPc();
  iniciarComuWifi();
  iniciarRfid();
  iniciarLcd();
  wifiPrimeraVez();  
}

void loop() {
  if (Serial2.available()){ //si habla el wifi
     msg = Serial2.readString();
     Serial.println(msg);
     procesarMensaje(msg);
  }

  if (Serial.available()){ //si introduzco algun comando 
      char a = Serial.read();
      Serial2.print(a);
  }

  if ( mfrc522.PICC_IsNewCardPresent()){  //si hay tarjeta en el lector
     procesarTarjeta();
  }
}

void iniciarLcd(){
  lcd.init();// Inicializar el LCD
  lcd.backlight(); //Encender la luz de fondo.
}

void iniciarRfid(){
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
  Serial.println("LECTOR RFID INICIADO");
}

void iniciarMonitorPc(){
  Serial.begin(4800); //Iniciar el monitor serial
}

void iniciarComuWifi(){
  Serial2.begin(4800);
  while (!Serial2) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void wifiPrimeraVez(){
    lcd.print("Iniciando wifi");
    Serial2.write("AT\r\n"); //VER SI ESTA RESPONDIENDO EL WIFI ESP8266
    msg2 = respuestaWifi();
    if (seEncuentra(msg2,"OK")){//si responde OK 
        lcd.clear();
        lcd.print("wifi ok");

        Serial2.write("AT+CWQAP\r\n"); //desconectar de cualquier red
        msg2 = respuestaWifi(); //desconectar de cualquier red

        comando = "AT+CIPSTA=\"" + miIp + "\"\r\n";
        Serial2.print(comando); //establecer ip
        //Serial2.write("AT+CIPSTA=\"192.168.1.12\"\r\n");
        msg2 = respuestaWifi();
        if (seEncuentra(msg2,"OK")){//si responde OK 
            lcd.clear();
            lcd.print(miIp); //ip   
            
            comando = "AT+CWJAP=\"" + miRed + "\"" + "," + "\"" + contraRed + "\"" + "\r\n";
            Serial2.print(comando); //se conecta a la red
            msg2 = respuestaWifi(); 
            msg2 = respuestaWifi();
            if (seEncuentra(msg2,"OK")){//si responde OK 
                  lcd.clear();
                  lcd.print("Router conectado"); 

                  Serial.println("========Conectando a Servidor==========");
                  comando = "AT+CIPSTART=\"TCP\",\"" + iPServidor + "\",2828\r\n";
                  Serial2.print (comando); //me conecto al servidor
                  //delay(100);
                  msg2 = respuestaWifi();
                  Serial.println(msg2);
                  
                  Serial.println(msg2);
                  
                  if (seEncuentra(msg2,"OK")){//si responde OK 
                      Serial.println("Respuesta = OK");
                      lcd.clear();
                      lcd.print("Conectado a server"); 
                      delay(2000);
                      mensajeInicio(); 
                  }else{
                      Serial.println("Respuesta = ERROR");
                      lcd.clear();
                      lcd.print("Server no encontrado"); 
                      lcd.setCursor(0, 1);
                      lcd.print("reintentando..."); 
                      delay(2000);
                      lcd.clear();
                      wifiPrimeraVez();
                  }
                        
            }else{
                  lcd.clear();
                  lcd.print("Fallo router"); 
                  lcd.setCursor(0, 1);
                  lcd.print("reintentando..."); 
                  delay(2000);
                  lcd.clear();
                  wifiPrimeraVez();
            }
            
        }else{
            lcd.clear();
            lcd.print("fallo Ip, reintentando");
            delay(2000);
            wifiPrimeraVez();
        }
        
    }else{ //SI FALLA EL OK DEL COMANDO AT
        lcd.clear();
        lcd.print("wifi fallo, reintentando");
        delay(2000);
        lcd.clear();
        wifiPrimeraVez();
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
      //Serial.println("=====respuesta bruta====");
      //Serial.print(msg);
      //Serial.println("=====fin respuesta bruta====");
      msg.replace("\n", "");
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

void procesarMensaje(String msg3){ 
    Serial.println("=========INGRESO A PROCESAR MENSAJE============");
    String cod = splitCasero(msg3, ":", 1);
    Serial.println(cod);
    if (cod=="a"){
        String saldo = splitCasero(msg3, ":", 2);
        Serial.println("Que inicie el juego!Su nuevo saldo es:"+saldo);
        mensajeA(saldo);
        delay(5000);
        mensajeInicio();
    }
    else{
        if (cod=="b"){
          String saldo = splitCasero(msg3, ":", 2);
          Serial.println("Saldo insuficiente! Su saldo actual:"+saldo);
          mensajeB(saldo);
          delay(5000);
          mensajeInicio();
        } else{
              if (cod=="c"){
                Serial.println("En mantenimiento, disculpe las molestias");
                mensajeC();
                delay(5000);
                mensajeInicio();
              } else{
                    if (cod=="d"){
                      Serial.println("Tarjeta no registrada...");
                      mensajeD();
                      delay(5000);
                      mensajeInicio();
                    } 
              }
        }
    }
}

void procesarTarjeta(){
       if ( mfrc522.PICC_ReadCardSerial()){
             for (byte i = 0; i < mfrc522.uid.size; i++) {
                     //Serial.print(mfrc522.uid.uidByte[i], HEX);   
                     ActualUID[i]=(mfrc522.uid.uidByte[i]);
             } 
             tarjetaRfid =  String(ActualUID[0], HEX) + String(ActualUID[1], HEX) + String(ActualUID[2], HEX) + String(ActualUID[3], HEX);
             enviarServer(tarjetaRfid);
             Serial.println(tarjetaRfid); 
             // Terminamos la lectura de la tarjeta  actual
             mfrc522.PICC_HaltA();
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


void enviarServer(String mensaje){
  String stringLength="AT+CIPSEND=";
  stringLength +=String(mensaje.length());
  stringLength +="\r\n";
  Serial2.print(stringLength); //con este comando le digo cuantos caracteres enviare a servidor...
  msg2 = respuestaWifi(); //respuesta del wifi para el comando numero de digitos que se enviara al servidor
  if (seEncuentra(msg2,"OK")){//si responde OK 
      Serial2.print(mensaje+"\r\n"); //enviamos el mensaje al servidor...
  }else{
      lcd.clear();
      lcd.print("Procesando..."); 
      //ACA TENGO QUE VER LA OPCION DE RECONECTAR Y PROSEGUIR O NO HACE NADA...
      //reconectar(mensaje);
      
  }
}



void reiniciar(String error){
  lcd.clear();
  lcd.print("Error: " + error);
  lcd.setCursor(0,1);
  lcd.print("Lo sentimos");
  delay(5000);
  lcd.clear();
  wifiPrimeraVez();
}
//======MENSAJES=========
void mensajeInicio(){
   lcd.clear();
   lcd.print("Bienvenido!");
   lcd.setCursor(0, 1);
   lcd.print("Pase su tarjeta");  
}
void mensajeA(String saldito){
  lcd.clear();
  lcd.print("Muchas gracias!");
  lcd.setCursor(0, 1);
  lcd.print("Su saldo:"+saldito);  
}

void mensajeB(String saldito){
  lcd.clear();
  lcd.print("Saldo insuficien");
  lcd.setCursor(0, 1);
  lcd.print("te! Su saldo:"+saldito);  
}

void mensajeC(){
  lcd.clear();
  lcd.print("Disculpe! Fuera ");
  lcd.setCursor(0, 1);
  lcd.print("de servicio");  
}

void mensajeD(){
  lcd.clear();
  lcd.print("=(");
  lcd.setCursor(0, 1);
  lcd.print("No registrada!");
  delay(2000);
  lcd.noBacklight(); // turn off backlight
  delay(1000);
  lcd.backlight(); 
}

void reconectar(String mensaje){
        Serial.println("======Desconectando======");
        Serial2.write("AT+CWQAP\r\n"); //desconectar de cualquier red
        msg2 = respuestaWifi(); //desconectar de cualquier red
        Serial.println(msg2);

        Serial.println("======Conectando a red======");
        comando = "AT+CWJAP=\"" + miRed + "\"" + "," + "\"" + contraRed + "\"" + "\r\n";
        Serial2.print(comando); //se conecta a la red
        msg2 = respuestaWifi(); 
        Serial.println(msg2);
        msg2 = respuestaWifi();
        Serial.println(msg2);
        if (seEncuentra(msg2,"OK")){

            Serial.println("======Conectando a Servidor======");
            comando = "AT+CIPSTART=\"TCP\",\"" + iPServidor + "\",2828\r\n";
            Serial2.print (comando); //me conecto al servidor
            msg2 = respuestaWifi();
            Serial.println(msg2);    
            if (seEncuentra(msg2,"OK")){//si responde OK 
                Serial.println("LA RESPUESTA FUE OK");
                enviarServer(mensaje);         
            }else{
                Serial.println("LA RESPUESTA FUE ERROR");
                reiniciar("Servidor");  
            }
            
        }else{
          reiniciar("Router");  
        }
        
}

  
