/**********************************************************************************
* Code for controlling a Garage Door
* connected to one NodeMCU using Amazon Echo-Dot
* 
* Code based on the great open source lib & example code at: 
* http://tinkerman.cat/emulate-wemo-device-esp8266/
* which is based off of the Python example code by: 
* https://github.com/makermusings/fauxmo
* 
* Also, thanks to Sid for Sid's E Classroom
* https://www.youtube.com/c/SidsEClassroom
* 
* fauxmoESP is a library for ESP8266-based devices that emulates a Belkin WeMo device 
* and thus allows you to control them using this protocol, in particular from 
* Alexa-powered devices like the Amazon Echo or the Dot.
* 
*  Simulation using 1 device to be discovered:
*  
*   ==> Open Garage
*   (A single Relay is connected to NodeMCU Pin D5)
*   
*  To discovery device for the first time, use voice commnad: 
*  "Computer (or Alexa), discovery devices"
*  
*  To activate a device or function, you should use voice commands, like: 
*  "Computer (or Alexa), turn on Garage" "
*
*  Developed by Marcelo Rovai on 13Sept17
*  Visit my blog: https://MJRoBot.org 
  *********************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "YOUR SSID"
#define WIFI_PASS "YOUR PASSWORD"    

#define SERIAL_BAUDRATE 115200

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Devices NodeMCU Pins */

#define RELAY_1 D5

bool openGarage = LOW;

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);
   //setup and wifi connection
   wifiSetup();
 
   // Set relay pin to output
   pinMode(RELAY_1, OUTPUT);

  //Set relay pin to HIGH ====== NOTE THAT THE RELAY USES INVERSE LOGIC =====
   
   digitalWrite(RELAY_1, HIGH); 
    
   // Device Names for Simulated Wemo switches
   fauxmo.addDevice("Garage");
   
   fauxmo.onMessage(callback); 
}

void loop() 
{
  fauxmo.handle();
  openGarageCmd();
}

/* ---------------------------------------------------------------------------
 Device Callback
 ----------------------------------------------------------------------------*/
void callback(uint8_t device_id, const char * device_name, bool state) 
{
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) 
  {
    Serial.println("ON");
  } 
  else 
  {
    Serial.println("OFF");
  }
  
  //Switching action on detection of device name

    if ( (strcmp(device_name, "Garage") == 0) ) 
  {
    if (state) 
    {
      openGarage = HIGH; 
    } 
    else 
    {
      digitalWrite(RELAY_1, HIGH);
    }
  }
}

/* ---------------------------------------------------------------------------
 Special Function to open Garage
 ----------------------------------------------------------------------------*/
void openGarageCmd()
{
  if (openGarage == HIGH)
  { 
    digitalWrite(RELAY_1, LOW);
    delay(1000);
    digitalWrite(RELAY_1, HIGH);
    openGarage = LOW;
  }
}
    
/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
