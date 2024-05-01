/*
Copy this file to project's root folder owerwriting default display.cpp
Display is using i2c interface, so you need to connect it on top of RTC module - use same pins,
be aware of +- posioningm (it may differ from RTC module).
*/


#include "1config.h"
#include "clock.h"
#include <HT16K33.h>

HT16K33 seg(0x70);

uint8_t flash_text[4] = { 0x00, 0x71, 0x38, 0x77 };
uint8_t cfg_text[4] = { 0x00, 0x39, 0x71, 0x6F };

void displayInit() {
  seg.begin();
  seg.displayOn();
}

void brightness(int brightness) {
  seg.setBrightness(brightness);
}

void configureSegment(configStructure *cfg, int symbol_id, int offset) {
}

void displayTime(configStructure *cfg, rtcData *tm, bool dot_state) {
  seg.displayTime(tm->hour, tm->minute, dot_state);
}

void displayTemperature(configStructure *cfg, rtcData *tm) {
  seg.displayUnit(float(tm->temperature), 1, HT16K33_C);
  seg.displayColon(0);
}

void displayYear(configStructure *cfg, rtcData *tm) {
  seg.displayInt(tm->year);
}

void displayMonthDay(configStructure *cfg, rtcData *tm) {
  seg.displayTime(tm->month, tm->day, 0);
}

void displayFlashMode(configStructure *cfg) {
  seg.displayRaw(flash_text, false);
}

void displaySetupMode(configStructure *cfg) {
  seg.displayRaw(cfg_text, false);
}
