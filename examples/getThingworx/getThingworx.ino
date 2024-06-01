//Name: getThingworx
//Author: David Velasquez
//Date: 31/05/2024
//Description: This program gets Thingworx Server variables using REST GET request through ThingworxESP32 library

//Libraries
#include <ThingworxESP32.h>

//I/O pin labeling

//Constants
#define TPOST 1000  //Time between requests to TWX server (every 1 secs)
const char* WIFI_SSID = "Change to your SSID"; //WiFi SSID
const char* WIFI_PASSWORD = "Change to your pass"; //WiFi Pass
//->TWX Constants
const char* TWX_HOST = "Change to your twx host";  //TWX Host example: myhost.edu.co (without http at beginning)
unsigned int TWX_PORT = 80; //TWX host port
const char* TWX_API_KEY = "Write your TWX API key here"; //API Key from TWX
const char* TWX_THING_NAME = "Name of your TWX Thing";  //Thing name from TWX

//Variables
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

  //Attempt a WiFi connection to desired access point at WIFI_SSID, WIFI_PASSWORD
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

  //Physical outputs initialization

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
    lastConnectionTime = millis();  //Refresh last connection time
  }
}
