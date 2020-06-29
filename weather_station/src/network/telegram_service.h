// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef TELEGRAM_SERVICE_H
#define TELEGRAM_SERVICE_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

class TelegramService {

public:
    TelegramService(const char *token, WiFiClientSecure *client);

    void botInteraction();

private:
    UniversalTelegramBot _bot;
};

#endif //TELEGRAM_SERVICE_H
