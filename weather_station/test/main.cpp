#define OUT_TEMPER_TITLE    "Outdoors t, C"
#define ROOM_TEMPER_TITLE   "Room t, C"
#define OUT_HUM_TITLE       "Outdoors h, %"
#define ROOM_HUM_TITLE      "Room h, %"
#define PRESSURE_TITLE      "Atm.press. p, mmHg"

#include <iostream>

uint8_t getStartXPosition(const char *str, uint8_t textSize) {
    uint8_t charCount = 0;
    while (str[charCount] != 0x00) charCount++;

    uint16_t stringSize = (charCount << 3) * textSize;
    if (stringSize > 320)
        return 2;

    return (320 - stringSize) >> 2;
}

int main() {
    std::cout << "value: " << (int) getStartXPosition(OUT_TEMPER_TITLE, 2) <<  std::endl;
    std::cout << "value: " << (int) getStartXPosition(ROOM_TEMPER_TITLE, 2) <<  std::endl;
    std::cout << "value: " << (int) getStartXPosition(OUT_HUM_TITLE, 2) <<  std::endl;
    std::cout << "value: " << (int) getStartXPosition(ROOM_HUM_TITLE, 2) <<  std::endl;
    std::cout << "value: " << (int) getStartXPosition(PRESSURE_TITLE, 2) <<  std::endl;

    return 0;
}