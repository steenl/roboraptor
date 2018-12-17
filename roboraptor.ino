// Voice controlled robot raptor instructable
//
// By SteenL Portland 2018
// based in part from Igor Fonseca Albuquerque 2018
// based on Instructables IoT Class by Becky Stern 2017
// based on the Adafruit IO Subscription Example

// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************ Adafruit IO Configuration *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME    "steenl"
#define IO_KEY         "b5f5b13f4ce34e138efbb0208b966df7"

/******************************* WIFI Configuration **************************************/

#define WIFI_SSID       "xxxxx"
#define WIFI_PASS       "*******"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/************************ Main Program Starts Here *******************************/
#include <ESP8266WiFi.h>
#include <AdafruitIO.h>
//#include <Adafruit_MQTT.h>
// #include <ArduinoHttpClient.h>

// These are used to set the direction of the bridge driver.
//SKL Aug9 Seems specific to nodeMCU comment out and adapt.
#define DLY 300
// 0,2,4,5,12,13,14,15     good outputs
#define RD1 13
#define RD2 14
#define LD1 12
#define LD2 15
#define HD1 0
#define HD2 2
#define TiltD1 4
#define TiltD2 5
//#define TailD1 0
//#define TailD2 2
AdafruitIO_Feed *command = io.feed("robovoice"); // Set up the 'command' feed

// SETUP
void setup()
{
  // Configure pins
  pinMode(LED_BUILTIN, OUTPUT);
     pinMode     (RD1,   OUTPUT);
     digitalWrite(RD1, LOW);
     pinMode     (RD2,   OUTPUT);
     digitalWrite(RD2, LOW);
   pinMode     (LD1,   OUTPUT);
   digitalWrite(LD1, LOW);
   pinMode     (LD2,   OUTPUT);
   digitalWrite(LD2, LOW);
     pinMode     (HD1,   OUTPUT);
     digitalWrite(HD1, LOW);
     pinMode     (HD2,   OUTPUT);
     digitalWrite(HD2, LOW);
     pinMode     (TiltD1, OUTPUT);
     digitalWrite(TiltD1, LOW);
     pinMode     (TiltD2, OUTPUT);
     digitalWrite(TiltD2, LOW);
//     pinMode     (TailD1,   OUTPUT);
//     digitalWrite(TailD1, LOW);
//     pinMode     (TailD2,   OUTPUT);
//     digitalWrite(TailD2, LOW);
       // Start serial communication
  Serial.begin(115200);
  delay(10);

    
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
    
  // set up a message handler for the 'command' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  command->onMessage(handleMessage);  

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(50);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

// MAIN CODE
void loop()
{
  //Serial.print("Entering main loop");
  io.run();
}

// this part of the code runs whenever there's a new message on Adafruit.io feed
void handleMessage(AdafruitIO_Data *data) {
  
  String commandStr = data->toString(); // store the incoming commands in a string
  
  // received message
  Serial.print("received <- ");
  Serial.println(commandStr);

  String cmd;
  int angle;

  // separate text and number ingredients
  for (int i = 0; i < commandStr.length(); i++) {
    if (commandStr.substring(i, i+1) == ":") {
      cmd = commandStr.substring(0, i);
      angle = commandStr.substring(i+1).toInt();
      break;
    }
  }

  // print command
  Serial.println(cmd);
  Serial.println(angle);
  
  // perform movements
// rear
  if (cmd.equalsIgnoreCase("rear")) {    
    Serial.println("rearing now");
    for (int i=0; i <angle; i++) {
    digitalWrite(LED_BUILTIN, LOW);
         digitalWrite(TiltD1, HIGH);
         digitalWrite(TiltD2,  LOW);
         delay(1000);
         digitalWrite(TiltD1, LOW);
         digitalWrite(TiltD2, HIGH);
         delay(1000); }
    Serial.println("Stop rotating");
       digitalWrite(TiltD1, LOW);
       digitalWrite(TiltD2, LOW);
       digitalWrite(LED_BUILTIN, HIGH);
     }
// head rotate
  if (cmd.equalsIgnoreCase("rotate")) {    
    Serial.println("rotating now");
    for (int i=0; i <angle; i++) {
    digitalWrite(LED_BUILTIN, LOW);
         digitalWrite(HD1, HIGH);
         digitalWrite(HD2, LOW);
       delay(700);
         digitalWrite(HD1, LOW);
         digitalWrite(HD2, HIGH);
         delay(700); 
   }
    Serial.println("Stop rotating");
       digitalWrite(HD1, LOW);
       digitalWrite(HD2, LOW);
       digitalWrite(LED_BUILTIN, HIGH);
     }
  // forward
  if (cmd.equalsIgnoreCase("forward")){    
    Serial.println("movin forward");
    digitalWrite(LED_BUILTIN, LOW);
    for (int i=0; i <angle; i++) {
       digitalWrite(RD1, HIGH);
       digitalWrite(RD2, LOW);
       digitalWrite(LD1, LOW);
       digitalWrite(LD2, HIGH);
       delay(DLY);
       digitalWrite(RD1, LOW);
       digitalWrite(RD2, HIGH);
       digitalWrite(LD1, HIGH);
       digitalWrite(LD2, LOW);
       //digitalWrite(mottail, LOW);
       delay(DLY);
    }  //loop forward count  
   //    delay(angle*1000);  //milliseconds
    Serial.println("Stop movin");
       digitalWrite(RD1, LOW);
       digitalWrite(RD2, LOW);
       digitalWrite(LED_BUILTIN, HIGH);
      }  // end if
} //end mainloop
