#define pinoD2 2
#define pinoD5 5
#define pinoD18 18
#define SERVO_PIN 26
#define SERVO_PIN3 27
#define SERVO_PIN2 14

#define PINO_BUZZER 32

#define PINO_RELE1 13
#define PINO_RELE2 15


#include <Stdio.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <WiFiManager.h>
//#include <pitches.h> 

const char* ssid     = "HUAWEI P30 Pro";
const char* password = "erickegado";

int aberto = 0;
int fechado = 100;

int cont1 = 0;
int cont2 = 0;
int cont3 = 0;

bool resposta;


WiFiServer server(80);
Servo servo1;
Servo servo2;
Servo servo3;


void setup()
{
    Serial.begin(115200);
    pinMode(pinoD2, OUTPUT);      // set the LED pin mode
    pinMode(pinoD5, OUTPUT);      // set the LED pin mode
    pinMode(pinoD18, OUTPUT);
    pinMode(SERVO_PIN, OUTPUT);      // set the LED pin mode
    pinMode(SERVO_PIN2, OUTPUT);
    pinMode(SERVO_PIN3, OUTPUT);

    pinMode(PINO_BUZZER, OUTPUT);

    pinMode(PINO_RELE1, OUTPUT);
    

    pinMode(PINO_RELE2, OUTPUT);
    


    servo1.attach(SERVO_PIN);
    servo2.attach(SERVO_PIN2);
    servo3.attach(SERVO_PIN3);

    servo1.write(fechado);
    servo2.write(fechado);
    servo3.write(fechado);

    delay(10);

    conecta_wifi();

    

}

void conecta_wifi(){
  int cont = 0;
  WiFiManager wm;
  bool res;
  char reposta;

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    cont++;

    if (cont == 25){
      break;
    }
  }

  res = wm.autoConnect("Caixa de Remedios");

  if(!res) {
    Serial.println("Failed to connect");
  } 
  else {    
    Serial.println("Conectado!");
  }


  IPAddress ip(192, 168, 43, 21); 
  IPAddress getaway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println(resposta);
            resposta = 0;
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;

          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        
        if (currentLine.endsWith("GET /bot1")) {
          servo1.write(aberto);
          resposta = 1;
        }
        if (currentLine.endsWith("GET /bot2")) {
          servo2.write(aberto);
          resposta = 1;

        }
        if (currentLine.endsWith("GET /bot3")) {
          servo3.write(aberto);
          resposta = 1;
        }
        
        if (currentLine.endsWith("GET /close1")) {
          servo1.write(fechado);
          desligaLeds();
          resposta = 1;
                          // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /close2")) {
          servo2.write(fechado);
          desligaLeds();
          resposta = 1;
                          // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /close3")) {
          servo3.write(fechado);
          desligaLeds();
          resposta = 1;
                          // GET /L turns the LED off
        }

        if (currentLine.endsWith("GET /buzzerH")) {
          /*
          digitalWrite(PINO_BUZZER, HIGH); // Ligar o buzzer
          delay(1000); // Deixa o buzzer ligado por 1 segundo
          digitalWrite(PINO_BUZZER, LOW); // Desligar o buzzer
          delay(1000);
          resposta = 1;
          */

        }

        if (currentLine.endsWith("GET /ledH")){
          ligaLeds();
          resposta = 1;
        }
      }
    }

    if (WiFi.status() != WL_CONNECTED){
      conecta_wifi();
    }

    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void ligaLeds(){
  if(digitalRead(PINO_RELE1) == LOW){
    digitalWrite(PINO_RELE1, HIGH);
    digitalWrite(PINO_RELE2, HIGH);
  }       
}

void desligaLeds(){
  if(digitalRead(PINO_RELE1) == HIGH){
    digitalWrite(PINO_RELE1, LOW);
    digitalWrite(PINO_RELE2, LOW);
  }
}