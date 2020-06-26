# Weather station
#### The project based on ESP32 module, builds via Arduino IDE.
#### Main features: registration temperature(outdoors, room), humidity(outdoors, room) and atmosphere pressure.
Three screens change in a loop and show current: Temperature(t, C), Humidity(h, %), Atm. pressure(p, mmHg).
Also, for each measure parameter there're additional data: minimum value, average value, maximum value.
All additional values calculated for the current day, and reset at the beginning of a new day (period from 00:00 to 23:59).

<img src="https://github.com/ArtemBotnev/gifs/blob/master/esp32_weather_station/temp.png" width="184" height="250"> <img src="https://github.com/ArtemBotnev/gifs/blob/master/esp32_weather_station/humidity.png" width="182" height="248"> <img src="https://github.com/ArtemBotnev/gifs/blob/master/esp32_weather_station/ws_pressure.png" width="167" height="248">

- [Display settings](#display-settings)
- [Storage settings](#storage-settings)
- [List of dependencies(libraries)](#list-of-dependencieslibraries)
- [List of modules(components)](#list-of-modulescomponents)
- [Pin connections](#pin-connections)

#### Display settings:
In order to show some string as a title of display, the feature ```showTitle``` of Display class must be enabled, when you can add string as a parameter to ```setTitle``` method.
```c++
display.showTitle = true;
display.setTitle("your title");
```
By default ```showTitle``` is enabled and display shows current time.

In order to show additional data min, average, max value for each measure parameter, the feature ```showAdditionData``` of Display class must be enabled.
```c++
display.showAdditionData = true;
```
By default ```showAdditionData``` is enabled.

#### Storage settings:
It supports the storage of additional data(min, average, max value for each measure parameter) in case of power failure and restoring data at the next controller boot (in the same day).
In order to be able to save data in non-volatile memory of the controller, it is necessary to format it in SPIFFS file system.
For more detail of using and adjustment SPIFFS look at 
[arduino-esp32fs-plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin)
It's not necessary to use storage and format file system. You can skip this just disable storage feature.
```c++ 
#define USE_STORAGE false
```
This constant is ```true``` by default.

*In case of using storage, several points should be noted:*
The variable ```storageIsAvailable``` will be ```true``` if storage works as expected and there're no errors.
You can specify periodic of data backups in minutes.
```c++ 
dataManager.setSaveStateFrequency(20);
```
By default it is 10. Value shout be from 0 to 59 (0 - save every loop iteration).

#### List of dependencies(libraries):
- Core
  - Wire - provided with IDE.
  - SPI - provided with IDE.
- Sensors 
  - [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
  - [Adafruit_BME280](https://github.com/adafruit/Adafruit_BME280_Library)
  - [DFRobot_SHT20](https://github.com/DFRobot/DFRobot_SHT20)
- Time
  - [DS3231](https://github.com/jarzebski/Arduino-DS3231)
- Graphics
  - [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
  - [Adafruit_ILI9341](https://github.com/adafruit/Adafruit_ILI9341)
- Storage
  - FS - provided with ESP32 plugin.
  - SPIFFS - provided with ESP32 plugin.
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
  
#### List of modules(components):
- Core
  - ESP-WROOM-32.
- Sensors
  - Room sensor(temperature, humidity, adm. pressure) - BME/BMP280.
  - Outdoor waterproof sensor(temperature, humidity) SHT-20 or better.
- Time
  - Module with DS3231 chip and battery power supply.
- Display
  - 2.8" 240x320 SPI TFT LCD Serial Port Module, ILI9341 5V/3.3V 2.8 inch LED Display For 5110 Interface 
or similar.

#### Pin connections:
| ESP32 | TFT Display | BME/BMP280 | SHT-20 | DS3231 |
|-------|-------------|------------|--------|--------|
| 3v3   | Vcc         | VIN        | VIN    | Vcc    |
| GND   | GND         | GND        | GND    | GND    |
| VIN   | LED         |            |        |        |
| 2     | RESET       |            |        |        |
| 4     | DC          |            |        |        |
| 15    | CS          |            |        |        |
| 18    | SCK         |            |        |        |
| 19    | SDO(MISO)   |            |        |        |
| 23    | SDI(MOSI)   |            |        |        |
| 32    |             | SCL        | SCL    | SCL    |
| 33    |             | SDA        | SDA    | SDA    |

<img src="https://github.com/ArtemBotnev/gifs/blob/master/esp32_weather_station/image.png">