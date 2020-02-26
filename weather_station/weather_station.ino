// Weather station
// Copyright Artem Botnev 2019-2020
// MIT License

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DFRobot_SHT20.h>

#include "src/common.h"
#include "src/display/display.h"
#include "src/clock/clock.h"
#include "src/storage/repository.h"

#define SENSOR_DELAY 100
#define SCREEN_DELAY 2800

#define I2C_SDA 33
#define I2C_SCL 32

Adafruit_BME280 bme;
DFRobot_SHT20 sht20;

Display display;
TClock cl;
DataManager dataManager;

//bool propertiesHasRead;

void setup() {
    Wire.begin(I2C_SDA, I2C_SCL);
    bme.begin();
    sht20.initSHT20();

//    Serial.begin(115200);
//    String s = dataManager.initExternalStorage();
//    Serial.println(s);
    // must be not less than 1 and not more than 59 minutes
//    dataManager.setSaveStateFrequency(3);

    display.begin();
    display.showTitle = true;
    display.showAdditionData = true;

    cl.init();
}

void loop() {
    readTemperatureAndShow();
    readHumidityAndShow();
    readAtmPressureAndShow();

    if (dataManager.useExternalStorage && dataManager.isCardAvailable()) {
        TClock::clockDataContainer c = cl.getClockData();
//        dataManager.setTimeAndCheckStateSaveAction(c.secondsStamp, c.minuteOfHour, c.date, c.time);
        dataManager.setTimeAndCheckStateSaveAction(100000, 3, "26.0.2020", "13:37");
        // TODO: clear cache each new day!!!
    }
}

void readTemperatureAndShow() {
    int16_t outCurrentTemper = round(sht20.readTemperature());
    delay(SENSOR_DELAY);

    int16_t roomCurrentTemper = round(bme.readTemperature());
    delay(SENSOR_DELAY);

    measureSet<int16_t> outT = dataManager.getMeasureSet(OUT_TEMPER, outCurrentTemper);
    measureSet<int16_t> roomT = dataManager.getMeasureSet(ROOM_TEMPER, roomCurrentTemper);

    display.setTitle(cl.getTimeString());
    display.drawTemperatureMenu(outT, roomT);
    delay(SCREEN_DELAY);
}

void readHumidityAndShow() {
    uint16_t outCurrentHumidity = round(sht20.readHumidity());
    delay(SENSOR_DELAY);

    uint16_t roomCurrentHumidity = round(bme.readHumidity());
    delay(SENSOR_DELAY);

    measureSet<int16_t> outH = dataManager.getMeasureSet(OUT_HUM, outCurrentHumidity);
    measureSet<int16_t> roomH = dataManager.getMeasureSet(ROOM_HUM, roomCurrentHumidity);

    display.drawHumidityMenu(outH, roomH);
    delay(SCREEN_DELAY);
}

void readAtmPressureAndShow() {
    // mmhg
    uint16_t currentPressure = round(bme.readPressure() * 0.0075F);
    delay(2 * SENSOR_DELAY);

    measureSet<int16_t> pressure = dataManager.getMeasureSet(PRESSURE, currentPressure);

    display.drawAtmPressureMenu(pressure);
    delay(SCREEN_DELAY);
}
