/*
 * Copyright 2018 Pedro Azevedo (prgazevedo@gmail.com)
 * and other contributors as indicated by the @author tags.
 * 
 * Based on: 
 * https://github.com/mikalhart/TinyGPSPlus
 * https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
 * https://github.com/sandeepmistry/arduino-LoRa
 **********************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <SPI.h> //Serial Comms Lib
#include <LoRa.h> //Lora Comms Lib
#include "SSD1306.h" //OLED display Lib
#include "_CStrings.h" //CString mem reduction code
#include "_SerialCode.h" //Serial lib
#include "_oledCode.h" //OLED code
#include "_GPSCode.h" //GPS code
#include "_LoraCode.h" //Lora Code
#include "_const.h"
#include "images.h" //Logo
#include "_MQTT.h"



void setup()
{
  writeSerial("setup Begin");
  //LED CFG
  pinMode(25,OUTPUT);
  _SerialInit();
  _OledInit();
  _LoraInit();
  #if _ROLE == 0
   writeSerial("setup Sender called");
    
    // Setup the GPS
    _SetupGPS();
  #endif
  #if _ROLE == 1
      // only compile MQTT if in receiver mode
      writeSerial("setup Receiver called");
      _setupMQTT();
  #endif
  
  writeSerial("setup Ended");

}

bool timer(){
  if (millis() - lastSendTime > INTERVAL){
      //time since last send
      lastSendTime = millis();
      return true;
 } 
 else return false;

}

bool newData(){
  if (pdata.timeMillis > lastSendTime + INTERVAL ){
      //time since last send
      lastSendTime = pdata.timeMillis;
      return true;
 } 
 else return false;

}


void loop()
{ 

  if(_ROLE==0)
  {
     if (timer()){
      
     //_ROLE==SENDER
      _Send();
      _LEDBlink();
      _getGPS();
     }
  }
  else
  {
    //_ROLE==RECEIVER
    _Receive();
    if(newData())
    {
       _publishData();
    }
    _LEDBlink();
    
  }

}
