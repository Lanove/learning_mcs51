#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "stm32f4xx_hal.h"
#include <cstdint>
#include <string>

class LCD {
public:
    // Constructor
    LCD(I2C_HandleTypeDef* i2c_handle, uint8_t lcd_address = 0x4E);

    // Initialization
    void init();

    // Basic commands
    void sendCommand(char cmd);
    void sendData(char data);

    // Control methods
    void display(bool display_on);
    void cursor(bool cursor_on);
    void blink(bool blink_on);

    // Display methods
    void clear();
    void home();

    // Cursor positioning
    void setCursor(uint8_t row, uint8_t col);

    // String output
    void sendString(const char* str);

    char buffer[4][20];
private:
    I2C_HandleTypeDef* i2c; // I2C handle
    uint8_t address;        // LCD I2C address
    uint8_t displayControl; // Display control flags
};

#endif // I2C_LCD_H
