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
 
  OLED_write("LoRa Initial success!");
  writeSerial("LoRa Initial success!");
  delay(1000);
}



void _sendPacket() {
   writeSerial("_sendMessage");
  LoRa.beginPacket();
  LoRa.write(localAddress);            // add sender address
  LoRa.write(destinationAddress);      // add destination address
  pdata.ID = packetID++;
  pdata.timeMillis = millis();                        // increment message ID
  //send pdata: ID and TimeStamp
  LoRa.write((uint8_t*)&pdata, sizeof(pdata));
   //send gdata: Latitude and Longitude
  LoRa.write((uint8_t*)&gdata, sizeof(gdata));
  payload_size = payload_data.length();
  LoRa.write(payload_size);           // add payload length
  LoRa.print(payload_data);                // add payload

  LoRa.endPacket(); 
}

void _Send(){
  writeSerial("_LoraSendPacket"); 
  
   _sendPacket(); 
  log_packet_data();
  OLED_COMMS_DATA();
 

}

void _postParse(){
  g_latitude = String(gdata.latitude,10);
  g_longitude = String(gdata.longitude,10);
  log_packet_data();
  OLED_COMMS_DATA();
}


void _parsePacket() {
  writeSerial("LoRa _parsePacket Called");
  payload_data="";
   localAddress = LoRa.read(); 
   destinationAddress = LoRa.read();  
   //read pdata: ID and TimeStamp               
   LoRa.readBytes((uint8_t*)&pdata, sizeof(pdata));
   //read gdata: Latitude and Longitude
   LoRa.readBytes((uint8_t*)&gdata, sizeof(gdata));
   payload_size = LoRa.read();
    // payload of packet
    while (LoRa.available())
    {
      payload_data += (char)LoRa.read();
    }
    rssi_value = LoRa.packetRssi();
    snr_value = LoRa.packetSnr();

   _postParse();
}


void _Receive(){
  //writeSerial("LoRa _parsePacket");
  //parsePacket:check if a packet was received
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    writeSerial("LoRa _onReceive");
    _parsePacket();  
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
