
#include <WiFi.h>
//#include <WebServer.h>
#include <AutoConnect.h>


//WebServer Server;
AutoConnect       Portal;//(Server);
AutoConnectConfig  Config;
bool bPortalStarted;

void _setupMAC(){
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  s_chipid = String((uint16_t)(chipid>>32),HEX)+String((uint32_t)chipid,HEX);
  Serial.println("ESP32 Chip ID String:"+s_chipid);
}

bool portalStarted(IPAddress ip) {
  bPortalStarted = true;
  Serial.println("Portal started at IP:" + WiFi.localIP().toString());
  return true;
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


void _updateWifiState(){
     Serial.println("_updateWifiState Wifi local IP: "+WiFi.localIP().toString());
   IPAddress Wifi_IP = WiFi.localIP();
    global_wifi_IP = Wifi_IP.toString();
}


String _checkWifiState(){
   Serial.println("_checkWifiState Wifi connection to: "+String(WiFi.SSID()));
    Serial.println("_checkWifiState Wifi connection status:"+wl_status_to_string( WiFi.status()));
}

void _setupAutoconnect() {
   bPortalStarted=false;
  Serial.println("_setupAutoconnect called");

  // Enable saved past credential by autoReconnect option,
  // even once it is disconnected.
  Config.autoReconnect = true;
  Config.apid = s_chipid;
  Config.portalTimeout = _PORTAL_TIMER;
  Portal.config(Config);
  Portal.onDetect(portalStarted);
   // Establish a connection with an autoReconnect option.
   Serial.println("Establish a connection with an autoReconnect option - Portal.begin to SSID:" _SSID_NAME " with password:" _SSID_PASSWORD " with timer: "+String(_SSID_CONNECT_TIMER)+ "(ms)");
  if (Portal.begin(_SSID_NAME,_SSID_PASSWORD,_SSID_CONNECT_TIMER)) {
    Serial.println("Connection established, AutoConnect service started with WIFI_STA mode. WiFi connected at:" + WiFi.localIP().toString());
  }else{
    Serial.println("WiFi NOT connected. Captive portal started with WIFI_AP_STA mode");
    _checkWifiState();
  }
}

void _handlePortal(){
  Portal.handleClient();
  if (WiFi.status() == WL_IDLE_STATUS) {
    Serial.println("WiFi.status is  WL_IDLE_STATUS. ESP.restart called");
    ESP.restart();

  }
}
