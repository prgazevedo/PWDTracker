/*
 * Copyright 2018 Pedro Azevedo (prgazevedo@gmail.com)
 * and other contributors as indicated by the @author tags.
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


#include "_Definitions.h" 
#include "_LED.h" 
#include "_Global.h"
#include "_Path.h"

// count of outgoing messages
byte packetID = 0;            
//Time of last Lora data packet
long lastSendTime = 0;

/*
byte destinationAddress = 0xFF;      // destination to send to 11111111
byte localAddress = 0x01;     // address of this device 00000001
*/
void _LoraInit(){
  writeSerial("_LoraInit");
    //LORA INIT
  SPI.begin(_SCK,_MISO,_MOSI,_SS); //INIT SERIAL WITH LORA
  LoRa.setPins(_SS,_RST,_DI00); //PINOUT to be used by LORA library (deve ser chamado antes do LoRa.begin)
  
  //INIT LORA BAND
  if (!LoRa.begin(_BAND))
  {
    OLED_write("Starting LoRa failed!");
    writeSerial("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc();
  if(_MAXPOWER){
    LoRa.setTxPower(_MAX_TX_POWER);
    writeSerial("LoRa is using Power:"+String(_MAX_TX_POWER));
  }
  
  //For Range
  if(_LONG_RANGE)
  {
     writeSerial("LoRa is using Long Range settings- SignalBandwidth: "+String(_SIGNAL_BANDWIDTH)+"Hz SpreadingFactor: "+String(_SPREADING_FACTOR)+"(6-12) CodingRate:"+String(_CODING_RATE)); 
     LoRa.setSignalBandwidth(_SIGNAL_BANDWIDTH); // Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, and 250E3.
     LoRa.setSpreadingFactor(_SPREADING_FACTOR); // Supported values are between 6 and 12
     LoRa.setCodingRate4(_CODING_RATE); // Supported values are between 5 and 8
  }
  
  OLED_write("LoRa Initial success!");
  writeSerial("LoRa Initial success!");
  
}



void _sendPacket() {
   writeSerial("_sendMessage");

  LoRa.beginPacket();
  writeSerial("beginPacket");
  payload_pdata_size = sizeof(pdata);
  payload_gdata_size = sizeof(gdata);
  LoRa.write(payload_pdata_size);      // pdata size
  pdata.ID = packetID++;
  pdata.timeMillis = millis();                        // increment message ID
  //send pdata: ID and TimeStamp
  LoRa.write((uint8_t*)&pdata, sizeof(pdata));
   //send gdata: Latitude and Longitude
  LoRa.write(payload_gdata_size);      // pdata size
  LoRa.write((uint8_t*)&gdata, sizeof(gdata));


  LoRa.endPacket(); 
  LoRa.sleep();
}


void _postParse(){
  if(sizeof(pdata)>0){
    gs_current_latitude = getCoordString(gdata.latitude);
    gs_current_longitude = getCoordString(gdata.longitude);
    getAdditionalDataString(gdata);
    _log_packet_data();
    _OLED_print_data();
     _addCoordsToPath(gs_current_latitude,gs_current_longitude);
     _encodePath();
  }
}

void _Send(){
  writeSerial("_LoraSendPacket"); 
  
   _sendPacket(); 
   _postParse();
 

}




bool _parsePacket() {
  writeSerial("LoRa _parsePacket Called");
   //read pdata: ID and TimeStamp      
   payload_pdata_size = LoRa.read();    
   if(payload_pdata_size>0)
   {       
     LoRa.readBytes((uint8_t*)&pdata, sizeof(pdata));
     payload_gdata_size = LoRa.read();
     //read gdata: Latitude and Longitude + additional attributes
     if(payload_gdata_size>0) 
     LoRa.readBytes((uint8_t*)&gdata, sizeof(gdata));
     
      // payload of packet
      /*
      while (LoRa.available())
      {
        payload_data += (char)LoRa.read();
      }
      */
      rssi_value = LoRa.packetRssi();
      snr_value = LoRa.packetSnr();
      _postParse();
      return true;
   }
   else return false;
   
}


void _Receive(){
  //writeSerial("LoRa _parsePacket");
  //parsePacket:check if a packet was received
  int packetSize = LoRa.parsePacket();
  if (packetSize>0) { 
    writeSerial("LoRa _Receive calls _parsePacket");
    if(_parsePacket()){
       packetID++;
       writeSerial("LoRa _Receive _parsePacket success");  
    }else{
       writeSerial("LoRa _Receive _parsePacket failure (size of packet?)");  
    }
  }
}

  
bool _sendTimer(){
  if (millis() - lastSendTime > _LORA_SEND_INTERVAL){
      //time since last send
      lastSendTime = millis();
      return true;
 } 
 else return false;

}

bool _receiveTimer(){
  if (pdata.timeMillis > lastSendTime + _LORA_SEND_INTERVAL ){
      //time since last send
      lastSendTime = pdata.timeMillis;
      return true;
 } 
 else return false;

}
