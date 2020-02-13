// Weather station
// Copyright Artem Botnev 2019
// MIT License

#include "display.h"

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

Display::Display() = default;

void Display::begin() { 
    tft.begin();
    // rotate screen on 180
    tft.setRotation(ROTATE_180);
}

void Display::setScreenColor(uint16_t color) {
    _screenColor = color;
}

void Display::fillScreen() {
    tft.fillScreen(_screenColor);
}

void Display::setTitle(const char *title) {
    _title = (char*) title;
}

void Display::drawTemperatureMenu(measureSet<int16_t> outT, measureSet<int16_t> roomT) {
    fillScreen();
    // header
    if (showTitle) drawHeadMenu(_title);
    // top
    drawMenuCell(outT, OUT_TEMPER_TITLE, 43, TOP_MENU_SHIFT_Y, get_out_temper_color);
    // bottom
    drawMenuCell(roomT, ROOM_TEMPER_TITLE, 68, BOTTOM_MENU_SHIFT_Y, get_room_temper_color);

    if (showAdditionData) drawBottom();
}

void Display::drawHumidityMenu(measureSet<int16_t> outH, measureSet<int16_t> roomH) {
    fillScreen();
    // header
    if (showTitle) drawHeadMenu(_title);
    // top
    drawMenuCell(outH, OUT_HUM_TITLE, 45, TOP_MENU_SHIFT_Y, get_humidity_color);
    // bottom
    drawMenuCell(roomH, ROOM_HUM_TITLE, 70, BOTTOM_MENU_SHIFT_Y, get_humidity_color);

    if (showAdditionData) drawBottom();
}

void Display::drawAtmPressureMenu(measureSet<int16_t> press) {
    fillScreen();
    if (showTitle) drawHeadMenu(_title);
    tft.setCursor(10, 25);
    drawMenuCell(press, PRESSURE_TITLE, 10, CENTER_MENU_SHIFT_Y, get_atm_press_color);

    if (showAdditionData) drawBottom();
}

void Display::drawMenuCell(
        measureSet<int16_t> measure,
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

    if (showAdditionData) drawAdditionalData(shiftY, measure, value_color);
}

void Display::drawAdditionalData(uint8_t shiftY, measureSet<int16_t> measure, uint16_t (*value_color)(int16_t)) {
    uint16_t yb = 120 + shiftY;
    // min value
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.setCursor(20, yb);
    tft.setTextColor(value_color(measure.min));
    tft.print(measure.min);
    // average value
    tft.setCursor(110, yb);
    tft.setTextColor(value_color(measure.average));
    tft.print(measure.average);
    // max value
    tft.setCursor(200, yb);
    tft.setTextColor(value_color(measure.max));
    tft.print(measure.max);
}

void Display::drawHeadMenu(const char *title) {
    tft.setCursor(25, 7);
    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);
    tft.println(title);
}

void Display::drawBottom() {
    tft.setTextColor(WHITE);
    tft.setTextSize(TITLE_TEXT_SIZE);

    tft.setCursor(12, 300);
    tft.print(MIN);
    tft.setCursor(78, 300);
    tft.print(AVERAGE);
    tft.setCursor(192, 300);
    tft.print(MAX);
}
