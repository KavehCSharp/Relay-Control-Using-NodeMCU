#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "4 Channel Relay";
const char* password = "12345678";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t Relay1pin = D1;
bool Relay1status = HIGH;

uint8_t Relay2pin = D2;
bool Relay2status = HIGH;

uint8_t Relay3pin = D3;
bool Relay3status = HIGH;

uint8_t Relay4pin = D4;
bool Relay4status = HIGH;

uint8_t LED_Red = D5;
uint8_t LED_Yellow = D6;
uint8_t LED_Green = D7;

bool LedYellowStatus = LOW;

void setup() {
  
  digitalWrite(LED_Red, LOW);
  digitalWrite(LED_Yellow, LOW);
  digitalWrite(LED_Green, LOW);
  
  Serial.begin(115200);
  pinMode(Relay1pin, OUTPUT);
  pinMode(Relay2pin, OUTPUT);
  pinMode(Relay3pin, OUTPUT);
  pinMode(Relay4pin, OUTPUT);

  digitalWrite(Relay1pin, Relay1status);
  digitalWrite(Relay2pin, Relay2status);
  digitalWrite(Relay3pin, Relay3status);
  digitalWrite(Relay4pin, Relay4status);

  pinMode(D5, OUTPUT); //red
  pinMode(D6, OUTPUT); // yellow
  pinMode(D7, OUTPUT); // green

  for(int i=1;i<10;i++)
  {
    digitalWrite(LED_Yellow, LedYellowStatus);

    LedYellowStatus = !LedYellowStatus;

    delay(1000); // wait 1 second
  }
  
  LedYellowStatus = LOW;
  
  digitalWrite(LED_Yellow, LedYellowStatus);

  digitalWrite(LED_Green, HIGH);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/relay1on", handle_relay1on);
  server.on("/relay1off", handle_relay1off);

  server.on("/relay2on", handle_relay2on);
  server.on("/relay2off", handle_relay2off);

  server.on("/relay3on", handle_relay3on);
  server.on("/relay3off", handle_relay3off);

  server.on("/relay4on", handle_relay4on);
  server.on("/relay4off", handle_relay4off);

  server.onNotFound(handle_NotFound);
  
  server.begin();
  //Serial.WriteLine("HTTP server started");
}

void loop() {
  server.handleClient();
  if(Relay1status)
  {digitalWrite(Relay1pin, HIGH);}
  else
  {digitalWrite(Relay1pin, LOW);}

  if(Relay2status)
  {digitalWrite(Relay2pin, HIGH);}
  else
  {digitalWrite(Relay2pin, LOW);}

  if(Relay3status)
  {digitalWrite(Relay3pin, HIGH);}
  else
  {digitalWrite(Relay3pin, LOW);}

  if(Relay4status)
  {digitalWrite(Relay4pin, HIGH);}
  else
  {digitalWrite(Relay4pin, LOW);}
  
}

void handle_OnConnect() {
 
  server.send(200, "text/html", SendHTML(Relay1status,Relay2status,Relay3status,Relay4status)); 
}

void handle_relay1on() {
  Relay1status = HIGH;
  server.send(200, "text/html", SendHTML(true,Relay2status,Relay3status,Relay4status)); 
}

void handle_relay1off() {
  Relay1status = LOW;
  server.send(200, "text/html", SendHTML(false,Relay2status,Relay3status,Relay4status)); 
}

void handle_relay2on() {
  Relay2status = HIGH;
  server.send(200, "text/html", SendHTML(Relay1status,true,Relay3status,Relay4status)); 
}

void handle_relay2off() {
  Relay2status = LOW;
  server.send(200, "text/html", SendHTML(Relay1status,false,Relay3status,Relay4status)); 
}

void handle_relay3on() {
  Relay3status = HIGH;
  server.send(200, "text/html", SendHTML(Relay1status,Relay2status,true,Relay4status)); 
}

void handle_relay3off() {
  Relay3status = LOW;
  server.send(200, "text/html", SendHTML(Relay1status,Relay2status,false,Relay4status)); 
}

void handle_relay4on() {
  Relay4status = HIGH;
  server.send(200, "text/html", SendHTML(Relay1status,Relay2status,Relay3status,true)); 
}

void handle_relay4off() {
  Relay4status = LOW;
  server.send(200, "text/html", SendHTML(Relay1status,Relay2status,Relay3status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t r1stat, uint8_t r2stat, uint8_t r3stat, uint8_t r4stat){

  if(r1stat&&r2stat&&r3stat&&r4stat)
  {
    digitalWrite(LED_Red, HIGH);  
  }else
  {
    digitalWrite(LED_Red, LOW);  
  }

  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Relay Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 4 Channel Relay Project</h1>\n";
  ptr +="<h3>Kaveh Taheri</h3>\n";
  
   if(r1stat)
  {ptr +="<a class=\"button button-off\" href=\"/relay1off\">OFF</a>\n";}
  else
  {ptr +="<a class=\"button button-on\" href=\"/relay1on\">ON</a>\n";}

 if(r2stat)
  {ptr +="<a class=\"button button-off\" href=\"/relay2off\">OFF</a>\n";}
  else
  {ptr +="<a class=\"button button-on\" href=\"/relay2on\">ON</a>\n";}

   if(r3stat)
  {ptr +="<a class=\"button button-off\" href=\"/relay3off\">OFF</a>\n";}
  else
  {ptr +="<a class=\"button button-on\" href=\"/relay3on\">ON</a>\n";}

 if(r4stat)
  {ptr +="<a class=\"button button-off\" href=\"/relay4off\">OFF</a>\n";}
  else
  {ptr +="<a class=\"button button-on\" href=\"/relay4on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
