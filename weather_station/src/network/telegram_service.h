// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef TELEGRAM_SERVICE_H
#define TELEGRAM_SERVICE_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "../common.h"

class TelegramService {

public:
    /**
     * telegram service initialization
     * @param token - bot token
     * @param client - wifi network client
     */
    TelegramService(const char *token, WiFiClientSecure *client);

    /**
     * starts dot work
     * @param time - timePack representing measures time
     * @param measureArrayGetter - function for receiving measures data
     */
    void botInteraction(timePack time, measureSet<int16_t> *(*measureArrayGetter)());

private:
    UniversalTelegramBot _bot;

    const char *_str_temper = "Temperature";
    const char *_str_hum = "Humidity";
    const char *_str_press = "Pressure";

    const char *_str_out = "out";
    const char *_str_room = "room";

    const char *_str_current = "current";
    const char *_str_min = "min";
    const char *_str_max = "max";
    const char *_str_av = "average";

    void replay(uint8_t messageNum, timePack time, measureSet<int16_t> *measures);

    const char *buildTemperBlock(measureSet<int16_t> *measures, bool isFullBlock);

    const char *buildHumBlock(measureSet<int16_t> *measures, bool isFullBlock);

    const char *buildPressureBlock(measureSet<int16_t> *measures, bool isFullBlock);

    const char *buildShortPressureBlock(measureSet<int16_t> measure);

    const char *buildOutRoomShortBlock(const char *title, measureSet<int16_t> out, measureSet<int16_t> room);

    const char *buildFullPressureBlock(measureSet<int16_t> measure);

    const char *buildOutRoomFullBlock(const char *title, measureSet<int16_t> out, measureSet<int16_t> room);

    const char *buildFullMeasureBlock(measureSet<int16_t> measure);
};

#endif //TELEGRAM_SERVICE_H
