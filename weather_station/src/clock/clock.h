// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#ifndef CLOCK_H
#define CLOCK_H

#include "../common.h"
#include <DS3231.h>

#define TIME_DATE_PATTERN "%02d:%02d %02d.%02d.%d"
#define TIME_DATE_STRING_SIZE 17
#define TIME_PATTERN "%02d:%02d:%02d"
#define TIME_STRING_SIZE 9
#define DATE_PATTERN "%02d.%02d.%d"
#define DATE_STRING_SIZE 11

class TClock {
public:
    void init();

    const char *getTimeString();

    /**
     * checks for a new day
     *
     * @return true if a new day has come, otherwise false
     */
    bool isNewDay();

    timePack getTimePack();

private:
    DS3231 _rtc;
    RTCDateTime _dt;

    uint8_t previousHourValue;
};

#endif //CLOCK_H
