// Weather station
// Copyright Artem Botnev 2019
// MIT License

#include "clock.h"

void TClock::init() {
    _rtc.begin();
    if (_dt.year < 2019) {
        // Set sketch compiling time
        _rtc.setDateTime(__DATE__, __TIME__);
//         Manual (YYYY, MM, DD, HH, II, SS)
//        _rtc.setDateTime(2019, 10, 29, 8, 27, 00);
    }
}

const char *TClock::getTime() {
    _dt = _rtc.getDateTime();

    static char result[TIME_STRING_SIZE];
    sprintf(result, TIME_PATTERN, _dt.hour, _dt.minute, _dt.day, _dt.month, _dt.year);

    return result;
}
