# README #

![IMG_0148.JPG.jpeg](https://bitbucket.org/repo/qEzRgg4/images/3018069817-IMG_0148.JPG.jpeg)

## Ссылки ##
* [wifi-iot.com](https://wifi-iot.com/)
* [esptool-gui](https://github.com/Rodmg/esptool-gui)
* [esptool](https://github.com/espressif/esptool)


## Код для заливки прошивки ##
```
#!bash

./esptool.py --port /dev/cu.wchusbserial1420 write_flash -fm=dio -fs=4MB 0x00000 ~/Downloads/0x00000_ESP8266_201705062105.bin

```