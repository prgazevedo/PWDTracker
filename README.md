# PWDTracker

## Build options

Requires following libraries:
 - PubsubClient for MQTT: https://github.com/knolleary/pubsubclient/releases/tag/v2.7
 - ArduinoJson for MQTT: https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
 - Autoconnect for Wifi: https://hieromon.github.io/AutoConnect/index.html
 - TinyGpsPlus for GPS: https://github.com/mikalhart/TinyGPS
 - SSD1306 for OLED: https://github.com/ThingPulse/esp8266-oled-ssd1306
 - Lora radio by Sandeep: https://github.com/sandeepmistry/arduino-LoRa

In Const.h change _ROLE to either 0 or 1 //SENDER=0 RECEIVER=1

## Hardware
- Upload this code via Serial to Heltec Wifi Lora: https://heltec.org/project/wifi-lora-32/
- Use a GPS module to connect (Rx<-->TX) to pins 22/23 on Heltec. Example module: https://www.aliexpress.com/item/32828934753.html
- Use ipex connector for Lora devices(DO NOT POWER UP WITHOUT ANTENNA CONNECTED)
- Use a good GPS antenna for precision outdoors


