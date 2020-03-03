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

class DataManager {

public:
    DataManager();

    measureSet<int16_t> getMeasureSet(MeasureType type, int16_t currentVal);

    void clearCache();

    /**
     * inits SPIFFS storage
     * @return true if storage was successfully inited, otherwise false
     */
    bool initStorage();

    /**
     * changes default frequency of cache state
     * can be used optionally because default value exists.
     *
     * @param minutes interval of savings
     */
    void setSaveStateFrequency(uint8_t minutes);

    /**
     * sets current time data, should be updated each iteration of loop function
     * @param timePack
     */
    void updateTimeData(timePack timePack);

    //TODO: test should be replaced to private
    bool saveState();

private:
    // files:
    const char *stateFile = "/saved_state.json";
    // state:
    const char *epoch_time_s = "epoch_seconds";
    const char *date_s = "date";
    const char *time_s = "time";
    const char *measurements_s = "measurements";
    const char *type_index_s = "type_index";
    const char *min_s = "min";
    const char *max_s = "max";
    const char *average_s = "average";
    const char *factor_s = "factor";
    const char *empty = "empty";
    // errors:
    char *errorReadingCard = "Cannot read storage!";
    char *errorFileOpening = "Error of file opening!";
    char *errorJsonDeserialization = "Error of json deserialization! - ";

    // must be not less than 1 and not more than 59 minutes
    uint8_t _saveStateFrequency = 10; // 10 minutes interval by default
    // current time data, should be updated each iteration of loop function
    timePack _timePack = timePack { 0, empty, empty, empty };
    bool _isStorageAvailable;
    fs::FS *_fs;

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

    Cache *_cache;

    item *getCacheItemByType(MeasureType type);

//    /**
//     * writes to file with @path according to @fileWriteAction algorithm
//     * @param path
//     * @param fileWriteAction
//     * @return  true if all work as expected, otherwise false
//     */
//    bool writeToFile(const char path, bool (*fileWriteAction)(File));
//
//    /**
//     * creates json string from cached data and stores it in file
//     * @param file - pointer to file
//     * @return true if all work as expected, otherwise false
//     */
//    bool stateToFile(File *file);

//    /**
//     * reads state from json string and sets it to cache
//     * @param json string
//     * @return true if all work as expected, otherwise false
//     */
//    bool stateFromJson(const char *json);
};

#endif //STORAGE_REPOSITORY_H
