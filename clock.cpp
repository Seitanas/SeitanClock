#include <NTPClient.h>
#include <RTClib.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include "config.h"
#include "clock.h"

RTC_DS3231 rtc;

void clockInit() {
  Wire.begin(5, 4);
  rtc.begin();
  DateTime now = rtc.now();
}

void updateNTPTime(configStructure *cfg) {
  Serial.println("Syncing time with " + cfg->ntp_server);
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, cfg->ntp_server.c_str());
  timeClient.begin();
  timeClient.setTimeOffset((cfg->tz * 3600) + (cfg->current_daylight_setting * 3600));
  // run NTP query multiple times if there is NTP failure. Update RTC only if NTP query is successfull.
  for (int i = 0; i < 3; i++) {
    if (timeClient.update()) {
      rtc.adjust(timeClient.getEpochTime());
      break;
    }
  }
}

int daylightSaving(configStructure *cfg, rtcData *tm) {
  if (!cfg->daylight_saving) {
    return 0;
  }
  // summer daylight savings happen on last Sunday of March at 01:00 UTC, ends on last Sunday of October at 01:00 UTC.
  int overflow_hour = cfg->tz + 1;
  // check for summer time period:
  if (tm->month > 3 && tm->month < 10) {  // its always +1 from April to September
    return 1;
  }

  if (tm->month == 3) {                                             // March. Leaping month. Summer time begins.
    if (tm->dow == 0) {                                             // Sunday
      if (tm->day >= 25 && tm->hour >= (overflow_hour + 1)) {  // last Sunday of the month 01:00 UTC
        return 1;
      }
    }
    else if ((tm->day - tm->dow + 7) > 31) {  // weekdays that are after last Sunday of the month
      return 1;
    }
    return 0;  // leap date not reached. Still winter time
  }

  if (tm->month == 10) {                                           // October. Leaping month. Summer time ends.
    if (tm->dow == 0) {                                            // Sunday
      if (tm->day >= 25 && tm->hour >= (overflow_hour + 1)) {  // last sunday of the month 01:00 UTC
        return 0;
      }
    }
    else if ((tm->day - tm->dow + 7) > 31) {  // weekdays that are after last Sunday of the month
      return 0;
    }
    return 1;  // leap date not reaced. Still summer time.
  }
  return 0;
}

void getTime(configStructure *cfg, rtcData *tm) {
  if (cfg->current_daylight_setting != daylightSaving(cfg, tm)){
    cfg->current_daylight_setting = daylightSaving(cfg, tm);
    updateNTPTime(cfg);
  }
  DateTime now = rtc.now();
  tm->year = now.year();
  tm->month = now.month();
  tm->dow = now.dayOfTheWeek();
  tm->day = now.day();
  tm->hour = now.hour();
  tm->minute = now.minute();
  tm->second = now.second();
  tm->temperature = rtc.getTemperature();
  if (tm->hour == 0 && tm->minute == 0 && tm->second == 0) {  // query NTP every night at 00:00:00
    updateNTPTime(cfg);
  }
}
