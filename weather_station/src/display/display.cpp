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
    drawMenuCell(outT, OUT_TEMPER_TITLE, TOP_MENU_SHIFT_Y, get_out_temper_color);
    // bottom
    drawMenuCell(roomT, ROOM_TEMPER_TITLE, BOTTOM_MENU_SHIFT_Y, get_out_temper_color);
}

void Display::drawHumidityMenu(MeasureSet outH, MeasureSet roomH) {
    // top
    fillScreen();
    drawMenuCell(outH, OUT_HUM_TITLE, TOP_MENU_SHIFT_Y, get_humidity_color);
    // bottom
    drawMenuCell(roomH, ROOM_HUM_TITLE, BOTTOM_MENU_SHIFT_Y, get_humidity_color);
}

void Display::drawAtmPressureMenu(MeasureSet press) {
    fillScreen();
    tft.setCursor(10, 25);
    drawMenuCell(press, PRESSURE_TITLE, CENTER_MENU_SHIFT_Y, get_atm_press_color);
}

void Display::drawMenuCell(MeasureSet measure, const char *header, uint8_t shiftY, uint16_t (*value_color)(int16_t)) {
    uint8_t x = getStartXPosition(header, TITLE_TEXT_SIZE);
    tft.setCursor(x, 5 + shiftY);

    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(header);

    uint8_t y = 42 + shiftY;
    if (measure.curValue < 0){
        tft.setCursor(30, y);
    } else if (measure.curValue < 10) {
        tft.setCursor(70, y);
    } else {
        tft.setCursor(55, y);
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

uint8_t Display::getStartXPosition(const char *str, uint8_t textSize) {
    uint8_t charCount = 0;
    while (str[charCount] != 0x00) charCount++;

    uint16_t stringSize = (charCount << 3) * textSize;
    if (stringSize > ILI9341_TFTWIDTH)
        return 2;

    return (ILI9341_TFTWIDTH - stringSize) >> 2;
}
