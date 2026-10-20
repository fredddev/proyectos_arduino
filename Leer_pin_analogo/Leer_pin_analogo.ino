void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // inicializa la comunicacion serial
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A0); //leer el pint analogo A0
  Serial.println(sensorValue); //muestra en pantalla
  delay(1);
}
