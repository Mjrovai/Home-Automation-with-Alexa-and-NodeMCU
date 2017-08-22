/**********************************************************************************
* Code for controlling Enterprise Weapons Simulation via RGB LEDs, Red LED and a Buzzer 
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
*  5 "single" devicesconnected directlly to NodeMCU Digital Pins:
*   ==> RED   (RGB RED)   ==> Connected to NodeMCU Pin D7
*   ==> GREEN (RGB GREEN) ==> Connected to NodeMCU Pin D5
*   ==> BLUE  (RGB BLUE)  ==> Connected to NodeMCU Pin D6
*   ==> LIGHT (LED RED)   ==> Connected to NodeMCU Pin D4
*   ==> ALARM             ==> Connected to NodeMCU Pin D8
*   
*  2 groups of simple devices:
*   ==> YELLOW (RGB RED + RGB GREEN)
*   ==> WHITE (RGB RED + RGB GREEN + RGB BLUE)
*   
*  3 special "weapon simulation" functions:
*   ==> Firing Torpedos
*   ==> Firing Phasers
*   ==> Firing All Weapons
*   
*  To discovery device for the first time, use voice commnad: 
*  "Computer (or Alexa), discovery devices"
*  
*  To activate a device or function, you should use voice commands, like: 
*  "Computer (or Alexa), turn on Light" or "..., turn off Light"
*  "Computer (or Alexa), turn on Alarm" or "..., turn off Alarm"
*  "Computer (or Alexa), turn on Blue" or "..., turn off Blue"
*  "Computer (or Alexa), turn on Torpedos"
*  "Computer (or Alexa), turn on Phasers"
*  "Computer (or Alexa), turn on All Weapons"
*
*  Developed by Marcelo Rovai on 19Aug17
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
#define RED_LED_PIN D4
#define RED_PIN     D7
#define GREEN_PIN   D5
#define BLUE_PIN    D6
#define BUZZER_PIN  D8

bool fireTorpedos = LOW;
bool firePhasers = LOW;
bool fireAllWeapons = LOW;

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);
   //setup and wifi connection
   wifiSetup();
 
   // Set led pins to outputs
   pinMode(RED_LED_PIN, OUTPUT);
   pinMode(RED_PIN, OUTPUT);
   pinMode(GREEN_PIN, OUTPUT);
   pinMode(BLUE_PIN, OUTPUT);
   pinMode(BUZZER_PIN, OUTPUT);

   // Set each device to LOW 
   digitalWrite(RED_LED_PIN, LOW);
   digitalWrite(RED_PIN, LOW);   
   digitalWrite(GREEN_PIN, LOW);  
   digitalWrite(BLUE_PIN, LOW); 
   digitalWrite(BUZZER_PIN, LOW); 
    
   // Device Names for Simulated Wemo switches
   fauxmo.addDevice("Red");
   fauxmo.addDevice("Green");
   fauxmo.addDevice("Blue");
   fauxmo.addDevice("Yellow");
   fauxmo.addDevice("White");
   fauxmo.addDevice("Light");
   fauxmo.addDevice("Alarm");
      
   fauxmo.addDevice("Torpedos");
   fauxmo.addDevice("Phasers");
   fauxmo.addDevice("All Weapons");
   fauxmo.onMessage(callback); 
}

void loop() 
{
  fauxmo.handle();
  torpedosFiring ();
  phasersFiring (); 
  allWeaponsFiring();
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
  
  if ( (strcmp(device_name, "Light") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(RED_LED_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(RED_LED_PIN, LOW);
    }
  }

  if ( (strcmp(device_name, "Alarm") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(BUZZER_PIN, HIGH);
    } 
    else 
    {
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
  
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

   if ( (strcmp(device_name, "Torpedos") == 0) ) 
  {
    if (state) 
    {
      fireTorpedos = HIGH;   
    } 
    else 
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
    }
  }

    if ( (strcmp(device_name, "Phasers") == 0) ) 
  {
    if (state) 
    {
      firePhasers = HIGH; 
    } 
    else 
    {
      digitalWrite(RED_PIN, LOW);
    }
  }

  if ( (strcmp(device_name, "All Weapons") == 0) ) 
  {
    if (state) 
    {
      fireAllWeapons = HIGH;   
    } 
    else 
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
    }
  }
  
}

/* ---------------------------------------------------------------------------
 Special Function to Fire Torpedos
 ----------------------------------------------------------------------------*/
void torpedosFiring ()
{
    if (fireTorpedos == HIGH)
    { 
      delay (1500*!fireAllWeapons);
      digitalWrite(BUZZER_PIN,HIGH);
      digitalWrite(RED_PIN, HIGH);
      delay (300); 
      digitalWrite(GREEN_PIN, HIGH);
      delay (300);
      digitalWrite(BLUE_PIN, HIGH);
      digitalWrite(BUZZER_PIN,LOW);
      delay (500);
      digitalWrite(BUZZER_PIN,HIGH);
      digitalWrite(BLUE_PIN, LOW);
      delay (300);
      digitalWrite(GREEN_PIN, LOW);
      delay (300);
      digitalWrite(RED_PIN, LOW);
      digitalWrite(BUZZER_PIN,LOW);
      fireTorpedos = LOW;     
    }
}

/* ---------------------------------------------------------------------------
 Special Function to Fire Phasers
 ----------------------------------------------------------------------------*/
void phasersFiring ()
{
    if (firePhasers == HIGH)
    { 
      delay (1500*!fireAllWeapons);
      int i = 0; 
      while (i < 15)
      {
        digitalWrite(BUZZER_PIN,HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        delay (100); 
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BUZZER_PIN,LOW);  
        delay (50); 
        i++;   
      }
      firePhasers = LOW;
    }
}

/* ---------------------------------------------------------------------------
 Special Function to Fire all Weapons
 ----------------------------------------------------------------------------*/
void allWeaponsFiring ()
{
    if (fireAllWeapons == HIGH)
    { 
      delay (1500);
      firePhasers = HIGH;
      phasersFiring ();
      delay (200);
      fireTorpedos = HIGH;   
      torpedosFiring ();
      delay (200);
      fireTorpedos = HIGH;
      torpedosFiring ();
      delay (200);
      firePhasers = HIGH;
      phasersFiring ();
      delay (200);
      firePhasers = HIGH;
      phasersFiring ();
      delay (200);    
      fireTorpedos = HIGH;
      torpedosFiring (); 
      fireAllWeapons = LOW;
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
