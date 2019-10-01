//
// Created by Artem Botnev on 04/01/19.
//

#include "display.h"

// TFT_ILI9163C tft(__CS, __DC, __RS);
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

Display::Display() = default;

void Display::begin() { 
    tft.begin();
}

void Display::drawTemperatureMenu(int8_t outTemperature, int8_t roomTemperature) {
    drawTopMenu(outTemperature, OUT_TEMPER_TITLE, get_out_temper_color);
    tft.drawLine(0, 64, 256, 64, WHITE);
    drawBottomMenu(roomTemperature, ROOM_TEMPER_TITLE, get_room_temper_color);
}

void Display::drawHumidityMenu(uint8_t outHumidity, uint8_t roomHumidity) {
    drawTopMenu((int8_t) outHumidity, OUT_HUM_TITLE, get_humidity_color);
    tft.drawLine(0, 64, 128, 64, WHITE);
    drawBottomMenu((int8_t) roomHumidity, ROOM_HUM_TITLE, get_humidity_color);
}

void Display::drawAtmPressureMenu(uint16_t pressure) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(10, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(PRESSURE_TITLE);

    tft.setCursor(20, 48);
    tft.setTextColor(get_atm_press_color(pressure));
    tft.setTextSize(VALUE_TEXT_SIZE);
    tft.print(pressure);
}

void Display::drawTopMenu(int8_t value, const char *header, uint16_t (*value_color)(int8_t)) {
    // tft.fillScreen();
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(23, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(header);

    if (value < 0){
        tft.setCursor(20, 22);
    } else if (value < 10) {
        tft.setCursor(50, 22);
    } else {
        tft.setCursor(35, 22);
    }

    tft.setTextColor(value_color(value));
    tft.setTextSize(VALUE_TEXT_SIZE);
    tft.print(value);
}

void Display::drawBottomMenu(int8_t value, const char *header, uint16_t (*value_color)(int8_t)) {
    tft.setCursor(33, 69);
    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(header);

    if (value < 0){
        tft.setCursor(20, 84);
    } else if (value < 10) {
        tft.setCursor(50, 84);
    } else {
        tft.setCursor(35, 84);
    }

    tft.setTextColor(value_color(value));
    tft.setTextSize(VALUE_TEXT_SIZE);
    tft.print(value);
}
