# EspOledTemp
The base for this build is a NodeMCU, a Soc based on ESP8266. It has a sensor for temperature and humidity connected, a HTU21D module. It also have an OLED connected for showing measured values, these values are also sent via Mqtt to a Mqtt-broker.
The values are also presented via a dynamic webpage where you can see the values in near realtime.

As for connections, both the Oled and the sensor uses I2C and connects to the NodeMCU's D1 and D2. See [this page](http://www.esp8266learning.com/esp8266-sht21-humidity-temperature-sensor-example.php) for an example. 

To get going, open include/settings.example, rename it to include/settings.h and adjust the values to match your setup. Then upload the code to your NodeMcu/Wemos/OtherEspDevice. 

![Screenshot](https://github.com/bphermansson/EspOledTemp/blob/dev/screenshot.png)

![Device](https://github.com/bphermansson/EspOledTemp/blob/dev/device.jpg)

More information: http://paheco.nu/espoledtemp/
