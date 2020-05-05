// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#include "clock.h"

void TClock::init() {
    _rtc.begin();
    // probably the library bug, unixtime needs to be reduced by a hour
    uint32_t unixtime = __TIME_UNIX__ - 60 * 60;
    // check if clock's time less than compile time
    if (_rtc.getDateTime().unixtime < unixtime) {
        _rtc.setDateTime(unixtime);
    }
}

const char *TClock::getTimeString() {
    _dt = _rtc.getDateTime();

    static char result[TIME_STRING_SIZE];
    sprintf(result, TIME_PATTERN, _dt.hour, _dt.minute, _dt.day, _dt.month, _dt.year);

    return result;
}

bool TClock::isNewDay() {
    bool result = false;
    if (previousHourValue == 23 && _dt.hour == 0) {
        result = true;
    }
    previousHourValue = _dt.hour;

    return result;
}
