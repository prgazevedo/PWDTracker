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
#include "_Definitions.h"
#include "_Global.h"
#include "_WifiManager.h"
#include "_File.h"
#include "_MQTT.h"
#include "_httpServer.h"
#include "_lowPower.h"



void setup()
{
  writeSerial("PWD setup Begin");
  //LED CFG
  pinMode(25,OUTPUT);
  _SerialInit();
  _OledInit();
  _LoraInit();
  _setupMAC();
  _Fileinit();
  writeSerial(_readPassword());
  _setupPower();
  #if _ROLE == 0
   writeSerial("setup Sender called");
    
    // Setup the GPS
    _SetupGPS();
    // Setup lower CPU frequency
    _switch_freq(CPU_FREQ);
  #endif
  #if _ROLE == 1
      // only compile MQTT if in receiver mode
      writeSerial("setup Receiver called");
      //_setupWifiConnection();
      
      _setupAutoconnect();

      _connectMQTTServer();
      //_setupWebServer();
      _setupHTTPSSERVER();
      OLED_write("Waiting for LORA Packets");
  #endif

  writeSerial("setup Ended");

}



void loop()
{ 
  if(_readPowerTimer()){
    _readBatteryPower();
  }
  if(_ROLE==0)
  {
     //_light_sleep();

     //_LEDBlink(); //SAVE POWER
     if (_sendTimer()){  
          
        _getGPS();
        _Send();
      }


  }
  else
  {
    //_ROLE==RECEIVER
    _Receive();
    if(_receiveTimer())
    {
      Serial.println("MQTT  _publishLocationData called"); 
       _publishLocationData();
    }
    if(_checkhttpserverTimer())
    {
      
      if(bPortalStarted){
        Serial.println("_handlePortal called"); 
        _handlePortal();
      }
      else {
        Serial.println("_httpServerLoop called"); 
        _updateWifiState();
        //_checkWifiState();
        
        _httpServerLoop();
      }

    }
    _LEDBlink();
    
  }

}
