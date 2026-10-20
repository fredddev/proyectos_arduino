
int pushButton = 2; //El pin 2 tiene un boton pulsador y por le pusimos este nombre
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //inicializa el monitor serial

  pinMode(pushButton,INPUT); //defino pin 2 digital como entrada
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(pushButton); //leer el pin 2
  Serial.println(buttonState); //muestra en pantalla si el boton esta presionado sale 1, caso contrario sale 0
  delay(1);
}
