

#include <WiFi.h>

#include "_File.h"
#include "_Definitions.h" 




void _setupMAC(){
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  s_chipid = String((uint16_t)(chipid>>32),HEX)+String((uint32_t)chipid,HEX);
  Serial.println("ESP32 Chip ID String:"+s_chipid);
}


void updateWifiIP(){
  IPAddress Wifi_IP = WiFi.localIP();
    global_wifi_IP = Wifi_IP.toString();
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
   Serial.println("Wifi connection to: "+String(WiFi.SSID()));
    Serial.println("Wifi connection status:"+wl_status_to_string( WiFi.status()));
   updateWifiIP();
   Serial.print("Wifi local IP: ");
   Serial.println(WiFi.localIP());
   
}

//Connect to WiFi
boolean connectWifi(){

  int status_timer=0;
  int wait_counter=0;
  int wifi_timeout = 0;
  //Wait until established
  Serial.println(" WiFi connecting!");
  Serial.println("#"+String(_SSID_NAME)+"#");
  while (WiFi.status() != WL_CONNECTED) {

    delay(_WIFI_WAIT);
    status_timer+=_WIFI_WAIT;
    Serial.print(".");
    wait_counter++;
    if(status_timer>_WIFI_STATUS_TIMER)
    {
       Serial.printf("Wifi connection status: %d\n", WiFi.status());
       status_timer=0;
    }
    if(wait_counter>_WIFI_RETRIES)
    {
      //restart wait
      Serial.println("Wifi connection Retries exceeded:"+String(_WIFI_RETRIES)+"  - ReConnection in progress");
      wait_counter=0;
      wifi_timeout++;
      WiFi.reconnect();
    }
    if(wifi_timeout>_WIFI_TIMEOUT){
      Serial.println("Wifi connection Timeout:"+String(_WIFI_TIMEOUT));
      return false;
    }
  }
  return true;
    
}





//Setup WiFi
boolean setupWiFi() {

  Serial.println("Wifi Mode is: "+String(_WIFI_MODE));
   WiFi.mode(_WIFI_MODE);
  Serial.println("MAC of device is: "+WiFi.macAddress());
  Serial.println("Connecting to WIFI: "+String(_SSID_NAME)); 
   Serial.print("Connecting to WIFI using password:");
   //String password = _readPassword();
   String password = _SSID_PASSWORD;
   OLED_write("Connecting to WIFI: "+String(_SSID_NAME)); 
   const char* cpassword = password.c_str();
  //Connect using SSID and PASSWORD
  if(password!=""){
    Serial.println(" WiFi.begin called");
    WiFi.begin(_SSID_NAME, cpassword);
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
    Serial.println("WiFi connected to"+String( _SSID_NAME));
    Serial.print("IP address : ");

    
    updateWifiIP();

    
    Serial.println(global_wifi_IP);
    Serial.println("\n#################");
    OLED_write("OK! IP:"+global_wifi_IP);
    return true;
  }
  else
  {
   Serial.println("connectWifi failed!");
  }
}


void _setupWifiConnection(){
  Serial.println("_setupWifiConnection called");
  global_wifi_conn_state = setupWiFi();
  if(!global_wifi_conn_state){
    Serial.println("setupWiFi failed ");

  }

}
