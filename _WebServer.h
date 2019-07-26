#include <WebServer.h>
#include "AutoConnect.h"  
#include "PageBuilder.h"  
// Variable to store the HTTP request
String header;

WebServer Server;
AutoConnect   Portal(Server);

WiFiServer WifiServer(80);

int webserverTimer = 0;

 void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void _setupAPWebServer() {
  Serial.println("_setupAPWebServer called");
   Server.on("/", rootPage);
    if (Portal.begin()) {
      Serial.println("WiFi connected: " + WiFi.localIP().toString());
    }
}

void _setupWebServer(){
  // Set web server port number to 80
  Serial.println("WiFiServer _setupServer called"); 
  WifiServer.begin();
  webserverTimer=millis();
  Serial.println("WiFiServer _setupServer end"); 
}


bool _checkWebClientTimer(){
   if (millis() - webserverTimer > _WEBSERVER_INTERVAL ){
      webserverTimer=millis();
      return true;
    } 
    else{
      return false;
    }
}




void _listen(){
  //AP MODE 
  if(!global_wifi_conn_state){
      Serial.println("_listen called:AP MODE Portal.handleClient");
     Portal.handleClient();
  }
  //STATION MODE 
  else
   {
    Serial.println("_listen called:STATION MODE listen for incoming clients");
    WiFiClient client = WifiServer.available();    // listen for incoming clients
    if (client) {                             
      Serial.println("new client");          
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            
        if (client.available()) {             // if there's client data
          char c = client.read();          // read a byte
            if (c == '\n') {                      // check for newline character,
            if (currentLine.length() == 0) {  // if line is blank it means its the end of the client HTTP request
              
              client.println("<!DOCTYPE html>"); // open wrap the web page
 
              client.print("<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta charset='utf-8'><style>#map {height: 100%;}html, body {height: 100%;margin: 0;padding: 0;}</style></head>");
              if(gs_current_latitude=="" || gs_current_longitude=="")  client.print("<body><h1>'Portuguese Water Dog' Server Maps V1." _SUBVERSION " got no coordinates!</h1>");
             else{
                Serial.println("_listen: served page for latitude:"+gs_current_latitude+" longitude:"+gs_current_longitude);
                client.print("<body><h1>'Portuguese Water Dog' Server Maps V1."_SUBVERSION "</h1><div id=\"map\"></div>");
                client.print("<script>function initMap(){var myLatLng =  {lat: "+gs_current_latitude+", lng: "+gs_current_longitude+"} ; var map = new google.maps.Map(document.getElementById('map'),{center: myLatLng,zoom: 18,mapTypeId: 'satellite'} ); var marker = new google.maps.Marker({ position: myLatLng, map: map, title: 'I am here!'}); }</script><script async defer src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap\"></script>");
              }            
              client.print("</body></html>"); // close wrap the web page
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            }
            /*
            client.println("<!DOCTYPE html>");
            client.print("<html><head><style>#map {height: 400px;   width: 100%;  } </style></head><body><h3>My Google Maps Demo</h3><!--The div element for the map --><div id=\"map\"></div><script>function initMap() {var uluru = {lat: -25.344, lng: 131.036};var map = new google.maps.Map( document.getElementById('map'), {zoom: 4, center: uluru});var marker = new google.maps.Marker({position: uluru, map: map});}</script><script async defer src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap\"></script>");
            client.print("</body></html>"); 
            client.println();
            break;
            }
            */
            
            else {   currentLine = ""; }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
           // here you can check for any keypresses if your web server page has any
        }
      }
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
      }
    }
}
