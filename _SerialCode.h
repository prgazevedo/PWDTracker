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

void writeSerial(String toWrite)
{
  if(_ROLE==0)
  {
    Serial.println("Module SENDER:"+toWrite);
  }
  else{
    Serial.println("Module RECEIVER:"+toWrite);
  }
}

void _SerialInit()
{

  Serial.begin(115200);
  while (!Serial);
  delay(50); 
  writeSerial("_SerialInit");
}

void log_packet_data(){
   if(_ROLE==0)
    {
      Serial.println("packet sent:");
    }
    else{
      Serial.println("packet received:");
    }

   writeSerial(" counter:"+String(pdata.ID));
   writeSerial(" time_stamp:"+String(pdata.timeMillis));
   writeSerial(" latitude:"+g_latitude);
   writeSerial(" longitude:"+g_longitude);
   writeSerial(" payload_pdata_size size:"+String(payload_pdata_size,DEC));
    writeSerial(" payload_gdata_size size:"+String(payload_gdata_size,DEC));
   
   if(_ROLE==1)
    {
        writeSerial(" RSSI: "+String(rssi_value));
        writeSerial(" SNR: "+String(snr_value));
    }
 

}
