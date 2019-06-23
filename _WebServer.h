
// Variable to store the HTTP request
String header;


// Set web server port number to 80
WiFiServer server(80);

int webserverTimer = 0;


void _setupWebServer(){
  Serial.println("WiFiServer _setupServer called"); 
  server.begin();
  webserverTimer=millis();
  Serial.println("WiFiServer _setupServer end"); 
}

bool _webClientTimer(){
   if (millis() - webserverTimer > WEBSERVER_INTERVAL ){
      webserverTimer=millis();
      return true;
    } 
    else{
      return false;
    }
}

void _listen(){
  //Serial.println("WiFiServer _listen called"); 
  WiFiClient client = server.available();    // listen for incoming clients
  if (client) {                             
    Serial.println("new client");          
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            
      if (client.available()) {             // if there's client data
        char c = client.read();          // read a byte
          if (c == '\n') {                      // check for newline character,
          if (currentLine.length() == 0) {  // if line is blank it means its the end of the client HTTP request
            client.println("<!DOCTYPE html>"); // open wrap the web page
            client.print("<html><head><meta name='viewport' content='initial-scale=1.0'><meta charset='utf-8'><style>#map {height: 100%;}html, body {height: 100%;margin: 0;padding: 0;}</style></head>");
 //           client.print("<body><h1>ESP32 Server Maps 'Portuguese Water Dog'</h1><div id='map'></div><script>function initMap(){var map = new google.maps.Map(document.getElementById('map'),{center: {lat: 29.975995, lng: 31.130806},zoom: 16,mapTypeId: 'satellite'});}</script><script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap'></script>");
 //            client.print("<body><h1>'Portuguese Water Dog' Server Maps</h1><div id='map'></div><script>function initMap(){var map = new google.maps.Map(document.getElementById('map'),{center: {lat: "+g_latitude+", lng: "+g_longitude+"},zoom: 16,mapTypeId: 'satellite'});}</script><script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap'></script>");
             client.print("<body><h1>'Portuguese Water Dog' Server Maps</h1><div id='map'></div><script>function initMap(){var myLatLng =  {lat: "+g_latitude+", lng: "+g_longitude+"} ; var map = new google.maps.Map(document.getElementById('map'),{center: myLatLng,zoom: 16,mapTypeId: 'satellite'} ); var marker = new google.maps.Marker({ position: myLatLng, map: map, title: 'I am here!'}); }</script><script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap'></script>");
 
            
            client.print("</body></html>"); // close wrap the web page
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {   currentLine = ""; }
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
