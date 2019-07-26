
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
  gs_current_altitude=gpsData.altitude_value;
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
 /*
  char temp[2]={' ','\0'};
  latitude  = gps.location.lat();
  longitude = gps.location.lng();

  if((latitude && longitude) && latitude != latlong.f[0]
      && longitude != latlong.f[1])
      {     
         Serial.println("_encodeLocation has latitude/longitude");
        latlong.f[0] = latitude;
        latlong.f[1] = longitude;
         
        Serial.println(sEncode);
        for(int i = 0; i < 8; i++){
           temp[0]=latlong.bytes[i];
            Serial.print(latlong.bytes[i], HEX);
        }
        //Serial.println(sEncode);
      }
      */
   String sEncode="LatLong- latitude: "+gs_current_latitude+ " and longitude: "+gs_current_longitude;
  return sEncode;
}

void _writeSerialGPS(){
  
  if (gps.location.isUpdated())
  {
    Serial.print(F("LOCATION   Fix Age="));
    Serial.print(gps.location.age());
    Serial.print(F("ms Raw Lat="));
    Serial.print(gps.location.rawLat().negative ? "-" : "+");
    Serial.print(gps.location.rawLat().deg);
    Serial.print("[+");
    Serial.print(gps.location.rawLat().billionths);
    Serial.print(F(" billionths],  Raw Long="));
    Serial.print(gps.location.rawLng().negative ? "-" : "+");
    Serial.print(gps.location.rawLng().deg);
    Serial.print("[+");
    Serial.print(gps.location.rawLng().billionths);
    Serial.print(F(" billionths],  Lat="));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.println(gps.location.lng(), 6);
  }

  else if (gps.date.isUpdated())
  {
    Serial.print(F("DATE       Fix Age="));
    Serial.print(gps.date.age());
    Serial.print(F("ms Raw="));
    Serial.print(gps.date.value());
    Serial.print(F(" Year="));
    Serial.print(gps.date.year());
    Serial.print(F(" Month="));
    Serial.print(gps.date.month());
    Serial.print(F(" Day="));
    Serial.println(gps.date.day());
  }

  else if (gps.time.isUpdated())
  {
    Serial.print(F("TIME       Fix Age="));
    Serial.print(gps.time.age());
    Serial.print(F("ms Raw="));
    Serial.print(gps.time.value());
    Serial.print(F(" Hour="));
    Serial.print(gps.time.hour());
    Serial.print(F(" Minute="));
    Serial.print(gps.time.minute());
    Serial.print(F(" Second="));
    Serial.print(gps.time.second());
    Serial.print(F(" Hundredths="));
    Serial.println(gps.time.centisecond());
  }

  else if (gps.speed.isUpdated())
  {
    Serial.print(F("SPEED      Fix Age="));
    Serial.print(gps.speed.age());
    Serial.print(F("ms Raw="));
    Serial.print(gps.speed.value());
    Serial.print(F(" Knots="));
    Serial.print(gps.speed.knots());
    Serial.print(F(" MPH="));
    Serial.print(gps.speed.mph());
    Serial.print(F(" m/s="));
    Serial.print(gps.speed.mps());
    Serial.print(F(" km/h="));
    Serial.println(gps.speed.kmph());
  }

  else if (gps.course.isUpdated())
  {
    Serial.print(F("COURSE     Fix Age="));
    Serial.print(gps.course.age());
    Serial.print(F("ms Raw="));
    Serial.print(gps.course.value());
    Serial.print(F(" Deg="));
    Serial.println(gps.course.deg());
  }

  else if (gps.altitude.isUpdated())
  {
    Serial.print(F("ALTITUDE   Fix Age="));
    Serial.print(gps.altitude.age());
    Serial.print(F("ms Raw="));
    Serial.print(gps.altitude.value());
    Serial.print(F(" Meters="));
    Serial.print(gps.altitude.meters());
    Serial.print(F(" Miles="));
    Serial.print(gps.altitude.miles());
    Serial.print(F(" KM="));
    Serial.print(gps.altitude.kilometers());
    Serial.print(F(" Feet="));
    Serial.println(gps.altitude.feet());
  }

  else if (gps.satellites.isUpdated())
  {
    Serial.print(F("SATELLITES Fix Age="));
    Serial.print(gps.satellites.age());
    Serial.print(F("ms Value="));
    Serial.println(gps.satellites.value());
  }

  else if (gps.hdop.isUpdated())
  {
    Serial.print(F("HDOP       Fix Age="));
    Serial.print(gps.hdop.age());
    Serial.print(F("ms raw="));
    Serial.print(gps.hdop.value());
    Serial.print(F(" hdop="));
    Serial.println(gps.hdop.hdop());
  }

  else if (millis() - last > 5000)
  {
    Serial.println();
    if (gps.location.isValid())
    {
      static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
      double distanceToLondon =
        TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          LONDON_LAT, 
          LONDON_LON);
      double courseToLondon =
        TinyGPSPlus::courseTo(
          gps.location.lat(),
          gps.location.lng(),
          LONDON_LAT, 
          LONDON_LON);

      Serial.print(F("LONDON     Distance="));
      Serial.print(distanceToLondon/1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToLondon, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToLondon));
      Serial.println(F("]"));
    }

    Serial.print(F("DIAGS      Chars="));
    Serial.print(gps.charsProcessed());
    Serial.print(F(" Sentences-with-Fix="));
    Serial.print(gps.sentencesWithFix());
    Serial.print(F(" Failed-checksum="));
    Serial.print(gps.failedChecksum());
    Serial.print(F(" Passed-checksum="));
    Serial.println(gps.passedChecksum());

    if (gps.charsProcessed() < 10)
      Serial.println(F("WARNING: No GPS data.  Check wiring."));

    last = millis();
    Serial.println();
  }
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
    _encodeLocationSummary();
    Serial.println("_encodeLocationSummary called");
    _encodeLocation();
    Serial.println("_encodeLocation called");
    _writeSerialGPS();
    Serial.println("_writeSerialGPS called");
   
}
