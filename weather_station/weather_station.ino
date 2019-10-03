
#include "src/display/display.h"

#define SENSOR_DELAY 100
#define SCREEN_DELAY 2800

Display display;

void setup() {
    display.begin();
}

void loop() {
    readTemperatureAndShow();
    readHumidityAndShow();
    readAtmPressureAndShow();
}

void readTemperatureAndShow() {
    delay(SENSOR_DELAY);
    delay(SENSOR_DELAY);

    struct MeasureSet outT = { -12, -20, -14, -7 };
    struct MeasureSet roomT = { 27, 22, 25, 28 };

    display.drawTemperatureMenu(outT, roomT);
    delay(SCREEN_DELAY);
}

void readHumidityAndShow() {
    delay(SENSOR_DELAY);
    delay(SENSOR_DELAY);

    struct MeasureSet outH = { 34, 30, 33, 36 };
    struct MeasureSet roomH = { 50, 48, 50, 51 };

    display.drawHumidityMenu(outH, roomH);
    delay(SCREEN_DELAY);
}

void readAtmPressureAndShow() {
    delay(2 * SENSOR_DELAY);

    struct MeasureSet pressure = { 744, 740, 743, 750 };
    display.drawAtmPressureMenu(pressure);
    delay(SCREEN_DELAY);
}
