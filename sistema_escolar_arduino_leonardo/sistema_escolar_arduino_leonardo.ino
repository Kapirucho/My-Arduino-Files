//**************************************************************
// fokus-dyp.mx - Sistema Entrega Escolar de Alumnos - v2.0
// Autor: Alejandro José Suarez Almaguer
// Codigo fuente para la version de Arduino Leonardo

#include <WiegandLeonardo.h>
//#include <UIPEthernet.h> // Used for Ethernet
#include <Ethernet.h>
#include <SPI.h>
byte mac[] = { 0xE8, 0xC8, 0x70, 0xFB, 0xAD, 0x35 }; 

int intentos = 0;

EthernetClient client;
char server[] = "192.168.1.1";

int ledON = 8;
int ledFUNC = 9;

WIEGAND wg; // Inicio wiegland

void setup() {
  Serial.begin(9600);
  Serial.println("fokus-dyp.mx - Sistema Entrega Escolar de Alumnos - v2.0");
  Serial.println("Iniciando sistema lector de tarjetas...");
  wg.begin();
  Serial.println("Obteniendo direccion IP...");
  Ethernet.begin(mac);
  pinMode(ledON, OUTPUT);
  pinMode(ledFUNC, OUTPUT);
  
  startSetup();

  printIPStatus();
  digitalWrite(ledON, HIGH);
}

void startSetup(){
  digitalWrite(ledON, HIGH);
  digitalWrite(ledFUNC, HIGH);
  delay(1000);
  digitalWrite(ledON, LOW);
  digitalWrite(ledFUNC, LOW);
}

void printIPStatus(){
  Serial.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP());
}

void loop() {
  if(wg.available())
  {
    String codigo = String(wg.getCode());
    Serial.println(codigo);
    
    ProcessCode(codigo);
    
    delay(1000);
  }
}

void ProcessCode(String codigo){
  sendrequest(codigo);
  sendrequest("0");//Peticion fantasma, para que empuje la anterior
}

void sendrequest(String codigo){
  if (client.connect(server, 80)) {//Conectar al servidor y puerto dado
      digitalWrite(ledFUNC, HIGH);
      // Make a HTTP request:
      intentos = 0;
      Serial.println("-> Connected");
      Serial.println("Enviando solicitud"); //Imprimir el comando
      Serial.println("Command: GET /index.php/enespera/agregar/" + codigo + "/4" ); //Imprimir el comando
      client.println("GET /index.php/enespera/agregar/" + codigo + "/4");// + " HTTP/1.1");
      client.println(" HTTP/1.1");
      client.println(" Host: 192.168.1.1" );
      client.println(" User-Agent: Arduino/1.0");
      client.println(" Connection: close" );
      client.println();
      client.stop();
      Serial.println("Cerrado");
    }
    else {
      Serial.println("--> connection failed/n");
      if(intentos < 2){
        sendrequest(codigo);
        intentos++;
        delay(1000);
      } else {
        digitalWrite(ledFUNC, LOW);
        return;
      }
    }
    digitalWrite(ledFUNC, LOW);
   return; 
}
