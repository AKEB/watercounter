# README #

Простая плата на базе микросхемы ESP8266 (Wemos D1 mini). Плата считает кол-во импульсов от счетчиков воды (горячей и холодной) и импульс от системы АкваСторож.
Отображение в web интерфейсе и отправка на сервер умного дома по протоколу MQTT

![1.jpg](https://github.com/AKEB/watercounter/blob/master/1.jpeg?raw=true)

![2.png](https://github.com/AKEB/watercounter/blob/master/2.png?raw=true)

![3.png](https://github.com/AKEB/watercounter/blob/master/3.png?raw=true)

![4.png](https://github.com/AKEB/watercounter/blob/master/4.png?raw=true)


File System install https://github.com/esp8266/arduino-esp8266fs-plugin


# Распиновка #
* Красная лампа - D8 - GPIO15
* Синяя лампа - D7 - GPIO13
* Встроенная лампа - D4 - GPIO2
* Холодная вода - D5 - GPIO14
* Горячая вода - D2 - GPIO4
* ALARM - D6 - GPIO12


## Ссылки ##
* [esptool-gui](https://github.com/Rodmg/esptool-gui)
* [esptool](https://github.com/espressif/esptool)


## Код для заливки прошивки ##
```
#!bash

./esptool.py --port /dev/cu.wchusbserial1420 write_flash -fm=dio -fs=4MB 0x00000 ~/Downloads/0x00000_ESP8266_201705062105.bin

```