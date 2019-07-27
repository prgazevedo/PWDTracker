#ifndef _CONST_H
#define _CONST_H

// _Main DEFINITIONS
// Possible roles SENDER or RECEIVER
#define _ROLE  0 //SENDER=0 RECEIVER=1
#define _SUBVERSION "16"

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
#define _WIFI_WAIT 500
#define _WIFI_STATUS_TIMER 5000
#define _WIFI_RETRIES  15
#define _WIFI_TIMEOUT 3
#define _WEBSERVER_INTERVAL 1500

//OLED LOGO DEFINITIONS
#define FRAME_DELAY 500 
#define LOGO_DELAY 5000 

#endif
