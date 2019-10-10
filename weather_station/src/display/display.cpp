//
// Created by Artem Botnev on 10/01/19.
//

#include "display.h"

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

Display::Display() = default;

void Display::begin() { 
    tft.begin();
}

void Display::setScreenColor(uint16_t color) {
    _screenColor = color;
}

void Display::fillScreen() {
    tft.fillScreen(_screenColor);
}

void Display::drawTemperatureMenu(MeasureSet outT, MeasureSet roomT) {
    fillScreen();
    // top
    drawMenuCell(outT, OUT_TEMPER_TITLE, 43, TOP_MENU_SHIFT_Y, get_out_temper_color);
    // bottom
    drawMenuCell(roomT, ROOM_TEMPER_TITLE, 68, BOTTOM_MENU_SHIFT_Y, get_out_temper_color);
}

void Display::drawHumidityMenu(MeasureSet outH, MeasureSet roomH) {
    // top
    fillScreen();
    drawMenuCell(outH, OUT_HUM_TITLE, 45, TOP_MENU_SHIFT_Y, get_humidity_color);
    // bottom
    drawMenuCell(roomH, ROOM_HUM_TITLE, 70, BOTTOM_MENU_SHIFT_Y, get_humidity_color);
}

void Display::drawAtmPressureMenu(MeasureSet press) {
    fillScreen();
    tft.setCursor(10, 25);
    drawMenuCell(press, PRESSURE_TITLE, 10, CENTER_MENU_SHIFT_Y, get_atm_press_color);
}

void Display::drawMenuCell(
        MeasureSet measure,
        const char *header,
        uint8_t shiftX,
        uint8_t shiftY,
        uint16_t (*value_color)(int16_t)) {

    tft.setCursor(shiftX, 5 + shiftY);

    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(header);

    uint8_t y = 42 + shiftY;
    if (measure.curValue < -9){
        tft.setCursor(35, y);
    } else if (measure.curValue < 0) {
        tft.setCursor(60, y);
    } else if (measure.curValue < 10) {
        tft.setCursor(95, y);
    } else if (measure.curValue < 100) {
        tft.setCursor(68, y);
    } else {
        tft.setCursor(35, y);
    }

    // current value
    tft.setTextColor(value_color(measure.curValue));
    tft.setTextSize(VALUE_TEXT_SIZE);
    tft.print(measure.curValue);
    uint8_t yb = 120 + shiftY;

    if (!showAdditionData)
        return;

    // min value
    tft.setCursor(20, yb);
    tft.setTextColor(value_color(measure.min));
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.print(measure.min);
    // average value
    tft.setCursor(120, yb);
    tft.setTextColor(value_color(measure.average));
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.print(measure.average);
    // max value
    tft.setCursor(200, yb);
    tft.setTextColor(value_color(measure.max));
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.print(measure.max);
}

void Display::drawHeadMenu(const char *title) {
    tft.setCursor(5, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(SMALL_TEXT);
    tft.println(title);
}
