# README #

![IMG_0040.jpg](https://bitbucket.org/repo/gkkjM4R/images/3806246916-IMG_0040.jpg)

## Ссылки ##
* [wifi-iot.com](https://wifi-iot.com/)
* [esptool-gui](https://github.com/Rodmg/esptool-gui)
* [esptool](https://github.com/espressif/esptool)


## Код для заливки прошивки ##
```
#!bash

./esptool.py --port /dev/cu.wchusbserial1420 write_flash -fm=dio -fs=4MB 0x00000 ~/Downloads/0x00000_ESP8266_201705062105.bin

```