/*************************************************************************************
*   Home Automation with Alexa and NodeMCU
*   WeMos smart devices emulation using a RGB LED
*  
*  The unmodified version of this code is originally
*  by kakopappa and can be found at http://bit.ly/2kKQiRg.
*
*  This version of the code has been modified by Charles Gantt
*   and requires five additional files which can be found at http://bit.ly/2lRDwAJ
*  Find out more about this project on Charles' website
*  http://www.themakersworkbench.com
*
*  3 "single" devices to be discovered
*   ==> RED
*   ==> GREEN
*   ==> BLUE
*   
*  Criating groups of devices
*   ==> YELLOW (RED/GREEN)
*   ==> WHITE (RED/GREEN/BLUE)
*
*  Code adapted from Charles Gantt original post by Marcelo Rovai on 15Aug17
*  Visit my blog: https://MJRoBot.org 
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
void ledRedOn();
void ledRedOff();
void ledGreenOn();
void ledGreenOff();
void ledBlueOn();
void ledBlueOff();

// Group devices on/off callbacks
void allLedsOn();
void allLedsOff();
void ledYellowOn();
void ledYellowOff();

// Change this before you flash
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *ledRed = NULL;
Switch *ledGreen = NULL;
Switch *ledBlue = NULL;
Switch *ledYellow = NULL;
Switch *allLeds = NULL;

// Set Relay Pins
int relayOne = D7;   // (red)
int relayTwo = D5;   // (green)
int relayThree = D6; // (blue)

void setup()
{
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Devices Definition (Max 14)
    // Format: Alexa invocation name, local port no, on callback, off callback
    
    // Single devices definition
    ledRed = new Switch("Red", 80, ledRedOn, ledRedOff);
    ledGreen = new Switch("Green", 81, ledGreenOn, ledGreenOff);
    ledBlue = new Switch("Blue", 82, ledBlueOn, ledBlueOff);
    
    // Group devices definition 
    ledYellow = new Switch("Yellow", 83, ledYellowOn, ledYellowOff);
    allLeds = new Switch("White", 84, allLedsOn, allLedsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*ledRed);
    upnpBroadcastResponder.addDevice(*ledGreen);
    upnpBroadcastResponder.addDevice(*ledBlue);
    upnpBroadcastResponder.addDevice(*ledYellow); 
    upnpBroadcastResponder.addDevice(*allLeds);

    //Set relay pins to outputs
    pinMode(relayOne, OUTPUT);
    pinMode(relayTwo, OUTPUT);
    pinMode(relayThree, OUTPUT);

    //Set each relay pin (LEDs) to LOW 
    digitalWrite(relayOne, LOW);   
    delay(500);
    digitalWrite(relayTwo, LOW);  
    delay(500);
    digitalWrite(relayThree, LOW);  
  }
}

void loop()
{
  if (wifiConnected) 
  {
    upnpBroadcastResponder.serverLoop();
    ledRed->serverLoop();
    ledGreen->serverLoop();
    ledBlue->serverLoop();
    ledYellow->serverLoop();
    allLeds->serverLoop();
  }
}

/* Functions for individuals Leds */
void ledRedOn() 
{
  Serial.print("RED LED turn on ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne on
}

void ledRedOff() {
  Serial.print("RED LED turn off ...");
  digitalWrite(relayOne, LOW);   // sets relayOne off
}

void ledGreenOn() {
  Serial.print("GREEN LED turn on ...");
  digitalWrite(relayTwo, HIGH);   // sets relayTwo on
}

void ledGreenOff() {
  Serial.print("GREEN LED turn off ...");
  digitalWrite(relayTwo, LOW);   // sets relayTwo Off
}

void ledBlueOn() {
  Serial.print("BLUE LED turn on ...");
  digitalWrite(relayThree, HIGH);   // sets relayThree on
}

void ledBlueOff() {
  Serial.print("BLUE LED turn off ...");
  digitalWrite(relayThree, LOW);   // sets relayThree off
}

/* Functions for groups */

void ledYellowOn() {
  Serial.print("YELLOW Color turn on ...");
  digitalWrite(relayOne, HIGH);   // sets relayFour on
  digitalWrite(relayTwo, HIGH);   // sets relayFour on
}

void ledYellowOff() {
  Serial.print("YELLOW Color turn off ...");
  digitalWrite(relayOne, LOW);   // sets relayFour off
  digitalWrite(relayTwo, LOW);   // sets relayFour on
}

void allLedsOn() {
  Serial.print("WHITE Color turn on ...");
  digitalWrite(relayOne, HIGH);   // sets relay1 on
  digitalWrite(relayTwo, HIGH);   // sets relay2 on
  digitalWrite(relayThree, HIGH);   // sets relay3 on
}

void allLedsOff() {
  Serial.print("WHITE Color turn off ...");
  digitalWrite(relayOne, LOW);   // sets relay1 off
  digitalWrite(relayTwo, LOW);   // sets relay2 off
  digitalWrite(relayThree, LOW);   // sets relay3 off
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
