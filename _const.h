#ifndef _CONST_H
#define _CONST_H

// _MainVar
// Possible roles SENDER or RECEIVER
#define _ROLE  1 //SENDER=0 RECEIVER=1
#define _SUBVERSION "14"

//LORA DEFINITIONS
// LORA Pin definition
#define _SCK     5    // GPIO5  -- SX127x's SCK
#define _MISO    19   // GPIO19 -- SX127x's MISO
#define _MOSI    27   // GPIO27 -- SX127x's MOSI
#define _SS      18   // GPIO18 -- SX127x's CS
#define _RST     14   // GPIO14 -- SX127x's RESET
#define _DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)
//Interval in ms between sending data to other Lora
#define _LORA_SEND_INTERVAL 1000
// LORA Band definition
#define _BAND    868E6  //Radio Frequency
#define _PABOOST true
#define _LONG_RANGE false
///LONG RANGE SETUP
#define _SIGNAL_BANDWIDTH 10.4E3
#define _SPREADING_FACTOR 7 
#define _CODING_RATE 8
#define _TX_POWER 18

//GPS DEFINITIONS
static const int RXPin = 22, TXPin = 23;
static const uint32_t GPSBaud = 9600;


//Time of last Lora data packet
long lastSendTime = 0;



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
//WIFI DEFINITIONS
//network SSID
#define _WIFI_MODE WIFI_AP_STA   //Alternatives 
#define _SSID_NAME "WLAN2"
#define _SSID_PASSWORD "pedro2chave2"
#define _WIFI_WAIT 500
#define _WIFI_STATUS_TIMER 5000
#define _WIFI_RETRIES  15
#define _WIFI_TIMEOUT 3
#define _WEBSERVER_INTERVAL 1500
//WIFI
boolean WIFI_CONNECTION = false;
String WIFI_IP_STR = "Not Connected";


//OLED DEFINITIONS
//parameters: address,SDA,SCL 
SSD1306 display(0x3c, 4, 15); //display object 
#define FRAME_DELAY 500 
#define LOGO_DELAY 5000 



String g_latitude ="";
String g_longitude ="";

#endif
