// Weather station
// Copyright Artem Botnev 2020
// MIT License

#include "telegram_service.h"

TelegramService::TelegramService(const char *token, WiFiClientSecure *client):
    _bot { UniversalTelegramBot(token, *client) } {}

void TelegramService::botInteraction(timePack time, measureSet<int16_t> *(*measureArrayGetter)()) {
    uint8_t newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);

    // there are no incoming message
    if(!newMessagesCount) return;

    measureSet<int16_t> *measures = measureArrayGetter();

    while(newMessagesCount) {
        for (uint8_t i = 0; i < newMessagesCount; i++) {
            replay(i, measures);
        }
        newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);
    }
}

void TelegramService::replay(uint8_t messageNum, measureSet<int16_t> *measures) {
    _bot.sendMessage(_bot.messages[messageNum].chat_id, _bot.messages[messageNum].text, "");
}