// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
//#include <UniversalTelegramBot.h>
//#include <DELAY.h>

class WifiConnectManager {

public:
    WifiConnectManager();

    void init(const char* ssid, const char* password);

    bool connectionEstablished();

//    WiFiClientSecure getClient();

private:
//    WiFiClientSecure client;
};

#endif //NETWORK_H
