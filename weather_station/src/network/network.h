// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
//#include <DELAY.h>

#include "telegram_service.h"

class NetworkManager {

public:
    NetworkManager();

    void init(const char* ssid, const char* password);

    bool connectionEstablished();

    void runTasks();

    void initTelegramService(const char* token);

private:
    WiFiClientSecure _client;
    TelegramService *_telegramService;
};

#endif //NETWORK_H
