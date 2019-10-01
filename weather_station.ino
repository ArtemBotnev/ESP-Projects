
#include "display.h"

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

    display.drawTemperatureMenu(-12, 27);
    delay(SCREEN_DELAY);
}

void readHumidityAndShow() {
    delay(SENSOR_DELAY);
    delay(SENSOR_DELAY);

    display.drawHumidityMenu(34, 50);
    delay(SCREEN_DELAY);
}

void readAtmPressureAndShow() {
    delay(2 * SENSOR_DELAY);

    display.drawAtmPressureMenu(744);
    delay(SCREEN_DELAY);
}
