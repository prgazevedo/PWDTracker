#ifndef _CONST_H
#define _CONST_H

// _MainVar
// Possible roles SENDER or RECEIVER
#define _ROLE  1 //SENDER=0 RECEIVER=1

//WIFI Vars
//network SSID
#define SSID_NAME "WLAN2"


//LORA Vars
const int _SPREADING = 9; //spreading factor
const String _SUBVERSION = "5";
//Interval in ms between sending data to other Lora
#define INTERVAL 500
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
String payload_data = "Hello";
int payload_size = 0;
byte destinationAddress = 0xFF;      // destination to send to 11111111
byte localAddress = 0x01;     // address of this device 00000001
byte packetID = 0;            // count of outgoing messages
long time_stamp = 0;        // last send time

//OLED 
//parameters: address,SDA,SCL 
SSD1306 display(0x3c, 4, 15); //display object 

//LORA data struct to send
typedef struct {
 float latitude;
 float longitude;
}gpsData;
gpsData gdata;



#endif
