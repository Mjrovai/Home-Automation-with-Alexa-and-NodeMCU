/**********************************************************************************
* Code for controlling a RGB LED
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
*  Simulation using 10 devices to be discovered:
*  
*  3 "single" devicesconnected directlly to NodeMCU Digital Pins:
*   ==> RED   (RGB RED)   ==> Connected to NodeMCU Pin D7
*   ==> GREEN (RGB GREEN) ==> Connected to NodeMCU Pin D5
*   ==> BLUE  (RGB BLUE)  ==> Connected to NodeMCU Pin D6
*   
*  2 groups of simple devices:
*   ==> YELLOW (RGB RED + RGB GREEN)
*   ==> WHITE (RGB RED + RGB GREEN + RGB BLUE)
*   
*  To discovery device for the first time, use voice commnad: 
*  "Computer (or Alexa), discovery devices"
*  
*  To activate a device or function, you should use voice commands, like: 
*  "Computer (or Alexa), turn on Blue" or "..., turn off Blue"
*
*  Developed by Marcelo Rovai on 22Aug17
*  Visit my blog: https://MJRoBot.org 
  *********************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "YOUR SSID HERE"
#define WIFI_PASS "YOUR PASSWORD HERE"    

#define SERIAL_BAUDRATE 115200

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Devices NodeMCU Pins */
#define RED_PIN     D7
#define GREEN_PIN   D5
#define BLUE_PIN    D6

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);
   //setup and wifi connection
   wifiSetup();
 
   // Set led pins to outputs
   pinMode(RED_PIN, OUTPUT);
   pinMode(GREEN_PIN, OUTPUT);
   pinMode(BLUE_PIN, OUTPUT);

   // Set each device to LOW 
   digitalWrite(RED_PIN, LOW);   
   digitalWrite(GREEN_PIN, LOW);  
   digitalWrite(BLUE_PIN, LOW); 
    
   // Device Names for Simulated Wemo switches
   fauxmo.addDevice("Red");
   fauxmo.addDevice("Green");
   fauxmo.addDevice("Blue");
   fauxmo.addDevice("Yellow");
   fauxmo.addDevice("White");
   fauxmo.onMessage(callback); 
}

void loop() 
{
  fauxmo.handle();
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
  
  if ( (strcmp(device_name, "Red") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(RED_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(RED_PIN, LOW);
    }
  }
  
  if ( (strcmp(device_name, "Green") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(GREEN_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(GREEN_PIN, LOW);
    }
  }
 
  if ( (strcmp(device_name, "Blue") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(BLUE_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(BLUE_PIN, LOW);
    }
  }

  if ( (strcmp(device_name, "Yellow") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
    }
  }

  if ( (strcmp(device_name, "White") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
    }
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
