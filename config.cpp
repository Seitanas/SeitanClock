#include "config.h"
#include <FS.h>
#include <ArduinoJson.h>

#define BUFFZIZE 512

void initializeFS() {
  bool success = SPIFFS.begin();
  if (success) {
    Serial.println("File system mounted with success");
  }
  else {
    Serial.println("Error mounting the file system");
  }
}

void readConfig(configStructure *cfg) {
  initializeFS();

  File config_file = SPIFFS.open("/config.json", "r");
  if (!config_file) {
    Serial.println("Failed to open config file for reading");
    return;
  }
  else {
    Serial.println("Reading config file");
    StaticJsonDocument<BUFFZIZE> cf;

    DeserializationError error = deserializeJson(cf, config_file);
    if (error) {
      Serial.println("Failed to read file, using default configuration");
      return;
    }
    cfg->wifi_ssid = cf["wifi_ssid"] | cfg->wifi_ssid;
    cfg->wifi_password = cf["wifi_password"] | cfg->wifi_password;
    cfg->host_name = cf["host_name"] | cfg->host_name;
    cfg->admin_username = cf["admin_username"] | cfg->admin_username;
    cfg->admin_password = cf["admin_password"] | cfg->admin_password;
    cfg->ntp_server = cf["ntp_server"] | cfg->ntp_server;
    cfg->color_day = cf["color_day"] | cfg->color_day;
    cfg->color_night = cf["color_night"] | cfg->color_night;
    cfg->brightness_day = cf["brightness_day"] | cfg->brightness_day;
    cfg->brightness_night = cf["brightness_night"] | cfg->brightness_night;
    cfg->light_sensor_value = cf["light_sensor_value"] | cfg->light_sensor_value;
    cfg->tz = cf["tz"] | cfg->tz;
    cfg->daylight_saving = cf["daylight_saving"] | cfg->daylight_saving;
    config_file.close();
  }
}

void writeConfig(configStructure *cfg) {
  File config_file = SPIFFS.open("/config.json", "w");
  if (!config_file) {
    Serial.println("Failed to open config file for writing");
    return;
  }
  else {
    Serial.println("Writing config file");
  }

  StaticJsonDocument<BUFFZIZE> cf;
  cf["wifi_ssid"] = cfg->wifi_ssid;
  cf["wifi_password"] = cfg->wifi_password;
  cf["host_name"] = cfg->host_name;
  cf["admin_username"] = cfg->admin_username;
  cf["admin_password"] = cfg->admin_password;
  cf["ntp_server"] = cfg->ntp_server;
  cf["color_day"] = cfg->color_day;
  cf["color_night"] = cfg->color_night;
  cf["brightness_day"] = cfg->brightness_day;
  cf["brightness_night"] = cfg->brightness_night;
  cf["light_sensor_value"] = cfg->light_sensor_value;
  cf["tz"] = cfg->tz;
  cf["daylight_saving"] = cfg->daylight_saving;

  if (serializeJson(cf, config_file) == 0) {
    Serial.println("Failed to write to config file");
  }
  config_file.close();
}