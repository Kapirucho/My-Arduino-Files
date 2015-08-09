//**************************************************************
// fokus-dyp.mx - Sistema Entrega Escolar de Alumnos - v2.0
// Autor: Alejandro José Suarez Almaguer
// Esta libreria se usa cuando se combinan dos arduinos este codigo no se conecta a internet
// Ya que el dispositivo esclavo es el que llevala conexión y procesamiento hacia el servidor
// Este codigo es mas limpio
// 

#include <Wire.h>
#include <Wiegand.h>

int intentos = 0;
int ledON = 8;
int ledFUNC = 9;

WIEGAND wg; // Inicio wiegland

void setup() {
  Serial.begin(9600);
  Serial.println("fokus-dyp.mx - Sistema Entrega Escolar de Alumnos - v2.0");
  Serial.println("Iniciando sistema lector de tarjetas...");
  Wire.begin(); // join i2c bus (address optional for master)
  wg.begin();
  pinMode(ledON, OUTPUT);
  pinMode(ledFUNC, OUTPUT);
  
  startSetup();
  
  digitalWrite(ledON, HIGH);
}

void startSetup(){
  digitalWrite(ledON, HIGH);
  digitalWrite(ledFUNC, HIGH);
  delay(1000);
  digitalWrite(ledON, LOW);
  digitalWrite(ledFUNC, LOW);
}

void loop() {
  if(wg.available())
  {
    String codigo = String(wg.getCode());
    Serial.println(codigo);
    sendrequest(codigo);
    delay(500);
  }
}

//En esta libreria el codigo send request indica el momento en el que el arduino maestro enviara codigo al arduino esclavo
void sendrequest(String codigo){
  Wire.beginTransmission(2); // transmit to device #4
  Wire.write(codigo); //Envio maximo de 6bytes Codigo de tarjeta
  Wire.endTransmission();
  delay(500);
}
