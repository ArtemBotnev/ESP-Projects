// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#include "clock.h"

void TClock::init() {
    _rtc.begin();
    RTCDateTime dt = _rtc.getDateTime();

    if (dt.year < __TIME_YEARS__ || dt.month < __TIME_MONTH__ || dt.day < __TIME_DAYS__ || dt.hour < __TIME_HOURS__
        || dt.minute < __TIME_MINUTES__) {
        // probably the library bug, unixtime needs to be reduced by an hour
        _rtc.setDateTime(__TIME_UNIX__ - 60 * 60);
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
