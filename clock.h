struct rtcData {
  int year = 0;
  int month = 0;
  int dow = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;
  int second = 0;
  int temperature = 0;
};

void clockInit();
void getTime(configStructure *cfg, rtcData *tm);
void updateNTPTime(configStructure *cfg);
