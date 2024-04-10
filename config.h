#include <Arduino.h>

struct configStructure {
  String wifi_ssid = "";
  String wifi_password = "";
  String host_name = "SeitanClock";
  String admin_username = "admin";
  String admin_password = "admin";
  String ntp_server = "time.google.com";
  String ip_address = "";
  uint32_t color_day = 0x1cabfc;
  uint32_t color_night = 0x1cabfc;
  uint32_t color_active = color_day;
  int brightness_day = 150;
  int brightness_night = 2;
  int light_sensor_value = 700;
  int tz = 0;
  bool daylight_saving = false;
  int current_daylight_setting = 0;
};

void readConfig(configStructure *cfg);
void writeConfig(configStructure *cfg);
