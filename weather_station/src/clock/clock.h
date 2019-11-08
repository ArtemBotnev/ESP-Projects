// Weather station
// Copyright Artem Botnev 2019
// MIT License

#ifndef CLOCK_H
#define CLOCK_H

#include <DS3231.h>

class TClock {
public:
    void init();

    const char *getTime();

private:
    static const uint8_t TIME_STRING_SIZE = 17;
    const char *TIME_PATTERN = "%02d:%02d %02d.%02d.%d";

    DS3231 _rtc;
    RTCDateTime _dt;
};

#endif //CLOCK_H
