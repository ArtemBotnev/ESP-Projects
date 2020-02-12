// Weather station
// Copyright Artem Botnev 2020
// MIT License

#include "repository.h"

DataManager::DataManager() {
    cache = new Cache();
}

measureSet<int16_t> DataManager::getMeasureSet(MeasureType type, int16_t currentVal) {
    item *item;

    switch (type) {
        case (ROOM_TEMPER):
            item = &cache->roomTemp;
            break;
        case (OUT_TEMPER):
            item = &cache->outTemp;
            break;
        case (ROOM_HUM):
            item = &cache->roomHum;
            break;
        case (OUT_HUM):
            item = &cache->outHum;
            break;
        case (PRESSURE):
            item = &cache->pressure;
            break;
        default:
            // error
            return measureSet<int16_t>{ -1000, -1000, -1000, -1000 };
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
