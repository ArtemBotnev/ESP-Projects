// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef STORAGE_REPOSITORY_H
#define STORAGE_REPOSITORY_H

#include "../common.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <ArduinoJson.h>

#define CARD_CS_PIN 27

struct networkProperty { const char *ssid, *password; };

class DataManager {

public:
    bool useExternalStorage = true;

    DataManager();

    measureSet<int16_t> getMeasureSet(MeasureType type, int16_t currentVal);

    void clearCache();

    const char *initExternalStorage();

    networkProperty readNetworkProperty();

private:
    const char *networkPropertyFile = "/network.json";
    const char *backupFile = "/saved_state.json";
    fs::FS *fs;
    bool cardAvailable;

    struct item {
        int16_t min;
        int16_t max;
        float average;
        uint16_t factor;
    };

    struct Cache {
        // temperature
        item roomTemp { 100, -100, -100.0f, 1 };
        item outTemp { 100, -100, -100.0f, 1 };
        // humidity
        item roomHum { 100, -100, .0f, 1 };
        item outHum { 100, -100, .0f, 1 };
        // pressure
        item pressure { 1000, -100, .0f, 1 };
    };

    Cache *cache;
};

#endif //STORAGE_REPOSITORY_H
