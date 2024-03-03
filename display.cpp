#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "clock.h"

#define PIN 2
#define NUMPIXELS 86
#define HOUR_UPPER_OFFSET 0
#define HOUR_LOWER_OFFSET 21
#define MINUTE_UPPER_OFFSET 44
#define MINUTE_LOWER_OFFSET 65
#define DOT_OFFSET 42
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int symbol[17][7]{
  //e  d  c  g  f  a  b 
  { 1, 1, 1, 0, 1, 1, 1 },  //0
  { 0, 0, 1, 0, 0, 0, 1 },  //1
  { 1, 1, 0, 1, 0, 1, 1 },  //2
  { 0, 1, 1, 1, 0, 1, 1 },  //3
  { 0, 0, 1, 1, 1, 0, 1 },  //4
  { 0, 1, 1, 1, 1, 1, 0 },  //5
  { 1, 1, 1, 1, 1, 1, 0 },  //6
  { 0, 0, 1, 0, 0, 1, 1 },  //7
  { 1, 1, 1, 1, 1, 1, 1 },  //8
  { 0, 1, 1, 1, 1, 1, 1 },  //9
  { 0, 0, 0, 1, 0, 0, 0 },  //-
  { 1, 1, 0, 0, 1, 1, 0 },  //C
  { 1, 1, 0, 1, 1, 0, 0 },  //t
  { 1, 0, 0, 1, 1, 1, 1 },  //P
  { 1, 0, 0, 1, 1, 1, 0 },  //F
  { 1, 1, 0, 0, 1, 0, 0 },  //L
  { 1, 0, 1, 1, 1, 1, 1 }   //A

};

void displayInit() {
  pixels.begin();
}

void brightness(int brightness) {
  if (pixels.getBrightness() == brightness) {
    return;
  }
  pixels.setBrightness(brightness);
}

void configureSegment(configStructure *cfg, int symbol_id, int offset) {
  int s = 0;
  while (s != 7) {
    if (symbol[symbol_id][s]) {
      pixels.setPixelColor(s * 3 + offset, cfg->color_active);
      pixels.setPixelColor(s * 3 + 1 + offset, cfg->color_active);
      pixels.setPixelColor(s * 3 + 2 + offset, cfg->color_active);
    }
    ++s;
  }
}

void flipDots(configStructure *cfg, int offset) {
  pixels.setPixelColor(offset, cfg->color_active);
  pixels.setPixelColor(offset + 1, cfg->color_active);
}

void displayTime(configStructure *cfg, rtcData *tm, bool dot_state) {
  pixels.clear();
  configureSegment(cfg, tm->hour / 10, HOUR_UPPER_OFFSET);
  configureSegment(cfg, tm->hour % 10, HOUR_LOWER_OFFSET);
  if (dot_state) {
    flipDots(cfg, DOT_OFFSET);
  }
  configureSegment(cfg, tm->minute / 10, MINUTE_UPPER_OFFSET);
  configureSegment(cfg, tm->minute % 10, MINUTE_LOWER_OFFSET);
  pixels.show();
}

void displayTemperature(configStructure *cfg, rtcData *tm) {
  pixels.clear();
  if (tm->temperature < 0) {
    configureSegment(cfg, 10, HOUR_UPPER_OFFSET);
  }
  configureSegment(cfg, tm->temperature / 10, HOUR_LOWER_OFFSET);
  configureSegment(cfg, tm->temperature % 10, MINUTE_UPPER_OFFSET);
  configureSegment(cfg, 11, MINUTE_LOWER_OFFSET);
  pixels.show();
}

void displayYear(configStructure *cfg, rtcData *tm) {
  pixels.clear();
  configureSegment(cfg, tm->year / 1000, HOUR_UPPER_OFFSET);
  configureSegment(cfg, tm->year / 100 % 10, HOUR_LOWER_OFFSET);
  configureSegment(cfg, tm->year / 10 % 10, MINUTE_UPPER_OFFSET);
  configureSegment(cfg, tm->year % 10, MINUTE_LOWER_OFFSET);
  pixels.show();
}

void displayMonthDay(configStructure *cfg, rtcData *tm) {
  pixels.clear();
  configureSegment(cfg, tm->month / 10, HOUR_UPPER_OFFSET);
  configureSegment(cfg, tm->month % 10, HOUR_LOWER_OFFSET);
  configureSegment(cfg, tm->day / 10, MINUTE_UPPER_OFFSET);
  configureSegment(cfg, tm->day % 10, MINUTE_LOWER_OFFSET);
  pixels.show();
}

void displayFlashMode(configStructure *cfg) {
  pixels.clear();
  configureSegment(cfg, 14, HOUR_LOWER_OFFSET);
  configureSegment(cfg, 15, MINUTE_UPPER_OFFSET);
  configureSegment(cfg, 16, MINUTE_LOWER_OFFSET);
  pixels.show();
}

void displaySetupMode(configStructure *cfg) {
  pixels.clear();
  configureSegment(cfg, 5, HOUR_LOWER_OFFSET);
  configureSegment(cfg, 12, MINUTE_UPPER_OFFSET);
  configureSegment(cfg, 13, MINUTE_LOWER_OFFSET);
  pixels.show();
}
