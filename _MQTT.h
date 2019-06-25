
#include <PubSubClient.h>

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
void _connectMQTTServer() {
  Serial.println("connectMQTTServer:Connecting to MQTT Server...");
  Serial.println("Configured MAC for MQTT is: "+DEVICE_ID);
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
      json+=getCoordString(gdata.latitude);
      json+=",";
      json+="\"Longitude\":";
      json+=getCoordString(gdata.longitude);
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
