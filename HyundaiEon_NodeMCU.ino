/*
 * Car Central Lock system control using NodeMCU
 * By: Tashmin Mishra
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

/* Put your SSID & Password */
const char* ssid = "";  // Enter SSID here
const char* password = "";  //Enter Password here

ESP8266WebServer server(80);

uint8_t Relay1 = D5;
uint8_t Relay2 = D6;
uint8_t Relay3 = D7;
uint8_t Relay4 = D8;

int load1, load2, load3, load4;

void setup() {
  Serial.begin(115200);
  Serial.println();

  IPAddress local_IP(192,168,4,1);
  IPAddress gateway(192,168,4,9);
  IPAddress subnet(255,255,255,0);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  pinMode(Relay1, OUTPUT); 
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT); 
 
  EEPROM.begin(512);
  
  load1 =0;
  load2 =0;
  load3 =0;
  load4 =0;

  Serial.print(load1); Serial.print(" "); 
  Serial.print(load2); Serial.print(" "); 
  Serial.print(load3); Serial.print(" "); 
  Serial.println(load4);

  digitalWrite(Relay1, load1); 
  digitalWrite(Relay2, load2); 
  digitalWrite(Relay3, load3); 
  digitalWrite(Relay4, load4);

  WiFi.softAP(ssid, password);
  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: \n");
  Serial.println(apip);
    
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/L1on",  handle_L1on);
  server.on("/L2on",  handle_L2on);
  server.on("/L3on",  handle_L3on);
  server.on("/L4on",  handle_L4on);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
    Serial.print(load1); Serial.print(" "); 
    Serial.print(load2); Serial.print(" "); 
    Serial.print(load3); Serial.print(" "); 
    Serial.println(load4);
    digitalWrite(Relay1, load1); 
    digitalWrite(Relay2, load2); 
    digitalWrite(Relay3, load3); 
    digitalWrite(Relay4, load4);
    delay(300);
    resetLoad();
}
void resetLoad(){
  load1=0;load2=0;load3=0;load4=0;
}
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(load1,load2,load3,load4)); 
}

void handle_L1on()  {load1=1;data();}
void handle_L2on()  {load2=1;data();}
void handle_L3on()  {load3=1;data();}
void handle_L4on()  {load4=1;data();}


void data(){
EEPROM.write(1, load1);
EEPROM.write(2, load2);
EEPROM.write(3, load3);
EEPROM.write(4, load4);  
EEPROM.commit();

  
server.send(200, "text/html", SendHTML(load1,load2,load3,load4));   
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t l1stat,uint8_t l2stat,uint8_t l3stat,uint8_t l4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Central Lock Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; background-color: #d7f2fa; display: inline-block; margin: 0px auto; text-align: center;}\n";
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
  ptr +="<h1>Hyundai-Eon Controller</h1>\n";
 

  ptr +="<p>\n</p><a class=\"button button-on\" href=\"/L1on\">Lock</a>\n";
  ptr +="<p> </p><a class=\"button button-on\" href=\"/L2on\">Unlock</a>\n";
  ptr +="<p> </p><a class=\"button button-on\" href=\"/L3on\">Mute</a>\n";
  ptr +="<p> </p><a class=\"button button-on\" href=\"/L4on\">Alarm</a>\n";
  
  

  ptr +="</body>\n";
  ptr +="<foot>\n";
  ptr +="<p><i>By: Tashmin Mishra</i></p>\n";
  ptr +="</foot>\n";
  ptr +="</html>\n";
  return ptr;
}
