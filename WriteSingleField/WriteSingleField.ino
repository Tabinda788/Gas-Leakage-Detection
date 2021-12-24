/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: ESP8266 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires ESP8266WiFi library and ESP8622 board add-on. See https://github.com/esp8266/Arduino for details.
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;
int gasAnalogPin = A0;
int sensorThres = 100;
const unsigned char Passive_buzzer = 4;

void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  pinMode(Passive_buzzer,OUTPUT);
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {


  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }



  int gasReading = analogRead(gasAnalogPin);
  Serial.println(gasReading);
  if(gasReading>240){
    digitalWrite(Passive_buzzer,HIGH) ; 
    Serial.println("Gas is leaking");
      int x = ThingSpeak.writeField(myChannelNumber, 1, gasReading, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    //Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  }
  else{
     int x = ThingSpeak.writeField(myChannelNumber, 1, gasReading, myWriteAPIKey);
    if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    //Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    digitalWrite(Passive_buzzer,LOW);
  
  }
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
//   pieces of information in a channel.  Here, we write to field 1.



  //delay(2000); // Wait 20 seconds to update the channel again
}