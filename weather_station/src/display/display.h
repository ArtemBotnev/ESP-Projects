// Weather station
// Copyright Artem Botnev 2019
// MIT License

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

#include "../common.h"

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
#define SMALL_TEXT 1
#define TITLE_TEXT_SIZE 2
#define VALUE_TEXT_SIZE 10

// Y shifts
#define TOP_MENU_SHIFT_Y 23
#define BOTTOM_MENU_SHIFT_Y 161
#define CENTER_MENU_SHIFT_Y 95

// titles
#define OUT_TEMPER_TITLE    "Outdoors t, C"
#define ROOM_TEMPER_TITLE   "Room t, C"
#define OUT_HUM_TITLE       "Outdoors h, %"
#define ROOM_HUM_TITLE      "Room h, %"
#define PRESSURE_TITLE      "Atm.press. p, mmHg"

// screen rotation
#define ROTATE_180 2

#define AVERAGE "average"
#define MAX "max"
#define MIN "min"


class Display {

public:
    bool showTitle, showAdditionData;

    Display();

    void begin();

    void setScreenColor(uint16_t color);

    void fillScreen();

    void setTitle(const char *title);

    void drawTemperatureMenu(measureSet<int16_t> outTemperature, measureSet<int16_t> roomTemperature);

    void drawHumidityMenu(measureSet<int16_t> outHumidity, measureSet<int16_t> roomHumidity);

    void drawAtmPressureMenu(measureSet<int16_t> pressure);

private:
    uint16_t _screenColor = ILI9341_BLACK;
    char *_title;

    void drawHeadMenu(const char *title);

    void drawBottom();

    void drawMenuCell(
            measureSet<int16_t> measure,
            const char *header,
            uint8_t x,
            uint8_t y,
            uint16_t (*value_color)(int16_t));

    void drawAdditionalData(uint8_t shiftY, measureSet<int16_t> measure, uint16_t (*value_color)(int16_t));

    int16_t round(float value);
};

#endif
