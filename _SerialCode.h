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
#include "_Global.h" 
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

String _log_additional_data(){
    return String("HDOP:"+gs_current_hdop+\
    " VDOP:"+gs_current_vdop+\
    " altitude:"+gs_current_altitude+\
    " fix_age:"+gs_current_fix_age+\
    " fix_time:"+gs_current_fix_time+\
    " satellites:"+gs_current_satellites);
}

void _log_packet_data( ){
   if(_ROLE==0)
    {
      Serial.println("packet sent:");
    }
    else{
      Serial.println("packet received:");
    }

   writeSerial(" counter:"+String(pdata.ID));
   writeSerial(" time_stamp:"+String(pdata.timeMillis));
   writeSerial(" latitude:"+gs_current_latitude);
   writeSerial(" longitude:"+gs_current_longitude);
   writeSerial(" payload_pdata_size size:"+String(payload_pdata_size,DEC));
    writeSerial(" payload_gdata_size size:"+String(payload_gdata_size,DEC));
   writeSerial(" Additional data:"+_log_additional_data());
   if(_ROLE==1)
    {
        writeSerial(" RSSI: "+String(rssi_value));
        writeSerial(" SNR: "+String(snr_value));
    }
 

}
