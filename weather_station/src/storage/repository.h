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

    /**
     * changes default frequency of cache state
     * can be used optionally because default value exists.
     *
     * @param minutes interval of savings
     */
    void setSaveStateFrequency(uint8_t minutes);

private:
    const char *networkPropertyFile = "/network.json";
    const char *stateFile = "/saved_state.json";

    // errors
    char *errorStorageNotAvailable =
            "External storage not available. Attache SD card and enable property DataManager::useExternalStorage";
    char *errorReadingCard = "Cannot read SD card!";
    char *errorFileOpening = "Error of file opening!";
    char *errorJsonDeserialization = "Error of json deserialization! - ";

    // must be not less than 1 and not more than 59 minutes
    uint8_t saveStateFrequency = 10; // 10 minutes interval by default
    uint8_t currentMinute;

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

    /**
     * saves current state of repository cache data to .json file on SD card
     *
     * @return true if state successfully saved, otherwise false
     */
    bool saveState();

    /**
     * reads state from repository .json file on SD card and store it to cache
     *
     * @return true if state successfully read, otherwise false
     */
    bool readState();

    item *getCacheItemByType(MeasureType type);
};

#endif //STORAGE_REPOSITORY_H
