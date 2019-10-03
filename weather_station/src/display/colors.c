//
// Created by Artem Botnev on 04/01/19.
//

#include "colors.h"

uint16_t get_out_temper_color(int16_t temperature)
{
    if (temperature < -17) {
        return BLUE;
    } else if (temperature < -7) {
        return LIGHT_BLUE;
    } else if (temperature < 2) {
        return WHITE;
    } else if (temperature < 12) {
        return YELLOW;
    } else if (temperature < 25) {
        return GREEN;
    } else {
        return RED;
    }
}

uint16_t get_room_temper_color(int16_t temperature)
{
    if (temperature < 17){
        return BLUE;
    } else if (temperature > 27){
        return RED;
    } else {
        return GREEN;
    }
}

uint16_t get_atm_press_color(int16_t pressure)
{
    if (pressure < 710 || pressure > 790){
        return RED;
    } else if (pressure < 730 || pressure > 770){
        return YELLOW;
    } else {
        return GREEN;
    }
}

uint16_t get_humidity_color(int16_t humidity)
{
    if (humidity < 28 || humidity > 72) {
        return RED;
    } else if (humidity < 40 || humidity > 60) {
        return YELLOW;
    } else {
        return GREEN;
    }
}
