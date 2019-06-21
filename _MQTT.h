

#include <PubSubClient.h>
#include <WiFi.h>


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

//Concatemos o id do quickstart com o id do nosso
//dispositivo
const String CLIENT_ID =  QUICK_START + DEVICE_ID;

//Cliente WiFi que o MQTT irá utilizar para se conectar
WiFiClient wifiClient;

//Cliente MQTT, passamos a url do server, a porta
//e o cliente WiFi
PubSubClient client(MQTT_SERVER, 1883, wifiClient);

//Intervalo entre os envios
#define INTERVAL 500

//Tempo do último envio
long lastSendTime = 0;


void _setupMQTT(){
  Serial.println("setupMQTT called");
  setupWiFi();
  connectMQTTServer();
}

//Connect to server MQTT
void connectMQTTServer() {
  Serial.println("connectMQTTServer:Connecting to MQTT Server...");
  //Se conecta ao id que definimos
  if (client.connect(CLIENT_ID.c_str())) {
    //if success connecting
    Serial.println("connected");
  } else {
    Serial.print("error = ");
    Serial.println(client.state());
  }
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
  Serial.print("Connecting to ");
  Serial.print(SSID);

  testMAC();
 

  //Connect using SSID and PASSWORD
  WiFi.begin(SSID, PASSWORD);

  //Wait until established
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If here it has Connected
  Serial.println("");
  Serial.println("WiFi connected to"+ SSID);
}


void publishData(){
       Serial.print("Publish message: ");
      //create the json to send to the mqtt server
      String msg = createJsonString();
      Serial.println(msg);
      //Publish in the topic 
      //to generate the graph
      client.publish(TOPIC_NAME, msg.c_str());
}

String createJsonString() {
  String json = "{";
    json+= "\"d\": {";
      json+="\"ID\":";
      json+=String(pdata.ID);
      json+=",";
      json+="\"Time\":";
      json+=String(pdata.timeMillis);
    json+="}";
  json+="}";
  return json;
}

#endif
