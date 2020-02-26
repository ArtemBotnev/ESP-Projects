// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#ifndef CLOCK_H
#define CLOCK_H

#include <DS3231.h>

class TClock {
public:
    struct clockDataContainer {
        uint32_t secondsStamp;
        uint8_t hourOfDay;
        uint8_t minuteOfHour;
        const char *date;
        const char *time;
    };

    void init();

    const char *getTimeString();

    clockDataContainer getClockData();

private:
    static const uint8_t DATE_TIME_STRING_SIZE = 17;
    const char *DATE_TIME_PATTERN = "%02d:%02d %02d.%02d.%d";
    static const uint8_t DATE_STRING_SIZE = 11;
    const char *DATE_PATTERN = "%02d.%02d.%d";
    static const uint8_t TIME_STRING_SIZE = 6;
    const char *TIME_PATTERN = "%02d:%02d";

    DS3231 _rtc;
    RTCDateTime _dt;
};

#endif //CLOCK_H
