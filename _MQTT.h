#include <MQTT.h>

//Server MQTT 
#define MQTT_SERVER "broker.hivemq.com"
//Name of the topic to send data
#define TOPIC_NAME "owntracks/PWD/M1"

const String QUICK_START = "";
//In DEVICE_ID use a unique ID e.g. MAC Address
const String DEVICE_ID = "cc50e398fb30";
//MQTT ClientId 
const String CLIENT_ID =  QUICK_START + DEVICE_ID;

//Wifi Client
WiFiClient wifiClient;
MQTTClient pubClient(256);

String lwMQTTErr(lwmqtt_err_t reason);
String lwMQTTErrConnection(lwmqtt_return_code_t reason);
void printErrorCodes();

//Connect to server MQTT
void _connectMQTTServer() {
  Serial.println("connectMQTTServer:Connecting to MQTT Server...");
  Serial.println("Configured MAC for MQTT is: "+DEVICE_ID);
  OLED_write("Connecting to MQTT Server..."); 
  //Client MQTT,server URL and port + Wifi
  pubClient.begin(MQTT_SERVER, 1883,wifiClient);
  //Connect to the Id we defined
    
  if (pubClient.connect(CLIENT_ID.c_str())) {
    //if success connecting
    Serial.println("MQTT connected");
    OLED_write("Connected to MQTT server");
  } else {
    Serial.println("NOT Connected to MQTT server");
    OLED_write("NOT Connected to MQTT server");
    printErrorCodes();
  }

  
    if (pubClient.publish(TOPIC_NAME, "hello from PWD Receiver M1")) {
      Serial.println("Initial Publish ok at: " MQTT_SERVER " under topic: " TOPIC_NAME);
    }
    else {
      Serial.println("Initial Publish failed");
    }
  
}


String createJsonString() {
  String battery = "--";
  String accuracy = "10";
  String pressure = "--";
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
  json+= "\"t\":\"t\"";
  json+=",";
  json+= "\"conn\":\"w\"";
  json+=",";
  json+= "\"tst\":";
  json+=String(location_fix_time);
  json+=",";
  json+= "\"alt\":";
  json+=String(altitude_gps_value);
  json+=",";
  json+= "\"_type\":\"location\"";
  json+=",";
  json+= "\"tid\":\"";
  json+= DEVICE_ID;
  json+="\"";
  json+="}";
  return json;


}


void printErrorCodes(){
    Serial.print("error = ");
    Serial.println(lwMQTTErr(pubClient.lastError()));
    Serial.print("returnCode = ");
    Serial.println(lwMQTTErrConnection(pubClient.returnCode()));
}


void publishData(String message){
     
      //Publish in the topic 
        if (pubClient.publish(TOPIC_NAME, message)) {
          Serial.println("Publish ok at: " MQTT_SERVER " under topic: " TOPIC_NAME);
        }
        else {
          Serial.println("Publish failed");
          printErrorCodes();
        }
    
      OLED_PUB_DATA();
}



void _publishLocationData(){
      //create the json to send to the mqtt server
      String msg = createJsonString();
      Serial.print(" _publishLocationDataPublish message: "+msg);
      publishData(msg);
}

String lwMQTTErr(lwmqtt_err_t reason)
{
  if (reason == lwmqtt_err_t::LWMQTT_SUCCESS)
    return("Success");
  else if (reason == lwmqtt_err_t::LWMQTT_BUFFER_TOO_SHORT)
    return("Buffer too short");
  else if (reason == lwmqtt_err_t::LWMQTT_VARNUM_OVERFLOW)
    return("Varnum overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_CONNECT)
    return("Network failed connect");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_TIMEOUT)
    return("Network timeout");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_READ)
    return("Network failed read");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_WRITE)
    return("Network failed write");
  else if (reason == lwmqtt_err_t::LWMQTT_REMAINING_LENGTH_OVERFLOW)
    return("Remaining length overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_REMAINING_LENGTH_MISMATCH)
    return("Remaining length mismatch");
  else if (reason == lwmqtt_err_t::LWMQTT_MISSING_OR_WRONG_PACKET)
    return("Missing or wrong packet");
  else if (reason == lwmqtt_err_t::LWMQTT_CONNECTION_DENIED)
    return("Connection denied");
  else if (reason == lwmqtt_err_t::LWMQTT_FAILED_SUBSCRIPTION)
    return("Failed subscription");
  else if (reason == lwmqtt_err_t::LWMQTT_SUBACK_ARRAY_OVERFLOW)
    return("Suback array overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_PONG_TIMEOUT)
    return("Pong timeout");
}

String lwMQTTErrConnection(lwmqtt_return_code_t reason)
{
  if (reason == lwmqtt_return_code_t::LWMQTT_CONNECTION_ACCEPTED)
    return("Connection Accepted");
  else if (reason == lwmqtt_return_code_t::LWMQTT_UNACCEPTABLE_PROTOCOL)
    return("Unacceptable Protocol");
  else if (reason == lwmqtt_return_code_t::LWMQTT_IDENTIFIER_REJECTED)
    return("Identifier Rejected");
  else if (reason == lwmqtt_return_code_t::LWMQTT_SERVER_UNAVAILABLE)
    return("Server Unavailable");
  else if (reason == lwmqtt_return_code_t::LWMQTT_BAD_USERNAME_OR_PASSWORD)
    return("Bad UserName/Password");
  else if (reason == lwmqtt_return_code_t::LWMQTT_NOT_AUTHORIZED)
    return("Not Authorized");
  else if (reason == lwmqtt_return_code_t::LWMQTT_UNKNOWN_RETURN_CODE)
    return("Unknown Return Code");
}
