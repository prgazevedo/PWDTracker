# PWDTracker
A prototype for geo location using Lora

## Build - Current Build options

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

## Upload Current Bin files
 - See binary files to upload to modules in [PWDTracker/bin](https://github.com/prgazevedo/PWDTracker/tree/master/bin)
 - Note: to upload: 1- turn on verbose output (in Preferences). 2- Make a dummy arduino file and during upload and watch for a command in the end with AVRDUDE in it along with the path of hex/bin file. 3- just copy paste & run the same command in CommandLine replacing the bin file with the one downloaded from here.

## Current Used Hardware
- Upload this code via Serial to Heltec Wifi Lora: https://heltec.org/project/wifi-lora-32/ can buy it here: https://www.amazon.es/gp/product/B076T28KWG
- Use a GPS module to connect (Rx<-->TX) to pins 22/23 on Heltec: Rx-Heltec_Pin22 to Tx pin on GPS and Rx-Heltec_Pin23 to  Rx pin on GPS . ![GPS connectors] (https://github.com/prgazevedo/PWDTracker/blob/master/images/GT-U7_connections_back.png)
Buy similar GPS modules at: https://www.aliexpress.com/item/32828934753.html or https://www.amazon.es/gp/product/B0783H7BLW
An example of soldering connections for GPS to Heltec module is
![Heltec sender to GPS module soldering connections](https://github.com/prgazevedo/PWDTracker/blob/master/images/IMG_3561.JPG)
- Use ipex connector for Lora devices(DO NOT POWER UP WITHOUT ANTENNA CONNECTED)
- Use a good GPS antenna for precision outdoors: https://www.amazon.es/gp/product/B01BML4XMQ
- Use a Ipex to SMA converter: https://www.amazon.es/gp/product/B07G5H6WH3
- Use the following batteries to Sender (Li-Po 3.7V 850mAh 6x30x48mm):https://mauser.pt/catalog/product_info.php?products_id=035-9016 and Receiver (Li-Po 3.7V 450mAh 5x22x48mm): https://mauser.pt/catalog/product_info.php?products_id=035-9008
- Connect the batteries and the Switch to Heltec Raster PicoBlade connector: https://mauser.pt/catalog/product_info.php?products_id=035-0050
- Solder the + contact of the battery and of Heltec connector to the switch: https://mauser.pt/catalog/product_info.php?products_id=010-1218
- 3d print the enclosures using the files at (https://github.com/prgazevedo/PWDTracker/tree/master/enclosures). An example of the printed enclosures is ![printed enclosures](https://github.com/prgazevedo/PWDTracker/blob/master/images/3D%20printed%20enclosures.JPG)
An example of the full setup is
![Heltec sender to GPS module soldering connections](https://github.com/prgazevedo/PWDTracker/blob/master/images/IMG_3562.JPG)


## Current Usage
 - Power up both devices (power to all via USB)
 - Check that both devices boot up and present correct GPS data in OLED display
 - Connect to receiver IP via http to view the location of sender device
 - View latitude and longitude of sender in the cloud: mqtt://broker.hivemq.com:1883 Topic:owntracks/PWD/#
 - Download the App at AppStore(https://apps.apple.com/us/app/owntracks/id692424691) or GooglePlay (https://play.google.com/store/apps/details?id=org.owntracks.android&hl=en)

## TODO
 - Add webserver to configure options
 - Add discovery of wireless LAN tracking
 - Remove all String usage (avoid) and use CString or Char*
 - Optimize Sender code to reduce power consumption
 - Setup and test autoconnect functionality in Receiver
