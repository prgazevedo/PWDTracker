
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
#ifndef _GLOBAL_H
#define _GLOBAL_H

//MAC Address
uint64_t chipid;
String s_chipid;
//GPS 
//Struct to hold location values
typedef struct CoordToSend
{
   uint16_t deg;
   uint32_t billionths;
   bool negative;
public:
   CoordToSend() : deg(0), billionths(0), negative(false){}
}coordToSend;

//Struct to hold all values
typedef struct GpsData{
 coordToSend latitude,longitude;
 float hdop_value;
 float vdop_value;
 int altitude_value;
 int fix_age_value;
 uint32_t fix_time_value;
 int satellites_value;
}gpsData;

gpsData gdata;

int payload_gdata_size = 0;
//Used for outputing to serial and html
String gs_current_latitude;
String gs_current_longitude;
bool bFirst = true;
String gs_rcv_latitude;
String gs_rcv_longitude;
String gs_current_hdop;
String gs_current_vdop;
float gf_current_hdop;
float gf_current_vdop;
String gs_current_altitude;
String gs_current_fix_age;
String gs_current_fix_time;
String gs_current_satellites;
String gspath;

typedef struct GpsPath{
  String lat;
  String lng;
}gpsPath;

typedef union {
    float f[2];         // Assigning fVal.f will also populate fVal.bytes;
    unsigned char bytes[8];   // Both fVal.f and fVal.bytes share the same 4 bytes of memory.
} floatArr2Val;
floatArr2Val latlong; //LAT and Long.
float latitude;
float longitude;

//LORA 
//LORA data struct to send
typedef struct {
  unsigned int ID;
  long timeMillis;
}PacketData;

//LORA packet data
PacketData pdata;
int payload_pdata_size = 0;

//LORA radio signal data
int rssi_value = 0;
int snr_value = 0;

//WIFI IP
String global_wifi_IP = "Not Connected";
String gcurrent_ssid = _SSID_NAME;
//WIFI Connection State
bool global_wifi_conn_state = false;
//BATTERY
String sVBAT ="";
#endif
