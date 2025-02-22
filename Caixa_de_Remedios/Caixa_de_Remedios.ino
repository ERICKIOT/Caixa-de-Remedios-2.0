#define pinoD2 2
#define pinoD5 5
#define pinoD18 18


#define SERVO_PIN3 26
#define SERVO_PIN2 27
#define SERVO_PIN 14

#define PINO_BUZZER 32

#define PINO_RELE1 13
#define PINO_RELE2 15


#include <Stdio.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <WiFiManager.h>
#include <pitches.h.ino>

const char* ssid     = "PRINT808";
const char* password = "PRiNt3RbLO8";

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
    digitalWrite(PINO_RELE1, HIGH);
    pinMode(PINO_RELE2, OUTPUT);
    digitalWrite(PINO_RELE2, HIGH);

    
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
          tocaAlarme();
          resposta = 1;
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
  if(digitalRead(PINO_RELE1) == HIGH){
    digitalWrite(PINO_RELE1, LOW);
    digitalWrite(PINO_RELE2, LOW);
  }       
}

void desligaLeds(){
  if(digitalRead(PINO_RELE1) == LOW){
    digitalWrite(PINO_RELE1, HIGH);
    digitalWrite(PINO_RELE2, HIGH);
  }
}

void tocaAlarme(){
  int melody[] = {
    NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
    NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
    NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
    NOTE_A4
  };

  int durations[] = {
    8, 8, 4, 4,
    8, 8, 4, 4,
    8, 8, 4, 4,
    2
  };

  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(PINO_BUZZER, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(PINO_BUZZER);
  }
}