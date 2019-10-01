//
// Created by Artem Botnev on 04/01/19.
//

/**
 * For TFT display 2.8’
 * renders measured parameters' menu
 *
 * Display pin			  ESP32 board pin
 * Vcc					  Vcc
 * GND					  GND
 * CS					  D15
 * RESET				  D2
 * D/C					  D4
 * SDI(MOSI)			  D23
 * SCK					  D18
 * LED					  Vcc
 * SDO(MISO)              D19
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

extern "C" {
    #include "colors.h"
};

// esp32 pins
#define TFT_DC 4
#define TFT_CS 15
#define TFT_RST 2
#define TFT_MISO 19         
#define TFT_MOSI 23           
#define TFT_CLK 18 

// texts size
#define TITLE_TEXT_SIZE 1
#define VALUE_TEXT_SIZE 5

// titles
#define OUT_TEMPER_TITLE    "Outdoors t, C"
#define ROOM_TEMPER_TITLE   "Room t, C"
#define OUT_HUM_TITLE       "Outdoors h, %"
#define ROOM_HUM_TITLE      "Room h, %"
#define PRESSURE_TITLE      "Atm.press. p, mmHg"

class Display {

public:
    Display();

    void begin();

    void drawTemperatureMenu(int8_t outTemperature, int8_t roomTemperature);

    void drawHumidityMenu(uint8_t outHumidity, uint8_t roomHumidity);

    void drawAtmPressureMenu(uint16_t pressure);

private:
    void drawTopMenu(int8_t value, const char *header, uint16_t (*value_color)(int8_t));

    void drawBottomMenu(int8_t value, const char *header, uint16_t (*value_color)(int8_t));
};

#endif
