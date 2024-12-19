#include "LCD.h"
#include "stm32f4xx_hal.h"

// Constructor
LCD::LCD(I2C_HandleTypeDef* i2c_handle, uint8_t lcd_address)
    : i2c(i2c_handle), address(lcd_address), displayControl(0) {}

// Send a command to the LCD
void LCD::sendCommand(char cmd) {
    char data_u = (cmd & 0xF0);
    char data_l = ((cmd << 4) & 0xF0);
    uint8_t data_t[4] = {
        static_cast<uint8_t>(data_u | 0x0C), // en=1, rs=0
        static_cast<uint8_t>(data_u | 0x08), // en=0, rs=0
        static_cast<uint8_t>(data_l | 0x0C), // en=1, rs=0
        static_cast<uint8_t>(data_l | 0x08)  // en=0, rs=0
    };
    HAL_I2C_Master_Transmit(i2c, address, data_t, 4, 100);
    HAL_Delay(1);
}

// Send data to the LCD
void LCD::sendData(char data) {
    char data_u = (data & 0xF0);
    char data_l = ((data << 4) & 0xF0);
    uint8_t data_t[4] = {
        static_cast<uint8_t>(data_u | 0x0D), // en=1, rs=1
        static_cast<uint8_t>(data_u | 0x09), // en=0, rs=1
        static_cast<uint8_t>(data_l | 0x0D), // en=1, rs=1
        static_cast<uint8_t>(data_l | 0x09)  // en=0, rs=1
    };
    HAL_I2C_Master_Transmit(i2c, address, data_t, 4, 100);
    HAL_Delay(1);
}

// Initialize the LCD
void LCD::init() {
    HAL_Delay(50); // Wait for >40ms
    sendCommand(0x30);
    HAL_Delay(5); // Wait for >4.1ms
    sendCommand(0x30);
    HAL_Delay(1); // Wait for >100us
    sendCommand(0x30);
    HAL_Delay(10);
    sendCommand(0x20); // 4-bit mode
    HAL_Delay(10);

    // Display initialization
    sendCommand(0x28); // Function set: 4-bit mode, 2-line display, 5x8 characters
    HAL_Delay(1);
    sendCommand(0x08); // Display off
    HAL_Delay(1);
    sendCommand(0x01); // Clear display
    HAL_Delay(2);
    sendCommand(0x06); // Entry mode set: increment cursor, no shift
    HAL_Delay(1);
    sendCommand(0x0C); // Display on, cursor off, blink off
    HAL_Delay(1);
}

// Turn the display on/off
void LCD::display(bool display_on) {
    if (display_on) {
        displayControl |= 0x04; // LCD_DISPLAYON
    } else {
        displayControl &= ~0x04; // LCD_DISPLAYOFF
    }
    sendCommand(0x08 | displayControl);
    HAL_Delay(1);
}

// Turn the cursor on/off
void LCD::cursor(bool cursor_on) {
    if (cursor_on) {
        displayControl |= 0x02; // LCD_CURSORON
    } else {
        displayControl &= ~0x02; // LCD_CURSOROFF
    }
    sendCommand(0x08 | displayControl);
    HAL_Delay(1);
}

// Turn blinking on/off
void LCD::blink(bool blink_on) {
    if (blink_on) {
        displayControl |= 0x01; // LCD_BLINKON
    } else {
        displayControl &= ~0x01; // LCD_BLINKOFF
    }
    sendCommand(0x08 | displayControl);
    HAL_Delay(1);
}

// Clear the display
void LCD::clear() {
    sendCommand(0x01); // LCD_CLEARDISPLAY
    HAL_Delay(2);
}

// Return cursor to home position
void LCD::home() {
    sendCommand(0x02); // LCD_RETURNHOME
    HAL_Delay(2);
}

// Set cursor position
void LCD::setCursor(uint8_t row, uint8_t col) {
    const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    sendCommand(0x80 | (col + row_offsets[row])); // LCD_SETDDRAMADDR
    HAL_Delay(2);
}

// Send a string to the LCD
void LCD::sendString(const char* str) {
    while (*str) {
        sendData(*str++);
    }
}
