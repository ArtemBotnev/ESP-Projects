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
//        _rtc.setDateTime(curUnixtime - 24 * 60 * 60);
        _rtc.setDateTime(curUnixtime);
    }
}

const char *TClock::getTimeString() {
    _dt = _rtc.getDateTime();

    static char result[TIME_DATE_STRING_SIZE];
    sprintf(result, TIME_DATE_PATTERN, _dt.hour, _dt.minute, _dt.day, _dt.month, _dt.year);

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

timePack TClock::getTimePack() {
    _dt = _rtc.getDateTime();

    static char timeDate[TIME_DATE_STRING_SIZE];
    sprintf(timeDate, TIME_DATE_PATTERN, _dt.hour, _dt.minute, _dt.day, _dt.month, _dt.year);
    static char time[TIME_STRING_SIZE];
    sprintf(time, TIME_PATTERN, _dt.hour, _dt.minute, _dt.second);
    static char date[DATE_STRING_SIZE];
    sprintf(date, DATE_PATTERN, _dt.day, _dt.month, _dt.year);

    return timePack { _dt.unixtime, _dt.day, _dt.minute, timeDate, time, date };
}
