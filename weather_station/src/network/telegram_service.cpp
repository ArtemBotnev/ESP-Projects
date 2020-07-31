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
            replay(i, time, measures);
        }
        newMessagesCount = _bot.getUpdates(_bot.last_message_received + 1);
    }
}

void TelegramService::replay(uint8_t messageNum, timePack time, measureSet<int16_t> *measures) {
    String message = _bot.messages[messageNum].text;

    bool isLatinF = message[0] == 'f' || message[0] == 'F';
    bool isCyrillicP = message[0] == 0xd0 && (message[1] == 0x9f || message[1] == 0xbf);
    bool printFullBlock = isLatinF || isCyrillicP;

    const char *temperBlock = buildTemperBlock(measures, printFullBlock);
    const char *humBlock = buildHumBlock(measures, printFullBlock);
    const char *pressBlock = buildPressureBlock(measures, printFullBlock);

    uint16_t resultSize = strlen(time.timeDate) + strlen(temperBlock) + strlen(humBlock) + strlen(pressBlock);
    char result[resultSize];
    sprintf(result, "%s\n\n%s\n\n%s\n\n%s", time.timeDate, temperBlock, humBlock, pressBlock);

    free((void *) temperBlock);
    free((void *) humBlock);
    free((void *) pressBlock);

    _bot.sendMessage(_bot.messages[messageNum].chat_id, result, "");
}

const char *TelegramService::buildTemperBlock(measureSet<int16_t> *measures, bool isFullBlock) {
    measureSet<int16_t> room = measures[(int) ROOM_TEMPER];
    measureSet<int16_t> out = measures[(int) OUT_TEMPER];

    if (isFullBlock) {
        return buildOutRoomFullBlock(_str_temper, out, room);
    } else {
        return buildOutRoomShortBlock(_str_temper, out, room);
    }
}

const char *TelegramService::buildHumBlock(measureSet<int16_t> *measures, bool isFullBlock) {
    measureSet<int16_t> room = measures[(int) ROOM_HUM];
    measureSet<int16_t> out = measures[(int) OUT_HUM];

    if (isFullBlock) {
        return buildOutRoomFullBlock(_str_hum, out, room);
    } else {
        return buildOutRoomShortBlock(_str_hum, out, room);
    }
}

const char *TelegramService::buildPressureBlock(measureSet<int16_t> *measures, bool isFullBlock) {
    measureSet<int16_t> measure = measures[(int) PRESSURE];

    if (isFullBlock) {
        return buildFullPressureBlock(measure);
    } else {
        return buildShortPressureBlock(measure);
    }
}

const char *TelegramService::buildShortPressureBlock(measureSet<int16_t> measure) {
    char *block = (char *) calloc(sizeof(_str_press) + 5, sizeof(char*));
    sprintf(block, "%s: %d", _str_press, measure.curValue);

    return block;
}

const char *
TelegramService::buildOutRoomShortBlock(const char *title, measureSet<int16_t> out, measureSet<int16_t> room) {
    uint8_t outSize = sizeof(_str_out) + 5;
    char outStr[outSize];
    sprintf(outStr, "%s: %d", _str_out, out.curValue);
    uint8_t roomSize = sizeof(_str_room) + 5;
    char roomStr[roomSize];
    sprintf(roomStr, "%s: %d", _str_room, room.curValue);

    char *block = (char *) calloc(strlen(title) + outSize + roomSize + 2, sizeof(char*));
    sprintf(block, "%s\n\t%s\n\t%s", title, outStr, roomStr);

    return block;
}

const char *
TelegramService::buildOutRoomFullBlock(const char *title, measureSet<int16_t> out, measureSet<int16_t> room) {
    const char *outMeasuresBlock = buildFullMeasureBlock(out);
    uint8_t outSize = sizeof(_str_out) + 4 + strlen(outMeasuresBlock);
    char outStr[outSize];
    sprintf(outStr, "\t%s -\n%s", _str_out, outMeasuresBlock);
    free((void *) outMeasuresBlock);

    const char *roomMeasureBlock = buildFullMeasureBlock(room);
    uint8_t roomSize = sizeof(_str_room) + 4 + strlen(roomMeasureBlock);
    char roomStr[roomSize];
    sprintf(roomStr, "\t%s -\n%s", _str_room, roomMeasureBlock);
    free((void *) roomMeasureBlock);

    char *block = (char *) calloc(strlen(title) + outSize + roomSize + 2, sizeof(char *));
    sprintf(block, "%s\n%s\n%s", title, outStr, roomStr);

    return block;
}

const char *TelegramService::buildFullPressureBlock(measureSet<int16_t> measure) {
    const char *measureBlock = buildFullMeasureBlock(measure);
    char *block = (char *) calloc(sizeof(_str_press) + strlen(measureBlock) + 3, sizeof(char*));
    sprintf(block, "%s\n%s", _str_press, measureBlock);
    free((void *) measureBlock);

    return block;
}

const char *TelegramService::buildFullMeasureBlock(measureSet<int16_t> measure) {
    uint8_t blockSize = sizeof(_str_current) + sizeof(_str_min) + sizeof(_str_max) + sizeof(_str_av) + 8 * 4;
    char *block = (char *) calloc(blockSize, sizeof(char*));
    sprintf(
            block,
            "\t\t%s: %d\n\t\t%s: %d\n\t\t%s: %d\n\t\t%s: %d",
            _str_current, measure.curValue,
            _str_min, measure.min,
            _str_max, measure.max,
            _str_av, (int16_t)(measure.average + 0.5f)
    );

    return block;
}