#include <ESP8266mDNS.h>
#include "config.h"
#include "ap.h"
#include "http_server.h"
#include "wifi.h"
#include "clock.h"
#include "display.h"
#include "light_sensor.h"


#define SETUP_PIN 13
// for how long show additional info (date/temperature) when button is pressed
#define SHOW_INFO_TIMER 2000

// should be ok for all cases
#define REFRESH_TIME 500

void setup() {
  pinMode(SETUP_PIN, INPUT); 
  Serial.begin(115200);
  displayInit();
  clockInit();
}

void loop() {
  configStructure cfg;
  rtcData tm;
  readConfig(&cfg);
  if (!digitalRead(SETUP_PIN) || cfg.wifi_ssid.length() == 0) {
    configStructure cfg; //reset config to default
    displaySetupMode(&cfg);
    startAP(&cfg);
  }
  wifiConnect(&cfg);
  delay(2000);
  MDNS.begin(cfg.host_name.c_str());
  startWebServer(&cfg);
  updateNTPTime(&cfg);
  int loop_count = 0;
  int light_avg_count = 0;
  float light_avg = 0;
  bool dot_state = false;
  int info_type = 0; // 0 = time 1 = show year 2 = show month+day 3 = show temperature
  int info_timer = 0;
  while (true) {
    handleWebServer();
    MDNS.update();
    if(!digitalRead(SETUP_PIN)) { // if setup button is pressed, display temperature, followed by date
      info_type = 1;
    }
    if (loop_count >= REFRESH_TIME) {
      loop_count = 0;
      if (dot_state) { //read rtc every second (every second time dot_state updates);
        getTime(&cfg, &tm);
      }
      dot_state = !dot_state;
      switch(info_type) {
        case 1:
          displayYear(&cfg, &tm);
          break;
        case 2:
          displayMonthDay(&cfg, &tm);
          break;
        case 3:
          displayTemperature(&cfg, &tm);
          break;
        default:
          displayTime(&cfg, &tm, dot_state);
      } 
      if (light_avg_count == 10){
        light_avg_count = 0;
        if (light_avg > cfg.light_sensor_value){
          cfg.color_active = cfg.color_day;
          brightness(cfg.brightness_day);
        }
        else{
          cfg.color_active = cfg.color_night;
          brightness(cfg.brightness_night);
        }
      }
      light_avg = light_avg * light_avg_count + lightValue();
      ++light_avg_count;
      light_avg = light_avg/(light_avg_count);
    }
    if (info_timer >= SHOW_INFO_TIMER) {
      info_timer = 0;
      ++info_type;
      if (info_type > 3) {
        info_type = 0;
      }

    }
    if (info_type != 0) {
      ++info_timer;      
    }
    ++loop_count;
    delay(1);
  }
}
