/*
 * lcd_controller.h
 *
 *  Created on: Dec 18, 2024
 *      Author: kohigashi
 */

#ifndef INC_LCD_CONTROLLER_H_
#define INC_LCD_CONTROLLER_H_

#include <LCD.h>
#include "data_bindings.hpp"
#include "string.h"
#include <stdio.h>
#include "i2c.h"
#include "usbd_cdc_if.h"

#define LCD_REFRESH_INTERVAL 200 // LCD refresh rate in ms
#define LCDI2C_ADDRESS 0x27
#define LCD_ROWS 20
#define LCD_COLS 4

#define ARROW_BLINK_INTERVAL 500
#define ARROW_SYMBOL (char)0x7E
#define DEGREE_SYMBOL (char)223
#define BLOCK_SYMBOL (char)0xFF

#define LCD_SCREEN_COUNT 1
#define LCD_HIGHEST_CURSOR_COUNT 3
#define LCD_HIGHTEST_DIGIT_COUNT 9

extern LCD lcd;

typedef enum
{
    SCREEN_MAIN,
} LCD_SCREEN;

class LCD_Controller
{
private:
    uint32_t refresh_tick;  // Variable to store the milis for lcd refresh
    uint32_t blink_tick;    // Variable to store the millis for cursor blink
    LCD_SCREEN screen;       // Variable to store the lcd screen position
    int8_t cursor;           // Variable to store the cursor position
    bool cursor_blink;        // Variable to store whether cursor should be blinked or not
    bool cursor_blink_flag;    // Variable to store the display condition of arrow (1 == arrow displayed), used to blink the arrow of cursor

    const int8_t cursor_coordinate[LCD_SCREEN_COUNT][LCD_HIGHEST_CURSOR_COUNT][2] = { // Variable to store the coordinate of each cursor position on each screen
        {                                                                            // SCREEN_MAIN
         {0, 0},
         {1, 0},
         {2, 0}
        }
    };
	const int8_t cursor_count[LCD_SCREEN_COUNT] = {3}; // Variable to store maximum cursor count of each screen

    const int8_t digit_coordinate[LCD_SCREEN_COUNT][LCD_HIGHTEST_DIGIT_COUNT] = {    // Variable to store the coordinate of each digit position on each screen
        {
            15,14,13,11,10,9,7,6,5
        }
    };

    // Variables to keep last value of x to be used on lcd refresh (only display the changed value on LCD)
    bool last_cursor_blink;
    int last_cursor;

    bool digit_blink;
    bool digit_blink_flag;
    int8_t digit_cursor;
    int8_t digit_row;
    int8_t digit_col;

    bool last_digit_blink;
    int last_digit_cursor;

    void draw_cursor(int8_t position, bool display = true);

    bool force_refresh;
    int refresh_row;

public:
    char tempBuf[32];
    void begin();
    void service();

    void set_screen(LCD_SCREEN screen_);
    LCD_SCREEN get_screen() { return screen; }

    void increment_arrow_cursor();
    void decrement_arrow_cursor();
    void set_arrow_cursor(int8_t position);
    int get_arrow_cursor() { return cursor; }
    void set_arrow_blink(bool blink);
    bool get_arrow_blink() { return cursor_blink;}

    void set_digit_row(int8_t row);
    void set_digit_col(int8_t col);
    void increment_digit_cursor();
    void decrement_digit_cursor();
    void set_digit_cursor(int8_t position);
    int get_digit_cursor() { return digit_cursor; }
    void set_digit_blink(bool blink);
    bool get_digit_blink() { return digit_blink; }
    void set_digit_row_col(int8_t row, int8_t col);
    int get_digit_row() { return digit_row; }
    int get_digit_col() { return digit_col; }
    void force_refresh_row(int row){
        force_refresh = true;
        refresh_row = row;
    }

    Si5315_Channel_Frequency last_channel_freq;
};

extern LCD_Controller lcd_controller;


#endif /* INC_LCD_CONTROLLER_H_ */
