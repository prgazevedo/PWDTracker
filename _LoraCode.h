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


#include "_const.h" 
#include "_LED.h" 

void _LoraInit(){
  writeSerial("_LoraInit");
    //LORA INIT
  SPI.begin(SCK,MISO,MOSI,SS); //INIT SERIAL WITH LORA
  LoRa.setPins(SS,RST,DI00); //PINOUT to be used by LORA library (deve ser chamado antes do LoRa.begin)
  
  //INIT LORA BAND
  if (!LoRa.begin(BAND))
  {
    OLED_write("Starting LoRa failed!");
    writeSerial("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc();

  //For Range
  LoRa.setSignalBandwidth(10.4E3); // Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, and 250E3.
  LoRa.setSpreadingFactor(7); // Supported values are between 6 and 12
  LoRa.setCodingRate4(8); // Supported values are between 5 and 8
  LoRa.setTxPower(18); // Supported values are between 2 and 17

  
  OLED_write("LoRa Initial success!");
  writeSerial("LoRa Initial success!");
  delay(1000);
}



void _sendPacket() {
   writeSerial("_sendMessage");
  LoRa.beginPacket();
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
}


void _postParse(){
  if(sizeof(pdata)>0){
    g_latitude = getCoordString(gdata.latitude);
    g_longitude = getCoordString(gdata.longitude);
    log_packet_data();
    OLED_COMMS_DATA();
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
     //read gdata: Latitude and Longitude
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
       writeSerial("LoRa _Receive _parsePacket success");  
    }else{
       writeSerial("LoRa _Receive _parsePacket failure (size of packet?)");  
    }
  }
}

  
bool _sendTimer(){
  if (millis() - lastSendTime > LORA_SEND_INTERVAL){
      //time since last send
      lastSendTime = millis();
      return true;
 } 
 else return false;

}

bool _receiveTimer(){
  if (pdata.timeMillis > lastSendTime + LORA_SEND_INTERVAL ){
      //time since last send
      lastSendTime = pdata.timeMillis;
      return true;
 } 
 else return false;

}
