// Weather station
// Copyright Artem Botnev 2020
// MIT License

#include "network.h"

NetworkManager::NetworkManager() = default;

void NetworkManager::init(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

bool NetworkManager::connectionEstablished() {
    return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::runTasks() {
    if (_telegramService) {
        _telegramService->botInteraction();
    }
}

void NetworkManager::initTelegramService(const char* token) {
    _telegramService = new TelegramService(token, &_client);
}