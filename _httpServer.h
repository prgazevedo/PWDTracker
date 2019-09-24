/**
 * Example for the ESP32 HTTP(S) Webserver
 *
 * IMPORTANT NOTE:
 * To run this script, your need to
 *  1) Enter your WiFi SSID and PSK below this comment
 *  2) Make sure to have certificate data available. You will find a
 *     shell script and instructions to do so in the library folder
 *     under extras/
 *
 * This script will install an HTTP Server on port 80 and an HTTPS server
 * on port 443 of your ESP32 with the following functionalities:
 *  - Show simple page on web server root
 *  - 404 for everything else
 *
 * The comments in this script focus on making both protocols available,
 * for setting up the server itself, see Static-Page.
 */

// Include certificate data (see note above)
#include "cert.h"
#include "private_key.h"



// Includes for the server
// Note: We include HTTPServer and HTTPSServer
#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

// The HTTPS Server comes in a separate namespace. For easier use, include it here.
using namespace httpsserver;
int httpserverTimer = 0;
// Create an SSL certificate object from the files included above
SSLCert cert = SSLCert(
  example_crt_DER, example_crt_DER_len,
  example_key_DER, example_key_DER_len
);

// First, we create the HTTPSServer with the certificate created above
HTTPSServer secureServer = HTTPSServer(&cert);

// Additionally, we create an HTTPServer for unencrypted traffic
HTTPServer insecureServer = HTTPServer();

// Declare some handler functions for the various URLs on the server
void handleRoot(HTTPRequest * req, HTTPResponse * res);
void handle404(HTTPRequest * req, HTTPResponse * res);

void _setupHTTPSSERVER() {

  // For every resource available on the server, we need to create a ResourceNode
  // The ResourceNode links URL and HTTP method to a handler function
  ResourceNode * nodeRoot = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * node404  = new ResourceNode("", "GET", &handle404);

  // Add the root node to the servers. We can use the same ResourceNode on multiple
  // servers (you could also run multiple HTTPS servers)
  secureServer.registerNode(nodeRoot);
  insecureServer.registerNode(nodeRoot);

  // We do the same for the default Node
  secureServer.setDefaultNode(node404);
  insecureServer.setDefaultNode(node404);

  Serial.println("Starting HTTPS server...");
  secureServer.start();
  Serial.println("Starting HTTP server...");
  insecureServer.start();
  if (secureServer.isRunning() && insecureServer.isRunning()) {
    Serial.println("Servers ready.");
  }
}



bool _checkhttpserverTimer(){
   if (millis() - httpserverTimer > _WEBSERVER_INTERVAL ){
      httpserverTimer=millis();
      return true;
    } 
    else{
      return false;
    }
}

void _httpServerLoop() {
  // We need to call both loop functions here
  secureServer.loop();
  insecureServer.loop();
  Serial.println("Servers loops called.");
  // Other code would go here...
  delay(1);
}

// The hanlder functions are the same as in the Static-Page example.
// The only difference is the check for isSecure in the root handler

