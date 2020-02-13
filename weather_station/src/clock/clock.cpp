// Weather station
// Copyright Artem Botnev 2019
// MIT License

#include "clock.h"

void TClock::init() {
    _rtc.begin();

    const uint32_t curUnixtime = _rtc.getDateTime().unixtime;
    _rtc.setDateTime(__DATE__, __TIME__);
    _dt = _rtc.getDateTime();
    // check if compile time less than current
    if (curUnixtime > _dt.unixtime) {
        // probably the library bug, curUnixtime needs to be reduced by a day
        _rtc.setDateTime(curUnixtime - 24 * 60 * 60);
    }
}

const char *TClock::getTimeString() {
    _dt = _rtc.getDateTime();

    static char result[TIME_STRING_SIZE];
    sprintf(result, TIME_PATTERN, _dt.hour, _dt.minute, _dt.day, _dt.month, _dt.year);

    return result;
}
