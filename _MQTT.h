#include <PubSubClient.h>

const size_t cSize = 20;
char cStr [cSize];
//Server MQTT 
//#define MQTT_SERVER "quickstart.messaging.internetofthings.ibmcloud.com"
#define MQTT_SERVER "broker.hivemq.com"

//Name of the topic to send data
//#define TOPIC_NAME "iot-2/evt/status/fmt/json"
#define TOPIC_NAME "owntracks/PWD/M1"

//ID we shall use to connect
//QUICK_START -do not change
//const String QUICK_START = "d:quickstart:arduino:";
const String QUICK_START = "";
//In DEVICE_ID use a unique ID e.g. MAC Address
//https://quickstart.internetofthings.ibmcloud.com
//const String DEVICE_ID = "cc50e398fb30";
const String DEVICE_ID = "M1";

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
  //if(client.connect("PWD", "PWD", "pwdteste" )){
    //if success connecting
    Serial.println("MQTT connected");
    OLED_write("Connected to MQTT server");
  } else {
    Serial.print("error = ");
    Serial.println(client.state());
    OLED_write("NOT Connected to MQTT server");
  }
  
}


String createJsonString() {
  String battery = "10";
  String accuracy = "10";
  String pressure = "100";
  String vertical_accuracy = "100";
  String location_fix_time = "100";
  String altitude_gps_value = "100";

  
  String json = "{";
  json+= "\"batt\":";
  json+=String(battery);
  json+=",";
  json+= "\"lon\":";
  json+=getCoordString(gdata.longitude);
  json+=",";
  json+= "\"acc\":";
  json+=String(accuracy);
  json+=",";
  json+= "\"p\":";
  json+=String(pressure);
  json+=",";
  json+= "\"vac\":";
  json+=String(vertical_accuracy);
  json+=",";
  json+= "\"lat\":";
  json+=getCoordString(gdata.latitude);
  json+=",";
  json+= "\"t\": \"t\"";
  json+=",";
  json+= "\"conn\": \"w\"";
  json+=",";
  json+= "\"tst\":";
  json+=String(location_fix_time);
  json+=",";
  json+= "\"alt\":";
  json+=String(altitude_gps_value);
  json+=",";
  json+= "\"lon\":";
  json+=getCoordString(gdata.latitude);
  json+=",";
  json+= "\"_type\":\"location\"";
  json+=",";
  json+= "\"tid\": \"";
  json+= DEVICE_ID;
  json+="\"";
  json+="}";
  return json;
/* template  
{"batt":64
,"lon":-9.093604
,"acc":10
,"p":101.15644836425781
,"vac":10
,"lat":38.763523
,"t":"t"
,"conn":"w"
,"tst":1564047365
,"alt":10
,"_type":"location"
,"tid":"P9"}
*/


}


/*
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
*/



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
