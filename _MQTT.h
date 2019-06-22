

#include <PubSubClient.h>
#include <WiFi.h>
#include "_File.h"

const size_t cSize = 20;
char cStr [cSize];
//Server MQTT 
#define MQTT_SERVER "quickstart.messaging.internetofthings.ibmcloud.com"

//Name of the topic to send data
#define TOPIC_NAME "iot-2/evt/status/fmt/json"

//ID we shall use to connect
//QUICK_START -do not change
const String QUICK_START = "d:quickstart:arduino:";

//In DEVICE_ID use a unique ID e.g. MAC Address
//https://quickstart.internetofthings.ibmcloud.com
const String DEVICE_ID = "cc50e398fb30";

//MQTT ClientId 
const String CLIENT_ID =  QUICK_START + DEVICE_ID;

//Wifi Client
WiFiClient wifiClient;

//Client MQTT,server URL and port + Wifi
PubSubClient client(MQTT_SERVER, 1883, wifiClient);


//Connect to server MQTT
void connectMQTTServer() {
  Serial.println("connectMQTTServer:Connecting to MQTT Server...");
  OLED_write("Connecting to MQTT Server..."); 
  //Connect to the Id we defined
  if (client.connect(CLIENT_ID.c_str())) {
    //if success connecting
    Serial.println("connected");
  } else {
    Serial.print("error = ");
    Serial.println(client.state());
  }
  OLED_write("Connected to MQTT server");
}


//test Wifi MAC
void testMAC(){
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("Wifi of device is: "+WiFi.macAddress());
  Serial.println("Configured MAC for MQTT is: "+DEVICE_ID);
}

//Connect to WiFi
void setupWiFi() {
  Serial.println("setupWiFi called");
  testMAC();
  Serial.println("Connecting to WIFI: "+String(SSID_NAME)); 
  OLED_write("Connecting to WIFI: "+String(SSID_NAME)); 
  //Connect using SSID and PASSWORD
     Serial.print("password:");
   String password = _readPassword();
   const char* cpassword = password.c_str();
   /*
    for (int i = 0; i < strlen(cpassword); ++i) {
          Serial.printf("%02x ", cpassword[i]);
        }
        Serial.println("");
  */
  if(password!=""){
 
    WiFi.begin(SSID_NAME, cpassword);
  }
  else Serial.println("No password found - cannot connect to wifi");

  //Wait until established
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If here it has Connected
  Serial.println("");
  Serial.println("WiFi connected to"+String( SSID_NAME));
  OLED_write("Connected to "+String( SSID_NAME));
}


void _setupMQTT(){
  Serial.println("setupMQTT called");
  setupWiFi();
  connectMQTTServer();
}



String createJsonString() {
  String json = "{";
    json+= "\"d\": {";
      json+="\"ID\":";
      json+=String(pdata.ID);
      json+=",";
      json+="\"Time\":";
      json+=String(pdata.timeMillis);
      json+=",";
      json+="\"Latitude\":";
      json+=String(gdata.latitude,8);
      json+=",";
      json+="\"Longitude\":";
      json+=String(gdata.longitude,8);
    json+="}";
  json+="}";
  return json;
}

void _publishData(){
       Serial.print("Publish message: ");
      //create the json to send to the mqtt server
      String msg = createJsonString();
      Serial.println(msg);
      //Publish in the topic 
      //to generate the graph
      client.publish(TOPIC_NAME, msg.c_str());
      OLED_PUB_DATA();
}
