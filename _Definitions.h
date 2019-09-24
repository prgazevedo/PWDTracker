#ifndef _CONST_H
#define _CONST_H

// _Main DEFINITIONS
// Possible roles SENDER or RECEIVER
#define _ROLE  1 //SENDER=0 RECEIVER=1
#define _SUBVERSION "20"

//POWER DEFINITIONS
#define BATTERY_PIN 13
#define _FULL_BATTERY    3700  //The default battery is 3700mv when the battery is fully charged.
#define _POWER_READ_INTERVAL 20000
#define CPU_FREQ 80 //160
#define CONFIG_ESP_CONSOLE_UART_NUM 1
//LOW POWER DEFINITIONS
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  2       /* Time ESP32 will go to sleep (in seconds) */
//LORA DEFINITIONS
// LORA Pin definition
#define _SCK     5    // GPIO5  -- SX127x's SCK
#define _MISO    19   // GPIO19 -- SX127x's MISO
#define _MOSI    27   // GPIO27 -- SX127x's MOSI
#define _SS      18   // GPIO18 -- SX127x's CS
#define _RST     14   // GPIO14 -- SX127x's RESET
#define _DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)
//Interval in ms between sending data to other Lora
#define _LORA_SEND_INTERVAL 10000
// LORA Band definition
#define _BAND    868E6  //Radio Frequency
#define _PABOOST true
#define _LONG_RANGE true
#define _MAXPOWER true
///LONG RANGE SETUP
#define _SIGNAL_BANDWIDTH 62.5E3
#define _SPREADING_FACTOR 12 
#define _CODING_RATE 5
#define _MAX_TX_POWER 20

//GPS DEFINITIONS
#define RXPin 22
#define TXPin 23
#define GPSBaud 9600
#define GPS_H_PRECISION 2.5 // from: https://www.u-blox.com/sites/default/files/products/documents/NEO-7_DataSheet_%28UBX-13003830%29.pdf
#define GPS_V_PRECISION 5 //double the horizontal - https://portal.u-blox.com/s/question/0D52p00008HKCDmCAP/does-ublox-standalone-positioning-provide-vertical-positioning-how-precise-is-it
//WIFI DEFINITIONS
//network SSID
#define _WIFI_MODE WIFI_AP_STA   //Alternatives 
#define _SSID_NAME "PWD"
#define _SSID_PASSWORD "pwdteste"
#define _SSID_CONNECT_TIMER 30000
#define _PORTAL_TIMER 180000
//EEPROM Settings
#define CREDENTIAL_OFFSET 0

//WEBSERVER
#define HTTP_PORT 80
#define _WEBSERVER_INTERVAL 1500

//OLED LOGO DEFINITIONS
#define FRAME_DELAY 500 
#define LOGO_DELAY 5000 

//MQTT DEFINITIONS
//Server MQTT 
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_SERVER_PORT 1883
#define MQTT_MESSAGE_SIZE 256
//Name of the topic to send data
#define TOPIC_NAME_HEADER "owntracks/PWD/TEST"

#endif
