//Name: getThingworxJSONLib
//Author: David Velasquez
//Date: 16/03/2024
//Description: This program gets 2 variables from Thingworx server and reflects the first one as a PWM on LED
//Requires ArduinoJson library

//Libraries
#include <ThingworxESP32.h>
#include <ArduinoJson.h>

//I/O pin labeling

//Server Constants
#define TPOST 1000  //Time between requests to TWX server (every 1 secs)
#define READCOUNT 2 //Number of variables to be read from thingworx (2)
const char* WIFI_SSID = "Change to your SSID"; //WiFi SSID
const char* WIFI_PASSWORD = "Change to your pass"; //WiFi Pass
//->TWX Constants
const char* TWX_HOST = "Change to your twx host";  //TWX Host example: myhost.edu.co (without http at beginning)
unsigned int TWX_PORT = 80; //TWX host port
const char* TWX_API_KEY = "Write your TWX API key here"; //API Key from TWX
const char* TWX_THING_NAME = "Name of your TWX Thing";  //Thing name from TWX
const char* TWX_PROPERTY_NAMES[] = {"PROPERY_NAME_1", "PROPERY_NAME_2"};  //Name of the variables (properties) to be read from the thingworx server

//Variables
//->TWX Vars
float twxPropertyValues[READCOUNT] = {0, 0};  //Thingworx read variables values converted to float
String inputJson = "";  //Variable to store Input Properties JSON
//->Timing Vars
unsigned long lastConnectionTime = 0; //Last connection ms time between server requests
//->ThingworxESP32 Vars
Thingworx myThing(TWX_HOST, TWX_PORT, TWX_API_KEY, TWX_THING_NAME);  //Declare the TWX object with his corresponding properties

//Subroutines & functions
void printWifiStatus() {
  //Print SSID name
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  //Print ipv4 assigned to WiFi module
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  //Print signal strength for WiFi module
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void WiFiInit() {
  delay(1000);  //Wait 1 sec for module initialization

  //Attempt a WiFi connection to desired access point at ssid, password
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1000); //Wait 1 sec for establishing connection
  }
  //Print WiFi status
  printWifiStatus();
}

void setup() {
  //I/O configuration
  pinMode(LED_BUILTIN, OUTPUT); //Built in LED from board as output

  //Physical outputs initialization
  digitalWrite(LED_BUILTIN, LOW); //Turn off built in LED

  //Communications
  Serial.begin(115200); //Serial communications with computer at 115200 bauds for debug purposes
  WiFiInit(); //WiFi communications initialization
}

void loop() {
  if (millis() - lastConnectionTime > TPOST) {  //Send request to server every TPOST seconds
    inputJson = myThing.getjson(); //Get all properties from server platform
    // inputJson = myThing.getjson("", true); //Get all properties from server platform and print debug info
    //inputJson = myThing.getjson("write1"); //Get 1 property with write1 label from server platform
    Serial.println(inputJson);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, inputJson);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    for (int i = 0; i < READCOUNT; i++) twxPropertyValues[i] = doc["rows"][0][TWX_PROPERTY_NAMES[i]];  //Extract each variable value from the JSON and store it on the corresponding var
    Serial.println("write1: " + String(twxPropertyValues[0]) + " write2: " + String(twxPropertyValues[1]));
    //twxPropertyValues[0] = doc["rows"][0][TWX_PROPERTY_NAMES[0]];
    //Serial.println("write1: " + String(twxPropertyValues[0]));
    analogWrite(LED_BUILTIN, twxPropertyValues[0]); //Reflect thingworx variable as PWM in onboard led
    lastConnectionTime = millis();  //Refresh last connection time
  }
}
