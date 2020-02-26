// Weather station
// Copyright Artem Botnev 2019-2020
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

    static char result[DATE_TIME_STRING_SIZE];
    sprintf(result, DATE_TIME_PATTERN, _dt.hour, _dt.minute, _dt.day, _dt.month, _dt.year);

    return result;
}

TClock::clockDataContainer TClock::getClockData() {
    _dt = _rtc.getDateTime();

    uint32_t sec = _dt.unixtime - 24 * 60 * 60;

    char date[DATE_STRING_SIZE];
    sprintf(date, DATE_PATTERN, _dt.day, _dt.month, _dt.year);
    char time[TIME_STRING_SIZE];
    sprintf(time, TIME_PATTERN, _dt.hour, _dt.minute);

    return clockDataContainer { sec, _dt.hour, _dt.minute, date, time };
}
