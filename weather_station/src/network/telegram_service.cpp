// Weather station
// Copyright Artem Botnev 2020
// MIT License

#include "telegram_service.h"

TelegramService::TelegramService(const char *token, WiFiClientSecure *client):
    _bot { UniversalTelegramBot(token, *client) } {}

void TelegramService::botInteraction() {
    uint8_t newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);

    while(newMessagesCount) {
        for (uint8_t i = 0; i < newMessagesCount; i++) {
            _bot.sendMessage(_bot.messages[i].chat_id, _bot.messages[i].text, "");
        }
        newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);
    }
}