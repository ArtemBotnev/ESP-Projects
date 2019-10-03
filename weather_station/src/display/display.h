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
#define SMALL_TEXT 1
#define TITLE_TEXT_SIZE 2
#define VALUE_TEXT_SIZE 10

// Y shifts
#define TOP_MENU_SHIFT_Y 10
#define BOTTOM_MENU_SHIFT_Y 148
#define CENTER_MENU_SHIFT_Y 88

// titles
#define OUT_TEMPER_TITLE    "Outdoors t, C"
#define ROOM_TEMPER_TITLE   "Room t, C"
#define OUT_HUM_TITLE       "Outdoors h, %"
#define ROOM_HUM_TITLE      "Room h, %"
#define PRESSURE_TITLE      "Atm.press. p, mmHg"

#define AVERAGE "average"
#define MAX "max"
#define MIN "min"

/**
 * Container for measured parameters
 */
struct MeasureSet {
    const int16_t curValue, min, average, max;
};

class Display {

public:
    bool showAdditionData;

    Display();

    void begin();

    void setScreenColor(uint16_t color);

    void fillScreen();

    void drawTemperatureMenu(MeasureSet outTemperature, MeasureSet roomTemperature);

    void drawHumidityMenu(MeasureSet outHumidity, MeasureSet roomHumidity);

    void drawAtmPressureMenu(MeasureSet pressure);

private:
    uint16_t _screenColor = ILI9341_BLACK;

    void drawHeadMenu(const char *title);

    void drawMenuCell(MeasureSet measure, const char *header, uint8_t shiftY, uint16_t (*value_color)(int16_t));

    uint8_t  getStartXPosition(const char *str, uint8_t textSize);
};

#endif
