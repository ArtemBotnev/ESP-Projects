// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#include "repository.h"

DataManager::DataManager() {
    cache = new Cache();
}

measureSet<int16_t> DataManager::getMeasureSet(MeasureType type, int16_t currentVal) {
    item *item = getCacheItemByType(type);
    if (!item) {
        // error
        return measureSet<int16_t> { -1000, -1000, -1000, -1000 };
    }

    if (currentVal < item->min) {
        item->min = currentVal;
    }

    if (currentVal > item->max) {
        item->max = currentVal;
    }

    if (item->factor > 1) {
        item->average = (item->average * item->factor + (float) currentVal) / ++item->factor;
    } else {
        item->average = currentVal;
        item->factor++;
    }

    return measureSet<int16_t> { currentVal, item->min, (int16_t)(item->average + 0.5f), item->max };
}

void DataManager::clearCache() {
    delete cache;
    cache = new Cache();
}

const char *DataManager::initExternalStorage() {
    if (!useExternalStorage) {
        return errorStorageNotAvailable;
    }


    SPIClass * hspi = new SPIClass(HSPI);
    hspi->begin(25, 26, 27, CARD_CS_PIN); //SCLK, MISO, MOSI, SS
    if(!SD.begin(CARD_CS_PIN, *hspi)) {
        return "Card Mount Failed";
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        return "No SD card attached";
    }


    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    static char result[15 + 10];
    sprintf(result, "SD Card Size: %lluMB\n", cardSize);

    fs = &SD;
    cardAvailable = true;

    return result;
}

bool DataManager::isCardAvailable() {
    return cardAvailable;
}

networkProperty DataManager::readNetworkProperty() {
    if (!useExternalStorage) {
        return networkProperty { ERROR, errorStorageNotAvailable };
    }

    if (!cardAvailable) {
        return networkProperty { ERROR, errorReadingCard };
    }

    File file = fs->open(networkPropertyFile);
    if (!file) {
        return networkProperty { ERROR, errorFileOpening };
    }

    const int capacity = JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(1);
    StaticJsonDocument<capacity> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        return networkProperty { ERROR, strcat(errorJsonDeserialization, error.c_str()) };
    }

    file.close();

    const char *ssid = doc[wifi_s][ssid_s];
    const char *pass = doc[wifi_s][pass_s];

    return networkProperty { ssid, pass };
}

void DataManager::setSaveStateFrequency(uint8_t minutes) {
    saveStateFrequency = minutes;
}

bool DataManager::setTimeAndCheckStateSaveAction(
        uint32_t secondsStamp,
        uint8_t minuteOfHour,
        const char *date,
        const char *time) {

    if (minuteOfHour % saveStateFrequency != 0) {
        return true;
    }

    return saveState(secondsStamp, date, time);
}

DataManager::item *DataManager::getCacheItemByType(MeasureType type) {
    switch (type) {
        case (ROOM_TEMPER):
            return &cache->roomTemp;
        case (OUT_TEMPER):
            return &cache->outTemp;
        case (ROOM_HUM):
            return &cache->roomHum;
        case (OUT_HUM):
            return &cache->outHum;
        case (PRESSURE):
            return &cache->pressure;
        default:
            // error
            return NULL;
    }
}

bool DataManager::saveState(uint32_t secondsStamp, const char *date, const char *time) {
    if (!useExternalStorage && !cardAvailable) {
        return false;
    }

    // remove old state file
    fs->remove(stateFile);

    // serialization;
    const int capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(MEASURE_TYPES_COUNT)
            + MEASURE_TYPES_COUNT * JSON_OBJECT_SIZE(5);
    StaticJsonDocument<capacity> doc;
    doc[time_stamp_s] = secondsStamp;
    doc[date_s] = date;
    doc[time_s] = time;

    const int arrayCapacity = JSON_ARRAY_SIZE(MEASURE_TYPES_COUNT) + MEASURE_TYPES_COUNT * JSON_OBJECT_SIZE(5);
    StaticJsonDocument<arrayCapacity> arrayDoc;
    item *item;
    for (uint8_t i = 0; i < MEASURE_TYPES_COUNT; i++) {
        MeasureType type = (MeasureType) i;
        item = getCacheItemByType(type);

        JsonObject obj = arrayDoc.createNestedObject();
        obj[type_index_s] = i;
        obj[min_s] = item->min;
        obj[max_s] = item->max;
        obj[average_s] = item->average;
        obj[factor_s] = item->factor;
    }
    doc[measurements_s] = arrayDoc;

    // write to file
    File file = fs->open(stateFile, FILE_WRITE);
    if (!file) {
        return false;
    }

    if (serializeJson(doc, file) == 0) {
        return false;
    }

    file.close();

    return true;
}

bool DataManager::readState() {
    if (!useExternalStorage && !cardAvailable) {
        return false;
    }

    File file = fs->open(stateFile);
    if (!file) {
        return false;
    }

    // deserialization;
    const int capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(MEASURE_TYPES_COUNT)
                         + MEASURE_TYPES_COUNT * JSON_OBJECT_SIZE(MEASURE_TYPES_COUNT);
    StaticJsonDocument<capacity> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        return false;
    }

    file.close();

    uint32_t timeStamp = doc[time_stamp_s];
    const char *date = doc[date_s];
    const char *time = doc[time_s];

    const int arrayCapacity = JSON_ARRAY_SIZE(MEASURE_TYPES_COUNT) + MEASURE_TYPES_COUNT * JSON_OBJECT_SIZE(5);
    StaticJsonDocument<arrayCapacity> arrayDoc;
    const char* jsonString = doc[measurements_s];
    error = deserializeJson(arrayDoc, jsonString);
    if (error) {
        return false;
    }

    JsonArray array = arrayDoc.as<JsonArray>();
    item *item;
    for(JsonVariant v : array) {
        MeasureType type = (MeasureType) (int) v[type_index_s];
        item = getCacheItemByType(type);
        if (!item) continue;

        item->min = v[min_s];
        item->max = v[max_s];
        item->average = v[average_s];
        item->factor = v[factor_s];
    }

    return true;
}
