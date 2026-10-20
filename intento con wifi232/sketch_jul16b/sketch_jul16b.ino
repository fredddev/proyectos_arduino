
#include <AltSoftSerial.h>

AltSoftSerial wifiSerial;

void setup() {
  // put your setup code here, to run once:
wifiSerial.begin(9600);

Serial.begin(9600);
Serial.println("levantando");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(wifiSerial.available()){
     Serial.write(wifiSerial.read());  
  }
  
  if(Serial.available()){
     wifiSerial.write(Serial.read()); 
  }
}
