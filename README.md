# PWDTracker
A prototype for geo location using Lora

## Current Build options

Requires following libraries:
 - PubsubClient for MQTT: https://github.com/knolleary/pubsubclient/releases/tag/v2.7
 - ArduinoJson for MQTT: https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
 - Autoconnect for Wifi: https://hieromon.github.io/AutoConnect/index.html
 - TinyGpsPlus for GPS: https://github.com/mikalhart/TinyGPS
 - SSD1306 for OLED: https://github.com/ThingPulse/esp8266-oled-ssd1306
 - Lora radio by Sandeep: https://github.com/sandeepmistry/arduino-LoRa
 - MQTT by Joël Gähwiler: https://github.com/256dpi/arduino-mqtt
Board library:
 - Heltec: https://github.com/HelTecAutomation/Heltec_ESP32

In _Definitions.h change _ROLE to either 0 or 1 //SENDER=0 RECEIVER=1

## Current Hardware
- Upload this code via Serial to Heltec Wifi Lora: https://heltec.org/project/wifi-lora-32/
- Use a GPS module to connect (Rx<-->TX) to pins 22/23 on Heltec. Example module: https://www.aliexpress.com/item/32828934753.html
- Use ipex connector for Lora devices(DO NOT POWER UP WITHOUT ANTENNA CONNECTED)
- Use a good GPS antenna for precision outdoors


## Current Usage
 - Power up both devices (power to all via USB)
 - Check that both devices boot up and present correct GPS data in OLED display
 - Connect to receiver IP via http to view the location of sender device
 - View latitude and longitude of sender in the cloud: mqtt://broker.hivemq.com:1883 Topice:owntracks/PWD/#

## TODO
 - Add webserver to configure options
 - Add discovery of wireless LAN tracking
 - Remove all String usage (avoid) and use CString or Char*
 - Optimize Sender code to reduce power consumption
 - Setup and test autoconnect functionality in Receiver
