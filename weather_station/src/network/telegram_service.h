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

    void replay(uint8_t messageNum, measureSet<int16_t> *measures);
};

#endif //TELEGRAM_SERVICE_H
