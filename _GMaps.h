#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
    

const char* host = "https://maps.googleapis.com"; 
  
void _getLocation(){    
    ///////////////////////////////////////////////
    // The section below deals with connection
    // to google maps API and making a GET Request
    ///////////////////////////////////////////////

    //Check to see if Receiver is to be trusted before making request
    if (veribuff[0] == 'A') {
      WiFiClientSecure client;
      const int httpPort = 443;
      if (!client.connect(host, httpPort)) {
        Serial.println("Error: Connection to Google Failed !!!");
        Serial.println("Waiting 2 secs");
        delay(2000);
        return;
      }
      
      Serial.println("Connected to Google Maps!!!");

      // VERY IMPORTANT !!!!
      // String urlKey, just below is where you add your API key 
      // Note the "&key= " at the beginning.

      //Building the GET request
      String urlPiece = "/maps/api/geocode/json?latlng=";
      String urlKey = "&key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0";
      String url = urlPiece + LAT + ',' + LONG + urlKey;

      Serial.print("\nSending Request.");
      Serial.println(url + "\n");

      //Send request
      client.print(String("GET ") + url + "\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");

      unsigned long timeout = millis();

      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println("Error: Client Timeout !!!");
          client.stop();
          return;
        }
      }

      //Check HTTP status
      char status[32] = {0};
      client.readBytesUntil('\r', status, sizeof(status));
      if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
        Serial.print(F("Unexpected response: "));
        Serial.println(status);
        return;
      }

      //Skip HTTP headers
      char endOfHeaders[] = "\r\n\r\n";
      if (!client.find(endOfHeaders)) {
        Serial.println(F("Invalid response"));
        return;
      }

      //Use arduinojson.org/assistant to compute the capacity.
      const size_t bufferSize = JSON_ARRAY_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 120;

      //Allocate JsonBuffer
      DynamicJsonBuffer jsonBuffer(bufferSize);

      //Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(client);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }

      JsonObject& results0 = root["results"][0];
      String results0_formatted_address = results0["formatted_address"];
      Serial.println("#################");
      Serial.println(results0_formatted_address);
      Serial.println("#################\n");
      Serial.print("Address Returned, going to sleep for ");
      Serial.print(TIME_TO_SLEEP);
      Serial.println(" Secs....");
      Serial.flush();
      GPSSerial.flush();
      esp_deep_sleep_start();
    }
}
