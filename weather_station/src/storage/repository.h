// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef STORAGE_REPOSITORY_H
#define STORAGE_REPOSITORY_H

#include "../common.h"

class DataManager {

public:
    DataManager();

    measureSet<int16_t> getMeasureSet(MeasureType type, int16_t currentVal);

    void clearCache();

private:
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
