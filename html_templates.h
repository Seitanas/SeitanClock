#include "light_sensor.h"
#include "version.h"

String stylesheet() {
  return "<style>\n"
  ".center {\n"
  "margin: auto;\n"
  "width: 300px;\n"
  "border: 0px;\n"
  "padding: 10px;\n"
  "overflow: hidden;\n"
  "white-space: nowrap;\n"
  "}\n"
  "#config label {\n"
  "display:flex;\n"
  "flex-direction:column\n"
  "}\n"
  "input[type=text], input[type=password], select {\n"
  "width: 100%;\n"
  "padding: 12px 20px;\n"
  "margin: 8px 0;\n"
  "display: inline-block;\n"
  "border: 1px solid #ccc;\n"
  "border-radius: 4px;\n"
  "box-sizing: border-box;\n"
  "}\n"
  "input[type=submit] {\n"
  "width: 100%;\n"
  "background-color: #4CAF50;\n"
  "color: white;\n"
  "padding: 14px 20px;\n"
  "margin: 8px 0;\n"
  "border: none;\n"
  "border-radius: 4px;\n"
  "cursor: pointer;\n"
  "}\n"
  "input[type=submit]:hover {\n"
  "background-color: #45a049;\n"
  "}\n"
  "div {\n"
  "border-radius: 5px;\n"
  "background-color: #f2f2f2;\n"
  "padding: 20px;\n"
  "}\n"
  "</style>\n";
}

String root_page(configStructure *cfg) {
  char day_var[8];
  sprintf(day_var, "%06X", (uint32_t) cfg->color_day); 
  char night_var[8];
  sprintf(night_var, "%06X", (uint32_t) cfg->color_night); 
  String dls = "";
  if (cfg -> daylight_saving) {
    dls = " checked";
  }

  return "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<title>" + cfg->host_name + "</title>\n"
  + stylesheet() +
  "</head>\n"
  "<body>\n"
  "<div class=\"center\">\n"
  "<form action=\"/save\" method=\"post\" id=\"config\">\n"
  "<label for=\"host_name\">Hostname:</label>\n"
  "<input type=\"text\" name=\"host_name\" value=\"" + cfg->host_name + "\"><br>\n"
  "<label for=\"wifi_ssid\">Wi-Fi SSID:</label>\n"
  "<input type=\"text\" name=\"wifi_ssid\" value=\"" + cfg->wifi_ssid + "\"><br>\n"
  "<label for=\"wifi_password\">Wi-Fi password:</label>\n"
  "<input type=\"password\" name=\"wifi_password\" value=\"" + cfg->wifi_password + "\"><br>\n"
  "<label for=\"admin_username\">Admin username:</label>\n"
  "<input type=\"text\" name=\"admin_username\" value=\"" + cfg->admin_username + "\"><br>\n"
  "<label for=\"admin_password\">Admin password:</label>\n"
  "<input type=\"password\" name=\"admin_password\" value=\"" + cfg->admin_password + "\"><br>\n"
  "<label for=\"tz\">Timezone:</label>\n"
  "<input type=\"text\" name=\"tz\" value=\"" + cfg->tz + "\"><br>\n"
  "<label for=\"daylight_saving\">Daylight saving:</label>\n"
  "<input type=\"checkbox\" name=\"daylight_saving\"" + dls + "><br>\n"
  "<label for=\"ntp_server\">NTP server:</label>\n"
  "<input type=\"text\" name=\"ntp_server\" value=\"" + cfg->ntp_server + "\"><br>\n"
  "<label for=\"color_day\">Daytime LED color:</label>\n"
  "<input type=\"color\" name=\"color_day\" value=\"#" + String(day_var) + "\">\n"
  "<label for=\"brightness_day\">Daytime display brightness:</label>\n"
  "<input type=\"text\" name=\"brightness_day\" value=\"" + cfg->brightness_day + "\"><br>\n"
  "<label for=\"color_night\">Nighttime LED color:</label>\n"
  "<input type=\"color\" name=\"color_night\" value=\"#" + String(night_var) + "\">\n"
  "<label for=\"brightness_night\">Nighttime display brightness:</label>\n"
  "<input type=\"text\" name=\"brightness_night\" value=\"" + cfg->brightness_night + "\"><br>\n"
  "<label for=\"light_sensor_value\">Daytime light sensor value:</label>\n"
  "<input type=\"text\" name=\"light_sensor_value\" value=\"" + cfg->light_sensor_value + "\"><br>\n"
  "Current light sensor reading: " + lightValue() + "<br>\n"
  "<input type=\"submit\" value=\"Save\">\n"
  "</form>\n"
  "<form action=\"/flash\" method=\"post\" onSubmit=\"return confirm('Do you want to enable flash mode?') \" id=\"config\">\n"
  "<input type=\"submit\" value=\"Enable flash mode\">\n"
  "</form>\n"
  "<form action=\"/restart\" method=\"post\" onSubmit=\"return confirm('Restart clock?') \" id=\"config\">\n"
  "<input type=\"submit\" value=\"Restart\">\n"
  "</form>\n<br>"
  "Firmware version: " + VERSION + "\n"
  "</div>\n"
  "</body>\n"
  "</html>";
}

String flash_page(configStructure *cfg) {
  return "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<title>" + cfg->host_name + " flash mode</title>\n"
  + stylesheet() +
  "</head>\n"
  "<body>\n"
  "<div class=\"center\">\n"
  "<b>Flash mode enabled.<br>Your device IP is: " + cfg->ip_address + "</b>\n"
  "</div>"
  "</body>\n"
  "</html>";
}

