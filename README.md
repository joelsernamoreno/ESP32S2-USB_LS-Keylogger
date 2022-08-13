# ESP32S2-USB_LS-Keylogger

**Idea, development and implementation:** @JoelSernaMoreno & @TobozoTagada

## Introduction

The sketch implements a hardware keylogger for the ESP32-S2 modules. ESP32-S2 modules do not support device and host at the same time, but it is possible to use USB Host Soft to implement this.

At this time, our keylogger with ESP32-S2 is limited, it only works with USB Low Speed keyboards.

The tests and PoC have been done with this keyboard: **UNYKAch Mini Keyboard KB 302**. 

Make sure to use a USB-LS keyboard for your tests.

## Installation

1. Install esptool: sudo apt install esptool
2. Install pyserial: sudo pip install pyserial
3. Download and Install the Arduino IDE: https://www.arduino.cc/en/main/software
4. Download Evil Crow RF V2 repository: git clone https://github.com/joelsernamoreno/ESP32S2-USB_LS-Keylogger.git
5. Download the EspTinyUSB library in the Arduino library directory: git clone https://github.com/chegewara/EspTinyUSB.git
6. Download the ESPAsyncWebServer library in the Arduino library directory: git clone https://github.com/me-no-dev/ESPAsyncWebServer.git
7. Download the AsyncTCP library in the Arduino library directory: git clone https://github.com/me-no-dev/AsyncTCP.git
8. Edit AsyncTCP/src/AsyncTCP.h and change the following:

* #define CONFIG_ASYNC_TCP_USE_WDT 1 to #define CONFIG_ASYNC_TCP_USE_WDT 0 

9. Open Arduino IDE
10. Go to File - Preferences. Locate the field "Additional Board Manager URLs:" Add "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json" without quotes. Click "Ok"
12. Select Tools - Board - Boards Manager. Search for "esp32". Install "esp32 by Espressif system version 2.0.4". Click "Close".
13. Open the firmware/firmware.ino sketch
14. Select Tools:
    * Board - "ESP32S2 Dev Module".
    * Upload Speed - "921600".
    * USB CDC On Boot - "Disabled".
    * USB Firmware MSC On Boot - "Disabled".
    * USB DFU On Boot - "Disabled".
    * All other options: Default
15. Flash the firmware

## Hardware support

The PoC is based on MouseLab's Nano ESP32-S2 and a female USB breakout

The ESP32-S2 and USB breakout connections have been made with the ESP32S2 GPIO8 and GPIO9 pins. You can change the pins in the sketch

![Keylogger](https://github.com/joelsernamoreno/ESP32S2-USB_LS-Keylogger/blob/main/images/keylogger.jpeg)

## First steps

1. Visualize the wifi networks around you and connect to the ESP32-S2 (default SSID: ESP32S2 USB-LS Keylogger).
2. Enter the password for the wifi network (default password: 123456789).
3. Open a browser and access the web panel (default IP: 192.168.4.1).

![webpanel](https://github.com/joelsernamoreno/ESP32S2-USB_LS-Keylogger/blob/main/images/webpanel.png)

![logs](https://github.com/joelsernamoreno/ESP32S2-USB_LS-Keylogger/blob/main/images/logs.png)

**NOTE:** This is a proof of concept, the logs disappear when the device is disconnected.

