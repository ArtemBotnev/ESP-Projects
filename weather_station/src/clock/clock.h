// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#ifndef CLOCK_H
#define CLOCK_H

#include <DS3231.h>
#include "compile_time.h"

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

private:
    static const uint8_t TIME_STRING_SIZE = 17;
    const char *TIME_PATTERN = "%02d:%02d %02d.%02d.%d";

    DS3231 _rtc;
    RTCDateTime _dt;

    uint8_t previousHourValue;
};

#endif //CLOCK_H
