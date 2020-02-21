// Weather station
// Copyright Artem Botnev 2020
// MIT License

#ifndef COMMON_H
#define COMMON_H

#include "inttypes.h"

#define ERROR "Error!"
// count of enum items in MeasureType
#define MEASURE_TYPES_COUNT 5

enum MeasureType { ROOM_TEMPER, OUT_TEMPER, ROOM_HUM, OUT_HUM, PRESSURE };

/**
 * Container for measured parameters
 */
template <typename T> struct measureSet { const T curValue, min, average, max; };

#endif //COMMON_H
