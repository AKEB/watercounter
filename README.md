# README #

![IMG_0254.jpg](https://bitbucket.org/repo/qEzRgg4/images/1915490043-IMG_0254.jpg)

![WaterCounter_схема.png](https://bitbucket.org/repo/qEzRgg4/images/1898141265-WaterCounter_%D1%81%D1%85%D0%B5%D0%BC%D0%B0.png)

![WaterCounter_bb.png](https://bitbucket.org/repo/qEzRgg4/images/601045342-WaterCounter_bb.png)


![WaterCounter_печатная плата.png](https://bitbucket.org/repo/qEzRgg4/images/3705732514-WaterCounter_%D0%BF%D0%B5%D1%87%D0%B0%D1%82%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%BB%D0%B0%D1%82%D0%B0.png)



## Ссылки ##
* [wifi-iot.com](https://wifi-iot.com/)
* [esptool-gui](https://github.com/Rodmg/esptool-gui)
* [esptool](https://github.com/espressif/esptool)


## Код для заливки прошивки ##
```
#!bash

./esptool.py --port /dev/cu.wchusbserial1420 write_flash -fm=dio -fs=4MB 0x00000 ~/Downloads/0x00000_ESP8266_201705062105.bin

```