void handleRoot(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html");

   res->println("<!DOCTYPE html>"); // open wrap the web page

   res->print("<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta charset='utf-8'><style>#map {height: 100%;}html, body {height: 100%;margin: 0;padding: 0;}</style></head>");
    if(gs_current_latitude=="" || gs_current_longitude=="")  res->print("<body><h1>'Portuguese Water Dog' Server Maps V1." _SUBVERSION " got no coordinates!</h1>");
   else{
      res->println("_listen: served page for latitude:"+gs_current_latitude+" longitude:"+gs_current_longitude);
      res->print("<body><h1>'Portuguese Water Dog' Server Maps V1."_SUBVERSION "</h1>");
      //Button TEST res->print("<form action = \"/Save position\" method=\"GET\">GPIO 0: <input type=\"submit\" name=\"on\" value=\"ON\"/>&nbsp;<input type=\"submit\" name=\"off\" value=\"OFF\"/>&nbsp;</form>\r\n"

            // You can check if you are connected over a secure connection, eg. if you
      // want to use authentication and redirect the user to a secure connection
      // for that
      if (req->isSecure()) {
        res->println("<p>You are connected via <strong>HTTPS</strong>. Address is: <a href=https://www.google.pt/maps/@"+gs_current_latitude+","+gs_current_longitude+",15z>Link to marker</a></p>");
      } else {
        res->println("<p>You are connected via <strong>HTTP</strong>.</p>");
      }
  
     res->print("<div id=\"map\"></div>");
     //TEST
          res->print(
               "<script>"
              "var myLatLng;"
              "var map;"
              "function CenterControl(controlDiv, map) {"    
                "var controlUI = document.createElement('div');"
                "controlUI.style.backgroundColor = '#fff';"
                "controlUI.style.border = '2px solid #fff';"
                "controlUI.style.borderRadius = '3px';"
                "controlUI.style.boxShadow = '0 2px 6px rgba(0,0,0,.3)';"
                "controlUI.style.cursor = 'pointer';"
                "controlUI.style.marginTop = '10px';"
                "controlUI.style.marginBottom = '22px';"
                "controlUI.style.textAlign = 'center';"
                "controlUI.title = 'Click to save the Sender Path';"
                "controlDiv.appendChild(controlUI);"
                "var controlText = document.createElement('div');"
                "controlText.style.color = 'rgb(25,25,25)';"
                "controlText.style.fontFamily = 'Roboto,Arial,sans-serif';"
                "controlText.style.fontSize = '16px';"
                "controlText.style.lineHeight = '38px';"
                "controlText.style.paddingLeft = '5px';"
                "controlText.style.paddingRight = '5px';"
                "controlText.innerHTML = 'Save Path';"
                "controlUI.appendChild(controlText);"
               " controlUI.addEventListener('click', function() {"
               "    map.setCenter(myLatLng);"
               "    var myJSON = JSON.stringify(myLatLng); "
               "  download(myJSON,'SavePath.txt','text/plain');"
               " });"
              " }"
              "function download(data, filename, type) {"
              "  var file = new Blob([data], {"
              "    type: type"
              "  });"
              "  if (window.navigator.msSaveOrOpenBlob) "// IE10+
              "    window.navigator.msSaveOrOpenBlob(file, filename);"
              "  else { "// Others
              "    var a = document.createElement(\"a\"),"
              "     url = URL.createObjectURL(file);"
              "    a.href = url;"
              "    a.download = filename;"
              "    document.body.appendChild(a);"
              "    a.click();"
              "    setTimeout(function() {"
              "      document.body.removeChild(a);"
              "      window.URL.revokeObjectURL(url);"
              "    }, 0);"
              "  }"
              "}"
            "function initMap(){"
            "myLatLng =  {lat: "+gs_current_latitude+", lng: "+gs_current_longitude+"} ; "
            "var firstLatLng =  {lat: "+gs_rcv_latitude+", lng: "+gs_rcv_longitude+"} ; "
            "var sndmarker = new google.maps.Marker({ position: myLatLng, map: map, title: 'Sender is here!'});"
            "sndmarker.setIcon('http://maps.google.com/mapfiles/ms/icons/red-dot.png');"
            "var rcvmarker = new google.maps.Marker({ position: firstLatLng, map: map, title: 'Receiver is here!'});"
            "rcvmarker.setIcon('http://maps.google.com/mapfiles/ms/icons/blue-dot.png');"
            " map = new google.maps.Map(document.getElementById('map'),{center: myLatLng,zoom: 18,mapTypeId: 'satellite'} ); "
            "var Coordinates = [ "+gspath+"];"
             "var Path = new google.maps.Polyline({path: Coordinates, geodesic: true, strokeColor: '#FF0000', strokeOpacity: 1.0, strokeWeight: 2 });"
              "var centerControlDiv = document.createElement('div');"
              "var centerControl = new CenterControl(centerControlDiv, map);"
              "centerControlDiv.index = 1;"
              "map.controls[google.maps.ControlPosition.TOP_CENTER].push(centerControlDiv);"
             "sndmarker.setMap(map);"
             "rcvmarker.setMap(map);"
             "Path.setMap(map);"
             "}"
             "</script>"
        "<script async defer src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap\"></script>");

     //OLD res->print("<script>function initMap(){var myLatLng =  {lat: "+gs_current_latitude+", lng: "+gs_current_longitude+"} ; var map = new google.maps.Map(document.getElementById('map'),{center: myLatLng,zoom: 18,mapTypeId: 'satellite'} ); var marker = new google.maps.Marker({ position: myLatLng, map: map, title: 'I am here!'}); }</script><script async defer src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyD97IK6vHrYexmh1bbeDI5bQ_EyXzR8dL0&callback=initMap\"></script>");
    }            
    res->print("</body></html>"); // close wrap the web page

 
  
/*  
  res->println("<html>");
  res->println("<head><title>Hello World!</title></head>");
  res->println("<body>");
  res->println("<h1>Hello World!</h1>");

  res->print("<p>Your server is running for ");
  res->print((int)(millis()/1000), DEC);
  res->println(" seconds.</p>");



  res->println("</body>");
  res->println("</html>");
  */
}

void handle404(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(404);
  res->setStatusText("Not Found");
  res->setHeader("Content-Type", "text/html");
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Not Found</title></head>");
  res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
  res->println("</html>");
}
