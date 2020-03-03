// Weather station
// Copyright Artem Botnev 2020
// MIT License

#include "repository.h"

DataManager::DataManager() {
    _cache = new Cache();
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
    delete _cache;
    _cache = new Cache();
}

bool DataManager::initStorage() {
    _isStorageAvailable = SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    _fs = &SPIFFS;

    return _isStorageAvailable;
}

void DataManager::setSaveStateFrequency(uint8_t minutes) {
    _saveStateFrequency = minutes;
}

void DataManager::updateTimeData(timePack timePack) {
    _timePack = timePack;
}

DataManager::item *DataManager::getCacheItemByType(MeasureType type) {
    switch (type) {
        case (ROOM_TEMPER):
            return &_cache->roomTemp;
        case (OUT_TEMPER):
            return &_cache->outTemp;
        case (ROOM_HUM):
            return &_cache->roomHum;
        case (OUT_HUM):
            return &_cache->outHum;
        case (PRESSURE):
            return &_cache->pressure;
        default:
            // error
            return NULL;
    }
}

bool DataManager::saveState() {
    if (!_isStorageAvailable) return false;

    File file = _fs->open(stateFile, FILE_WRITE);

    if (!file) return false;

    bool result;

    // serialization;
    const int capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(MEASURE_TYPES_COUNT)
                         + MEASURE_TYPES_COUNT * JSON_OBJECT_SIZE(5);

    StaticJsonDocument<capacity> doc;
    doc[epoch_time_s] = _timePack.epochSeconds;
    doc[date_s] = _timePack.date;
    doc[time_s] = _timePack.time;

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

    if (serializeJson(doc, file) == 0) {
        result = false;
    } else {
        result = true;
    }

    file.close();

    return result;
}
