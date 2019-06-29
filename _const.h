#ifndef _CONST_H
#define _CONST_H

// _MainVar
// Possible roles SENDER or RECEIVER
#define _ROLE  1 //SENDER=0 RECEIVER=1

//WIFI Vars
//network SSID
#define WIFI_MODE WIFI_AP_STA   //Alternatives 
#define SSID_NAME "PWD"
#define SSID_PASSWORD "pwdteste"
#define WIFI_WAIT 500
#define WIFI_STATUS_TIMER 5000
#define WIFI_RETRIES  15
#define WIFI_TIMEOUT 3
#define WEBSERVER_INTERVAL 1500
boolean WIFI_CONNECTION = false;
//LORA Vars
const int _SPREADING = 9; //spreading factor
const String _SUBVERSION = "13";
//Interval in ms between sending data to other Lora
#define LORA_SEND_INTERVAL 1000
//Time of last Lora data packet
long lastSendTime = 0;
// LORA Pin definition
#define SCK     5    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)
// LORA Band
#define BAND    868E6  //Radio Frequency
#define PABOOST true

//LORA data struct to send
typedef struct {
  unsigned int ID;
  long timeMillis;
}PacketData;
PacketData pdata;

//LORA packet data
int rssi_value = 0;
unsigned long snr_value = 0;
String packSize = "--";
int payload_pdata_size = 0;
int payload_gdata_size = 0;
byte destinationAddress = 0xFF;      // destination to send to 11111111
byte localAddress = 0x01;     // address of this device 00000001
byte packetID = 0;            // count of outgoing messages
long time_stamp = 0;        // last send time

//OLED 
//parameters: address,SDA,SCL 
SSD1306 display(0x3c, 4, 15); //display object 
#define FRAME_DELAY 500 
#define LOGO_DELAY 5000 
//LORA data struct to send

typedef struct CoordToSend
{
   uint16_t deg;
   uint32_t billionths;
   bool negative;
public:
   CoordToSend() : deg(0), billionths(0), negative(false){}
}coordToSend;


typedef struct GpsData{
 coordToSend latitude,longitude;
}gpsData;

gpsData gdata;
String g_latitude ="";
String g_longitude ="";

#endif
