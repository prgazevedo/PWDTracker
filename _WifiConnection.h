


#include <WiFi.h>
#include "_File.h"



// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";


//test Wifi MAC
void testMAC(){

  Serial.println("Wifi of device is: "+WiFi.macAddress());

}

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

//Connect to WiFi
void setupWiFi() {
  Serial.println("setupWiFi called");
  Serial.println("Wifi Mode is: "+String(WIFI_AP_STA));
   WiFi.mode(WIFI_MODE);
  testMAC();
  Serial.println("Connecting to WIFI: "+String(SSID_NAME)); 
  OLED_write("Connecting to WIFI: "+String(SSID_NAME)); 
  //Connect using SSID and PASSWORD
     Serial.print("password:");
   String password = _readPassword();
   const char* cpassword = password.c_str();

  if(password!=""){
 
    WiFi.begin(SSID_NAME, cpassword);
    WiFi.setAutoReconnect(true);
    WiFi.setHostname("PWDServer");
  }
  else
  {
    Serial.println("No password found - cannot connect to wifi");
    return;
  }
  int timer=0;
  int retries=0;
  //Wait until established
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_TIMER);
    timer+=WIFI_TIMER;
    Serial.print(".");
    if(timer>10*WIFI_TIMER)
    {
       Serial.printf("Connection status: %d\n", WiFi.status());
       timer=0;
       retries++;
    }
    if(retries>WIFI_RETRIES)
    {
      //restart connection retry
      retries=0;
      WiFi.reconnect();
    }
  }

  

  //If here it has Connected
  //Display on Serial Monitor
  Serial.println("\n#################");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi connected to"+String( SSID_NAME));
  OLED_write("Connected to "+String( SSID_NAME));
}


void _setupWifiConnection(){
  Serial.println("setupMQTT called");
  setupWiFi();

}
