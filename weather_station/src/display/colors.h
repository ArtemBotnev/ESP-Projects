// Weather station
// Copyright Artem Botnev 2019
// MIT License

/**
 * Colors for display
 * color is selecting according the value of the measured parameter
 */

#ifndef COLORS_H
#define COLORS_H

#include "inttypes.h"

// RGB565
//#define BLACK         0x0000
#define BLUE          0x001F
#define LIGHT_BLUE    0x64D9
#define RED           0xF800
#define GREEN         0x07E0
//#define CYAN          0x07FF
//#define MAGENTA       0xF81F
#define YELLOW        0xFFE0  
#define WHITE         0xFFFF

uint16_t get_out_temper_color(int16_t temperature);

uint16_t get_room_temper_color(int16_t temperature);

uint16_t get_atm_press_color(int16_t pressure);

uint16_t get_humidity_color(int16_t humidity);

#endif
