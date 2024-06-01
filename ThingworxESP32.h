/**********************************************************************************************************
ThingworxESP32.h - Library for uploading values to a ThingWorx "Thing" via ESP32 modules
Created by David Velasquez 31/05/2024 for
Universidad EAFIT
www.eafit.edu.co

Create a service in your Thing or ThingShape and create inputs that will match the variable names you use
in your Arduino code.  In the JavaScript window type something similar to the following:

me.PropertyOne = parseFloat(InputOne);
me.PropertyTwo = parseFloat(InputTwo);

Where Property one is the name of your first Thing or ThingShape property and InputOne is the name of
your first input.  Everything is case sensitive.
************************************************************************************************************/

#ifndef ThingworxESP32_h
#define ThingworxESP32_h

#include "Arduino.h"
#include "SPI.h"
#include "WiFi.h"
#include "string.h"

class Thingworx {
	public:
		Thingworx(const char* server, const int port, const char* appKey, const char* thingName, const char* serviceName = "");
		void post(const int sensorCount, const char* sensorNames[], float values[], bool debug=false);
		String getjson();
		String getjson(String property, bool debug=false);
		//void get(int inputCount, char* inputNames[], float *inputValues);	To be implemented

	private:
		WiFiClient _client;
		const char* _server;
		const int _port;
		const char* _appKey;
		const char* _thingName;
		const char* _serviceName;
};

#endif
