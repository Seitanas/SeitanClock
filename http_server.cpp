#include <Arduino.h>
#include "config.h"
#include "http_server.h"
#include "html_templates.h"
#include "clock.h"
#include "flash.h"


ESP8266WebServer server(80);

bool isNumeric(String val) {
  for (byte i = 0; i < val.length(); i++) {
    // allow "-" sign if its the first character in string and string is longer than 1 char
    if (i == 0 && val.charAt(i) == '-' && val.length() > 1) {
      continue;
    }
    if (!isDigit(val.charAt(i))) {
      return false;
    }
  }
  return true;
}

bool isColor(String val) {
  val.remove(0, 1);  // remove # from the beginning
  if (val.length() == 6) {
    return true;
  }
  return false;
}

int sanitizeInt(int val, int min, int max) {
  if (val > max) {
    val = max;
  }
  if (val < min) {
    val = min;
  }
  return val;
}

uint32_t sanitizeColor(String val) {
  val.remove(0, 1);  // remove # from the beginning
  return (uint32_t)strtol(val.c_str(), 0, 16);
}

bool stringValid(String val) {
  if (val.length() > 32) {
    return false;
  }
  return true;
}

void handleRoot(configStructure *cfg) {
  if (!server.authenticate(cfg->admin_username.c_str(), cfg->admin_password.c_str())) {
    server.requestAuthentication();
    return;
  }
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/html", root_page(cfg));
}

void handleSave(configStructure *cfg) {
  if (!server.authenticate(cfg->admin_username.c_str(), cfg->admin_password.c_str())) {
    server.requestAuthentication();
    return;
  }
  if (stringValid(server.arg("wifi_ssid"))) {
    cfg->wifi_ssid = server.arg("wifi_ssid");
  }
  if (stringValid(server.arg("wifi_password"))) {
    cfg->wifi_password = server.arg("wifi_password");
  }
  if (stringValid(server.arg("host_name"))) {
    cfg->host_name = server.arg("host_name");
  }
  if (stringValid(server.arg("admin_username"))) {
    cfg->admin_username = server.arg("admin_username");
  }
  if (stringValid(server.arg("admin_password"))) {
    cfg->admin_password = server.arg("admin_password");
  }
  if (stringValid(server.arg("ntp_server"))) {
    cfg->ntp_server = server.arg("ntp_server");
  }
  if (isColor(server.arg("color_day"))) {
    cfg->color_day = sanitizeColor(server.arg("color_day"));
  }
  if (isColor(server.arg("color_night"))) {
    cfg->color_night = sanitizeColor(server.arg("color_night"));
  }
  if (isNumeric(server.arg("brightness_day"))) {
    cfg->brightness_day = sanitizeInt(server.arg("brightness_day").toInt(), 1, 1024);
  }
  if (isNumeric(server.arg("brightness_night"))) {
    cfg->brightness_night = sanitizeInt(server.arg("brightness_night").toInt(), 1, 1024);
  }
  if (isNumeric(server.arg("light_sensor_value"))) {
    cfg->light_sensor_value = sanitizeInt(server.arg("light_sensor_value").toInt(), 1, 1024);
  }
  if (isNumeric(server.arg("tz"))) {
    cfg->tz = sanitizeInt(server.arg("tz").toInt(), -12, 12);
  }
  if (server.arg("daylight_saving") == "on") {
    cfg->daylight_saving = true;
  } 
  else {
    cfg->daylight_saving = false;
  }
  writeConfig(cfg);
  updateNTPTime(cfg);
  server.sendHeader("Location", "/");
  server.send(302, "text/html", "");
}

void handleFlash(configStructure *cfg) {
  if (!server.authenticate(cfg->admin_username.c_str(), cfg->admin_password.c_str())) {
    server.requestAuthentication();
    return;
  }
  server.send(200, "text/html", flash_page(cfg));
  startFlash(cfg);
}

void handleRestart(configStructure *cfg) {
  if (!server.authenticate(cfg->admin_username.c_str(), cfg->admin_password.c_str())) {
    server.requestAuthentication();
    return;
  }
  server.sendHeader("Location", "/");
  server.send(302, "text/html", "");
  ESP.restart();
}

void startWebServer(configStructure *cfg) {
  server.on("/", HTTP_GET, std::bind(&handleRoot, cfg));
  server.on("/save", HTTP_POST, std::bind(&handleSave, cfg));
  server.on("/flash", HTTP_POST, std::bind(&handleFlash, cfg));
  server.on("/restart", HTTP_POST, std::bind(&handleRestart, cfg));
  server.begin();
}

void handleWebServer() {
  server.handleClient();
}
