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

    item->current = currentVal;

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

    return measureSet<int16_t> { currentVal, item->min, item->average , item->max };
}

void DataManager::clearCache() {
    delete _cache;
    _cache = new Cache();
}

bool DataManager::initStorage(timePack pack) {
    _timePack = pack;

    _isStorageAvailable = SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    if (!_isStorageAvailable) return false;

    _fs = &SPIFFS;
    recoverState();

    return _isStorageAvailable;
}

void DataManager::setSaveStateFrequency(uint8_t minutes) {
    _saveStateFrequency = minutes % 60;
}

void DataManager::updateTimeData(timePack timePack) {
    _timePack = timePack;
    saveStateIfNeeded();
}

bool DataManager::isStorageAvailable() {
    return _isStorageAvailable;
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

void DataManager::saveStateIfNeeded() {
    if (!_isStorageAvailable) return;

    if(_saveStateFrequency == 0) {
        _isStorageAvailable = saveState();
        return;
    }

    if(_saveStateFrequency == 1 && _timePack.minute != _lastMinute) {
        _isStorageAvailable = saveState();
        _lastMinute = _timePack.minute;
        return;
    }

    if (_timePack.minute % _saveStateFrequency != 0) {
        _alreadySaved = false;
        return;
    }

    if (!_alreadySaved) {
        _isStorageAvailable = saveState();
        _alreadySaved = true;
    }
}

bool DataManager::saveState() {
    if (!_isStorageAvailable) return false;

    File file = _fs->open(stateFile, FILE_WRITE);

    if (!file) return false;

    // serialization;
    StaticJsonDocument<STATE_OBJECT_CAPACITY> doc;
    doc[epoch_time_s] = _timePack.epochSeconds;
    doc[day_s] = _timePack.day;
    doc[date_s] = _timePack.date;
    doc[time_s] = _timePack.time;

    StaticJsonDocument<ITEMS_ARRAY_CAPACITY> arrayDoc;
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

    bool result = serializeJson(doc, file);
    file.close();

    return result;
}

bool DataManager::recoverState() {
    if (!_isStorageAvailable) return false;

    File file = _fs->open(stateFile);

    if (!file || file.isDirectory()) return false;

    bool result;

    // deserialization;
    StaticJsonDocument<STATE_OBJECT_CAPACITY + ITEMS_ARRAY_CAPACITY> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        return false;
    }

    uint32_t epochTimeSeconds = doc[epoch_time_s];
    uint8_t day = doc[day_s];

    // check if should recover cache state or not
    if (!shouldRecoverState(epochTimeSeconds, day)) return true;

    StaticJsonDocument<ITEMS_ARRAY_CAPACITY> arrayDoc;
    arrayDoc = doc[measurements_s];
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

bool DataManager::shouldRecoverState(uint32_t epochSec, uint8_t day) {
    uint32_t delta = _timePack.epochSeconds - epochSec;
    bool isTheSameDay = _timePack.day == day;

    return delta < 24 * 60 * 60 && isTheSameDay;
}

measureSet<int16_t> *DataManager::getMeasuresArray() {
    item *roomTemper = getCacheItemByType(ROOM_TEMPER);
    item *outTemper = getCacheItemByType(OUT_TEMPER);
    item *roomHum = getCacheItemByType(ROOM_HUM);
    item *outHum = getCacheItemByType(OUT_HUM);
    item *pressure = getCacheItemByType(PRESSURE);

    static measureSet<int16_t> result[] {
            itemToMeasureSet(roomTemper),
            itemToMeasureSet(outTemper),
            itemToMeasureSet(roomHum),
            itemToMeasureSet(outHum),
            itemToMeasureSet(pressure),
    };

    return result;
}

measureSet<int16_t> DataManager::itemToMeasureSet(item *item) {
    return measureSet<int16_t> { item->current, item->min, item->average, item->max };
}