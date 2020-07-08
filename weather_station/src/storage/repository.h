// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef STORAGE_REPOSITORY_H
#define STORAGE_REPOSITORY_H

#include "../common.h"
#include "FS.h"
#include "SPIFFS.h"

#include <ArduinoJson.h>

#define FORMAT_SPIFFS_IF_FAILED true

#define ITEMS_ARRAY_CAPACITY JSON_ARRAY_SIZE(MEASURE_TYPES_COUNT) + 5 * JSON_OBJECT_SIZE(5) + 350
#define STATE_OBJECT_CAPACITY JSON_OBJECT_SIZE(4) + ITEMS_ARRAY_CAPACITY

class DataManager {

public:
    DataManager();

    measureSet<int16_t> getMeasureSet(MeasureType type, int16_t currentVal);

    void clearCache();

    /**
     * inits SPIFFS storage
     * @return true if storage was successfully inited, otherwise false
     */
    bool initStorage(timePack pack);

    /**
     * changes default frequency of cache state
     * can be used optionally because default value exists.
     *
     * @param minutes interval of savings, must be from 0 to 59 (0 - save every loop iteration)
     */
    void setSaveStateFrequency(uint8_t minutes);

    /**
     * sets current time data, should be updated each iteration of loop function
     * @param timePack
     */
    void updateTimeData(timePack timePack);

    /**
     * @return true - there are no problem with the storage in current time, otherwise false
     */
    bool isStorageAvailable();

    /**
     * @return array of measureSet where index corresponds to number of MeasureType enum entry
     */
    measureSet<int16_t> *getMeasuresArray();

private:
    // files:
    const char *stateFile = "/saved_state.json";
    // state:
    const char *epoch_time_s = "epoch_seconds";
    const char *day_s = "day";
    const char *date_s = "date";
    const char *time_s = "time";
    const char *measurements_s = "measurements";
    const char *type_index_s = "type_index";
    const char *current_s = "current";
    const char *min_s = "min";
    const char *max_s = "max";
    const char *average_s = "average";
    const char *factor_s = "factor";
    const char *empty = "empty";

    // must be from 0 to 59 (0 - save every loop iteration)
    uint8_t _saveStateFrequency = 10; // 10 minutes interval by default
    uint8_t _lastMinute;
    // current time data, should be updated each iteration of loop function
    timePack _timePack = timePack { 0, 0, 0, empty, empty, empty };
    bool _isStorageAvailable;
    bool _alreadySaved;
    fs::FS *_fs;

    struct item {
        int16_t current;
        int16_t min;
        int16_t max;
        float average;
        uint16_t factor;
    };

    struct Cache {
        // temperature
        item roomTemp { 0, 100, -100, -100.0f, 1 };
        item outTemp { 0, 100, -100, -100.0f, 1 };
        // humidity
        item roomHum { 0, 100, -100, .0f, 1 };
        item outHum { 0, 100, -100, .0f, 1 };
        // pressure
        item pressure { 0, 1000, -100, .0f, 1 };
    };

    Cache *_cache;

    item *getCacheItemByType(MeasureType type);

    void saveStateIfNeeded();

    /**
     * saves current cache state to storage
     *
     * @return true if all work as expected, otherwise false
     */
    bool saveState();

    /**
     * recovers cache state from storage
     *
     * @return true if all work as expected, otherwise false
     */
    bool recoverState();

    /**
     * checks should recover state from storage data or not
     *
     * @param epochSec - epoch time (seconds) got from storage
     * @param day - day of month got from storage
     * @return true of current day is the same as day from storage data, otherwise false
     */
    bool shouldRecoverState(uint32_t epochSec, uint8_t day);

    measureSet<int16_t> itemToMeasureSet(item *item);
};

#endif //STORAGE_REPOSITORY_H
