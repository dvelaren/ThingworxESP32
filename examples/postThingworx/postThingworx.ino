//Name: postThingworx
//Author: David Velasquez
//Date: 16/03/2024
//Description: This program sends 2 sensor vars to Thingworx Server.

//Libraries
#include <ThingworxESP32.h>

//I/O pin labeling
#define POT 0 //Potentiometer connected to pin A0

//Server Constants
#define TPOST 1000  //Time between requests to TWX server (every 1 secs)
#define SENSORCOUNT 2  //Number of sensor vars to send to TWX server (2)
const char* WIFI_SSID = "Change to your SSID"; //WiFi SSID
const char* WIFI_PASSWORD = "Change to your pass"; //WiFi Pass
//->TWX Constants
const char* TWX_HOST = "Change to your twx host";  //TWX Host example: myhost.edu.co (without http at beginning)
unsigned int TWX_PORT = 80; //TWX host port
const char* TWX_API_KEY = "Write your TWX API key here"; //API Key from TWX
const char* TWX_THING_NAME = "Name of your TWX Thing";  //Thing name from TWX
const char* TWX_SERVICE_NAME = "Name of your TWX Service";  //Thing service name from TWX
const char* TWX_PROPERTY_NAMES[] = {"INPUT1 name", "INPUT2 name"};  //Input names vector created on service from TWX (check names on TWX service)

//Variables
//->TWX Vars
float twxPropertyValues[SENSORCOUNT]; //Vector to store sensor vars to be sent to TWX
//->Timing Vars
unsigned long lastConnectionTime = 0; //Last connection ms time between server requests
//->ThingworxESP32 Vars
Thingworx myThing(TWX_HOST, TWX_PORT, TWX_API_KEY, TWX_THING_NAME, TWX_SERVICE_NAME);  //Declare the TWX object with his corresponding properties

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

  //Physical outputs initialization

  //Communications
  Serial.begin(115200); //Serial communications with computer at 115200 bauds for debug purposes
  WiFiInit(); //WiFi communications initialization
}

void loop() {
  //twxPropertyValues[0] = analogRead(POT) * 100.0 / 4095.0; //Uncomment if using real potentiometer at A0 pin
  twxPropertyValues[0] = 24; //Comment if using real potentiometer
  twxPropertyValues[1] = 88; //Write a constant value to the second sensor var
  if (millis() - lastConnectionTime > TPOST) {  //Send request to server every TPOST seconds
    myThing.post(SENSORCOUNT, TWX_PROPERTY_NAMES, twxPropertyValues); //Send values to server platform
    // myThing.post(SENSORCOUNT, TWX_PROPERTY_NAMES, twxPropertyValues, true); //Send values to server platform and print debug info
    lastConnectionTime = millis();  //Refresh last connection time
  }
}
