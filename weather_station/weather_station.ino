// Weather station
// Copyright Artem Botnev 2019
// MIT License

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DFRobot_SHT20.h>

#include "src/display/display.h"
#include "src/clock/clock.h"

#define SENSOR_DELAY 100
#define SCREEN_DELAY 2800

#define I2C_SDA 33
#define I2C_SCL 32

Adafruit_BME280 bme;
DFRobot_SHT20 sht20;

Display display;
TClock cl;

void setup() {
    Wire.begin(I2C_SDA, I2C_SCL);
    bme.begin();
    sht20.initSHT20();
    display.begin();
    display.showTitle = true;
    display.showAdditionData = true;
    cl.init();
}

void loop() {
    readTemperatureAndShow();
    readHumidityAndShow();
    readAtmPressureAndShow();
}

void readTemperatureAndShow() {
    int16_t outCurrentTemper = round(sht20.readTemperature());
    delay(SENSOR_DELAY);

    int16_t roomCurrentTemp = round(bme.readTemperature());
    delay(SENSOR_DELAY);

    struct MeasureSet outT = { outCurrentTemper, 0, 00, 0 };
    struct MeasureSet roomT = { roomCurrentTemp, 0, 0, 0 };

    display.setTitle(cl.getTime());
    display.drawTemperatureMenu(outT, roomT);
    delay(SCREEN_DELAY);
}

void readHumidityAndShow() {
    uint16_t outCurrentHumidity = round(sht20.readHumidity());
    delay(SENSOR_DELAY);

    uint16_t roomCurrentHumidity = round(bme.readHumidity());
    delay(SENSOR_DELAY);

    struct MeasureSet outH = { outCurrentHumidity, 0, 0, 0 };
    struct MeasureSet roomH = { roomCurrentHumidity, 0, 0, 0 };

    display.drawHumidityMenu(outH, roomH);
    delay(SCREEN_DELAY);
}

void readAtmPressureAndShow() {
    // mmhg
    uint16_t currentPressure = round(bme.readPressure() * 0.0075F);
    delay(2 * SENSOR_DELAY);

    struct MeasureSet pressure = { currentPressure, 0, 0, 0 };

    display.drawAtmPressureMenu(pressure);
    delay(SCREEN_DELAY);
}
