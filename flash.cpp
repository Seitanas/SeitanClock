#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "config.h"
#include "clock.h"
#include "display.h"

void startFlash(configStructure *cfg) {
  ArduinoOTA.setPassword(cfg->admin_password.c_str());
  String h = cfg->host_name + "-flash";
  ArduinoOTA.setHostname(h.c_str());
  ArduinoOTA.begin();
  displayFlashMode(cfg);
  while(true){
    ArduinoOTA.handle();
    delay(1);
  }
}