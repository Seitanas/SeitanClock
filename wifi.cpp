#include <ESP8266WiFi.h>
#include "config.h"

void wifiConnect(configStructure *cfg) {
  WiFi.setHostname(cfg->host_name.c_str());
  WiFi.begin(cfg->wifi_ssid, cfg->wifi_password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  cfg->ip_address = WiFi.localIP().toString();
  Serial.println("Connected, IP address: " + cfg->ip_address);
}

