
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


#include "_GPSUtils.h"
#include "_Global.h"


/* sample test stream
const char *testStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

  void _encodeTestStream(){
   while (*testStream)
    gps.encode(*testStream++);
}
*/


TinyGPSCustom vdop(gps, "GPGSA", 17);



// For stats that happen every 5 seconds
unsigned long last = 0UL;

// The serial connection to the GPS device
void _SetupGPS()
{

  Serial2.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.print(F("Using TinyGPS++ library v. ")); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));

  Serial.println();
}

void _writeGPSHeader(){
  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card   Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----       RX    RX        Fail"));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------"));
}




void _encodeGPS(){
  // Dispatch incoming characters
  while (Serial2.available() > 0)
  {
    char c;
    c = Serial2.read();
    Serial.write(c);
    gps.encode(c);
  }
}


void _encodeLocationSummary(){
  _writeGPSHeader();
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

   printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  Serial.println();
  
  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

GpsData getRawGPSData(){
    GpsData gpsData;
    gpsData.latitude.negative = gps.location.rawLat().negative;
    gpsData.latitude.deg = gps.location.rawLat().deg;
    gpsData.latitude.billionths =gps.location.rawLat().billionths;
    gpsData.longitude.negative = gps.location.rawLng().negative;
    gpsData.longitude.deg = gps.location.rawLng().deg;
    gpsData.longitude.billionths =gps.location.rawLng().billionths;
    //additional data
    gpsData.hdop_value=gps.hdop.hdop();
    gpsData.vdop_value=String(vdop.value()).toFloat();
    gpsData.altitude_value=gps.altitude.meters();
    gpsData.fix_age_value=gps.location.age();
    gpsData.fix_time_value=gps.time.value();
    gpsData.satellites_value=gps.satellites.value();
    
    return gpsData;
}
  


double getCoord(coordToSend rawData){
   double ret = rawData.deg + rawData.billionths / 1000000000.0;
   return rawData.negative ? -ret : ret;
}

void getAdditionalDataString(GpsData gpsData)
{
  gf_current_hdop=gpsData.hdop_value;
  gf_current_vdop=gpsData.vdop_value;
  gs_current_hdop=String(gf_current_hdop);
  gs_current_vdop=String(gf_current_vdop);
  gs_current_altitude=String(gpsData.altitude_value);
  gs_current_fix_age=String(gpsData.fix_age_value);
  gs_current_fix_time=String(gpsData.fix_time_value);
  gs_current_satellites=String(gpsData.satellites_value);
}

String getCoordString(coordToSend rawData){
     double ret = getCoord(rawData);
    char TempString[10];  //  Hold The Convert Data
    dtostrf(ret,3,6,TempString);
    // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
   return String(TempString);
}


String _encodeLocation(){
  Serial.println("_encodeLocation called"); 
  
  gdata = getRawGPSData();
  gs_current_latitude = getCoordString(gdata.latitude);
  gs_current_longitude = getCoordString(gdata.longitude);
  getAdditionalDataString(gdata);
 Serial.println("_encodeLocation has latitude: "+gs_current_latitude+ " and longitude: "+gs_current_longitude);
 Serial.println("_encodeLocation has addtional data: "+_log_additional_data());
   String sEncode="LatLong- latitude: "+gs_current_latitude+ " and longitude: "+gs_current_longitude;
  return sEncode;
}


void basicGPSDebug(){
  if (Serial2.available()) Serial.println(Serial2.read());
  else Serial.println("No data available");
}

void _getGPS()
{
    Serial.println("_getGPS called");
    //basicGPSDebug();
    
    _encodeGPS();
    //_encodeTestStream();
    //_encodeLocationSummary();
    //Serial.println("_encodeLocationSummary called");
    _encodeLocation();
    Serial.println("_encodeLocation called");
 
 
}
