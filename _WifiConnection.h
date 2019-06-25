

#include <WiFi.h>
#include "_File.h"
#include "_const.h" 
#include "_WebServer.h"





// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";



void _setupAP(){
    // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  Serial.println("\n#################");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}


String wl_status_to_string(int status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}


String _checkWifiState(){
   //Serial.println("Wifi connection to: "+String(WiFi.SSID()));
  //Serial.println("Wifi connection status:"+wl_status_to_string( WiFi.status()));
   Serial.print("Wifi local IP: ");
   //Serial.println(WiFi.localIP());
}

//Connect to WiFi
boolean connectWifi(){

  int status_timer=0;
  int wait_counter=0;
  int wifi_timeout = 0;
  //Wait until established
  Serial.println(" WiFi connecting!");
  Serial.println("#"+String(SSID_NAME)+"#");
  while (WiFi.status() != WL_CONNECTED) {

    delay(WIFI_WAIT);
    status_timer+=WIFI_WAIT;
    Serial.print(".");
    wait_counter++;
    if(status_timer>WIFI_STATUS_TIMER)
    {
       Serial.printf("Wifi connection status: %d\n", WiFi.status());
       status_timer=0;
    }
    if(wait_counter>WIFI_RETRIES)
    {
      //restart wait
      Serial.println("Wifi connection Retries exceeded:"+String(WIFI_RETRIES)+"  - ReConnection in progress");
      wait_counter=0;
      wifi_timeout++;
      WiFi.reconnect();
    }
    if(wifi_timeout>WIFI_TIMEOUT){
      Serial.println("Wifi connection Timeout:"+String(WIFI_TIMEOUT));
      return false;
    }
  }
  return true;
    
}




//Setup WiFi
boolean setupWiFi() {

  Serial.println("Wifi Mode is: "+String(WIFI_MODE));
   WiFi.mode(WIFI_MODE);
  Serial.println("MAC of device is: "+WiFi.macAddress());
  Serial.println("Connecting to WIFI: "+String(SSID_NAME)); 
   Serial.print("Connecting to WIFI using password:");
   String password = _readPassword();
   OLED_write("Connecting to WIFI: "+String(SSID_NAME)); 
   const char* cpassword = password.c_str();
  //Connect using SSID and PASSWORD
  if(password!=""){
    Serial.println(" WiFi.begin called");
    WiFi.begin(SSID_NAME, cpassword);
    WiFi.setAutoReconnect(true);
    WiFi.setHostname("PWDServer");
  }
  else
  {
    Serial.println("No password found - cannot connect to wifi");
    return false;
  }

    
  if(connectWifi())
  {
    //If here it has Connected
    //Display on Serial Monitor
    Serial.println("\n#################");
    Serial.println("WiFi connected to"+String( SSID_NAME));
    Serial.print("IP address : ");
    IPAddress Wifi_IP = WiFi.localIP();
    Serial.println(Wifi_IP);
    Serial.println("\n#################");
    OLED_write("Connected to "+String( SSID_NAME));
    OLED_write("IP: "+String(Wifi_IP));
    return true;
  }
  else
  {
   Serial.println("connectWifi failed!");
  }
}




void _setupWifiConnection(){
  Serial.println("_setupWifiConnection called");
  WIFI_CONNECTION = setupWiFi();
  if(!WIFI_CONNECTION){
    Serial.println("setupWiFi failed - calling _setupAPWebServer");
    _setupAPWebServer();
  }

}
