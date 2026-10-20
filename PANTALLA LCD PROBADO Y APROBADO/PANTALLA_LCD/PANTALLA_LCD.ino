#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

void setup() {
Serial.begin(9600); //Iniciar el monitor serial
  
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("Pase su tarjeta");
  lcd.setCursor(0, 1);
  lcd.print("Costo 2 Bs.");

  Serial.println("se ejecuto");
}

void loop() {
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  //lcd.setCursor(0, 1);
   // Escribimos el número de segundos trascurridos
  //lcd.print(millis()/1000);
  //lcd.print(" Segundos");
  //delay(1000);
}

