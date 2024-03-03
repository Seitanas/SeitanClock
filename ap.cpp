/*
In AP mode clock works with default values.
Client must connect to `SeitanClock-setup network and use bowser to open http://seitanclock-setup.local or http://192.168.4.1 address.
Use default credentials (admin:admin) to connect to config portal.
*/

#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "http_server.h"

void startAP(configStructure *cfg) {
  Serial.println("Stariting AP mode");
  WiFi.softAP("SeitanClock-setup", "seitanclock");
  startWebServer(cfg);
  MDNS.begin("SeitanClock-setup");
  while(true){
    handleWebServer();
    MDNS.update();
    delay(1);
  }
}
