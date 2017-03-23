/*************************************************************************************
*   Home Automation with Alexa and NodeMCU
*   WeMos smart devices emulation
*  
*  The unmodified version of this code is originally
*  by kakopappa and can be found at http://bit.ly/2kKQiRg.
*
*  This version of the code has been modified by Charles Gantt
*   and requires five additional files which can be found at http://bit.ly/2lRDwAJ
*  Find out more about this project on Charles' website
*  http://www.themakersworkbench.com
*
*  4 WeMo devices
*   ==> Light1
*   ==> Light2
*   ==> Outlet1
*   ==> Outlet2
*   
*  Code adapted from Charles Gantt original post by Marcelo Rovai on 23March17
*  
*  Criating groups of devices
*   ==> All Devices 
*   ==> Living Room (Light1 and Outlet2)
*   ==> Bed Room (Light2 and Outlet1)
*
*   Visit my blog: https://MJRoBot.org 
************************************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>

#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

// Single devices on/off callbacks
void lightOneOn();
void lightOneOff();
void lightTwoOn();
void lightTwoOff();
void outletOneOn();
void outletOneOff();
void outletTwoOn();
void outletTwoOff();

// Group devices on/off callbacks
void allDevicesOn();
void allDevicesOff();
void bedRoomOn();
void bedRoomOff();
void livingRoomOn();
void livingRoomOff();

// Change this before you flash
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *outletOne = NULL;
Switch *outletTwo = NULL;

Switch *allDevices = NULL;
Switch *bedRoom = NULL;
Switch *livingRoom = NULL;

// Set Relay Pins
int relayOne = 14;  // NodeMCU pin D5
int relayTwo = 15;  // NodeMCU pin D8
int relayThree = 3; // NodeMCU pin RX
int relayFour = 1;  // NodeMCU pin TX

void setup()
{
  Serial.begin(115200);
  wifiConnected = connectWifi();
  Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Devices Definition (Max 14)
    // Format: Alexa invocation name, local port no, on callback, off callback
    
    // Single devices definition
    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    outletOne = new Switch("Outlet One", 82, outletOneOn, outletOneOff);
    outletTwo = new Switch("Outlet Two", 83, outletTwoOn, outletTwoOff);
    
    // Group devices definition 
    allDevices = new Switch("All Devices", 84, allDevicesOn, allDevicesOff);
    bedRoom = new Switch("Bed Room", 85, bedRoomOn, bedRoomOff);
    livingRoom = new Switch("Living Room", 86, livingRoomOn, livingRoomOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    upnpBroadcastResponder.addDevice(*outletOne);
    upnpBroadcastResponder.addDevice(*outletTwo);

    upnpBroadcastResponder.addDevice(*allDevices);
    upnpBroadcastResponder.addDevice(*bedRoom);
    upnpBroadcastResponder.addDevice(*livingRoom);

    //Set relay pins to outputs
    pinMode(relayOne, OUTPUT);
    pinMode(relayTwo, OUTPUT);
    pinMode(relayThree, OUTPUT);
    pinMode(relayFour, OUTPUT);

    //Set each relay pin to HIGH
    digitalWrite(relayOne, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayTwo, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayThree, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayFour, HIGH);   // sets relayOne on
    delay(500);
  }
}

void loop()
{
  if (wifiConnected) 
  {
    upnpBroadcastResponder.serverLoop();
    lightOne->serverLoop();
    lightTwo->serverLoop();
    outletOne->serverLoop();
    outletTwo->serverLoop();
        
    allDevices->serverLoop();
    bedRoom->serverLoop();
    livingRoom->serverLoop();
  }
}

/* Functions for Lights */
void lightOneOn() {
  Serial.print("Switch 1 turn on ...");
  digitalWrite(relayOne, LOW);   // sets relayOne on
}

void lightOneOff() {
  Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne off
}

void lightTwoOn() {
  Serial.print("Switch 2 turn on ...");
  digitalWrite(relayThree, LOW);   // sets relayTwo on
}

void lightTwoOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(relayThree, HIGH);   // sets relayTwo Off
}

/* Functions for Outlets */
void outletOneOn() {
  Serial.print("Socket 1 turn on ...");
  digitalWrite(relayFour, LOW);   // sets relayThree on
}

void outletOneOff() {
  Serial.print("Socket 1 turn off ...");
  digitalWrite(relayFour, HIGH);   // sets relayThree off
}

void outletTwoOn() {
  Serial.print("Socket 2 turn on ...");
  digitalWrite(relayTwo, LOW);   // sets relayFour on
}

void outletTwoOff() {
  Serial.print("Socket 2 turn off ...");
  digitalWrite(relayTwo, HIGH);   // sets relayFour off
}

/* Functions for groups */

void allDevicesOn() {
  Serial.print("All Devices turn on ...");
  digitalWrite(relayOne, LOW);   // sets relay1 on
  digitalWrite(relayTwo, LOW);   // sets relay2 on
  digitalWrite(relayThree, LOW);   // sets relay3 on
  digitalWrite(relayFour, LOW);   // sets relay4 on
}

void allDevicesOff() {
  Serial.print("All Devices turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relay1 off
  digitalWrite(relayTwo, HIGH);   // sets relay2 off
  digitalWrite(relayThree, HIGH);   // sets relay3 off
  digitalWrite(relayFour, HIGH);   // sets relay4 off
}

void bedRoomOn() {
  Serial.print("Bed Room turn on ...");
  digitalWrite(relayThree, LOW);   // sets relay3 on
  digitalWrite(relayFour, LOW);   // sets relay4 on
}

void bedRoomOff() {
  Serial.print("Bed Room turn off ...");
  digitalWrite(relayThree, HIGH);   // sets relay3 off
  digitalWrite(relayFour, HIGH);   // sets relay4 off
}

void livingRoomOn() {
  Serial.print("Living Room turn on ...");
  digitalWrite(relayOne, LOW);   // sets relay1 on
  digitalWrite(relayTwo, LOW);   // sets relay2 on
}

void livingRoomOff() {
  Serial.print("Living Room turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relay1 off
  digitalWrite(relayTwo, HIGH);   // sets relay2 off
}

// connect to wifi – returns true if successful or false if not
  boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) 
  {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
  }
  else 
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
