// Weather station
// Copyright Artem Botnev 2020
// MIT License

#include "network.h"

WifiConnectManager::WifiConnectManager() = default;

void WifiConnectManager::init(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

bool WifiConnectManager::connectionEstablished() {
    return WiFi.status() != WL_CONNECTED;
}

//WiFiClientSecure WifiConnectManager::getClient() {
//
//}