/*
 * lcd_controller.cpp
 *
 *  Created on: Dec 18, 2024
 *      Author: kohigashi
 */

#include <lcd_controller.h>

extern float supply_voltage;
LCD lcd(&hi2c1);
LCD_Controller lcd_controller;

void format_number_with_separator(uint64_t value, char *output, size_t N) {
  char temp[32];
  int len, i, j, k;

  memset(output, 0, N);
  memset(temp, 0, sizeof(temp));
  // Convert the number to a zero-padded 10-digit string
  snprintf(temp, sizeof(temp), "%09llu", value);
  len = strlen(temp);

  // Add separators
  j = N - 1;          // Start from the end of the output buffer
  output[j--] = '\0'; // Null-terminate the string

  k = 0; // Counter to track digits grouped
  for (i = len - 1; i >= 0; i--) {
    output[j--] = temp[i];
    k++;
    if (k == 3 && i > 0) { // Add separator every 3 digits, but not at the start
      output[j--] = '.';
      k = 0;
    }
  }

  // Shift the result to the start of the output buffer
  memmove(output, &output[j + 1], N - j - 1);
}

void LCD_Controller::begin() { set_screen(SCREEN_MAIN); }

void LCD_Controller::set_screen(LCD_SCREEN screen) {
  lcd.clear();
  if (screen == SCREEN_MAIN) {
    format_number_with_separator(channel_freq.ch1, tempBuf, LCD_ROWS);
    snprintf(lcd.buffer[0], LCD_ROWS, " Ch0:%s Hz", tempBuf);

    format_number_with_separator(channel_freq.ch2, tempBuf, LCD_ROWS);
    snprintf(lcd.buffer[1], LCD_ROWS, " Ch1:%s Hz", tempBuf);

    format_number_with_separator(channel_freq.ch3, tempBuf, LCD_ROWS);
    snprintf(lcd.buffer[2], LCD_ROWS, " Ch2:%s Hz", tempBuf);

    sprintf(lcd.buffer[3],"Battery: %.2fV", supply_voltage);

    for (int i = 0; i < 4; i++) {
      lcd.setCursor(i, 0);
      lcd.sendString(lcd.buffer[i]);
    }
  }

  set_arrow_cursor(0);
  set_arrow_blink(false);
  memcpy(&last_channel_freq, &channel_freq, sizeof(channel_freq));
}

void LCD_Controller::service() {
  if (HAL_GetTick() - blink_tick >= ARROW_BLINK_INTERVAL && cursor_blink) {
    blink_tick = HAL_GetTick();
    cursor_blink_flag =
        !cursor_blink_flag; // Invert the blink flag every interval
    lcd.setCursor(cursor_coordinate[screen][cursor][0],
                  cursor_coordinate[screen][cursor][1]);
    draw_cursor(cursor, cursor_blink_flag);

    lcd.setCursor(digit_row, digit_coordinate[SCREEN_MAIN][digit_col]);
    if (cursor_blink_flag)
      lcd.sendData(
          lcd.buffer[digit_row][digit_coordinate[SCREEN_MAIN][digit_col]]);
    else
      lcd.sendData(' ');
  }
  if (digit_blink)
    lcd.setCursor(digit_row, digit_col);
  if (HAL_GetTick() - refresh_tick < LCD_REFRESH_INTERVAL)
    return;
  refresh_tick = HAL_GetTick();
  if (screen == SCREEN_MAIN) {
    lcd.setCursor(3,0);
    sprintf(lcd.buffer[3],"Battery: %.2fV", supply_voltage);
    lcd.sendString(lcd.buffer[3]);
    if (last_channel_freq.ch1 != channel_freq.ch1 ||
        (force_refresh && refresh_row == 0)) {
      force_refresh = false;
      format_number_with_separator(channel_freq.ch1, tempBuf, LCD_ROWS);
      snprintf(lcd.buffer[0], LCD_ROWS, " Ch0:%s Hz", tempBuf);

      lcd.setCursor(0, 1);
      lcd.sendString(&lcd.buffer[0][1]);
      char tempBuf[32];
      sprintf(tempBuf, "0:%llu\n", channel_freq.ch1);
      CDC_Transmit_FS((uint8_t *)tempBuf, strlen(tempBuf));
      last_channel_freq.ch1 = channel_freq.ch1;
    }
    if (last_channel_freq.ch2 != channel_freq.ch2 ||
        (force_refresh && refresh_row == 1)) {
      force_refresh = false;
      format_number_with_separator(channel_freq.ch2, tempBuf, LCD_ROWS);
      snprintf(lcd.buffer[1], LCD_ROWS, " Ch1:%s Hz", tempBuf);

      lcd.setCursor(1, 1);
      lcd.sendString(&lcd.buffer[1][1]);
      char tempBuf[32];
      sprintf(tempBuf, "1:%llu\n", channel_freq.ch2);
      CDC_Transmit_FS((uint8_t *)tempBuf, strlen(tempBuf));
      last_channel_freq.ch2 = channel_freq.ch2;
    }
    if (last_channel_freq.ch3 != channel_freq.ch3 ||
        (force_refresh && refresh_row == 2)) {
      force_refresh = false;
      format_number_with_separator(channel_freq.ch3, tempBuf, LCD_ROWS);
      snprintf(lcd.buffer[2], LCD_ROWS, " Ch2:%s Hz", tempBuf);

      char tempBuf[32];
      sprintf(tempBuf, "2:%llu\n", channel_freq.ch3);
      CDC_Transmit_FS((uint8_t *)tempBuf, strlen(tempBuf));
      lcd.setCursor(2, 1);
      lcd.sendString(&lcd.buffer[2][1]);
      last_channel_freq.ch3 = channel_freq.ch3;
    }
  }
}

void LCD_Controller::draw_cursor(int8_t position, bool display) {
  if (cursor == position && display)
    lcd.sendData(ARROW_SYMBOL);
  else
    lcd.sendData(' ');
}

void LCD_Controller::set_arrow_cursor(int8_t position) {
  cursor = position;
  blink_tick = HAL_GetTick(); // reset blink millis
  cursor_blink_flag = 1;      // Display the arrow on cursor
  // out of bound handler :
  if (cursor < 0)
    cursor = 0;
  else if (cursor >
           cursor_count[screen] -
               1) // cursor starts at 0 while indexsize count from 1, so size-1
    cursor = cursor_count[screen] - 1;
  //
  // Clear all cursor printed and draw the arrow on new cursor position
  for (int i = 0; i < cursor_count[screen]; i++) {
    lcd.setCursor(cursor_coordinate[screen][i][0],
                  cursor_coordinate[screen][i][1]);
    draw_cursor(i);
  }
}

void LCD_Controller::increment_arrow_cursor() { set_arrow_cursor(cursor + 1); }

void LCD_Controller::decrement_arrow_cursor() { set_arrow_cursor(cursor - 1); }

void LCD_Controller::set_arrow_blink(bool blink) {
  cursor_blink = blink;
  if (!cursor_blink) { // Redraw the arrow after changing cursorBlink flag,
                       // because the arrow might disappear when flag changed
                       // whilst arrow is undisplayed
    lcd.setCursor(cursor_coordinate[screen][cursor][0],
                  cursor_coordinate[screen][cursor][1]);
    draw_cursor(cursor);
  }
}

void LCD_Controller::set_digit_row_col(int8_t row, int8_t col) {
  digit_row = row;
  digit_col = col;
}

void LCD_Controller::set_digit_row(int8_t row) { digit_row = row; }
void LCD_Controller::set_digit_col(int8_t col) { digit_col = col; }

void LCD_Controller::set_digit_blink(bool blink) {
  digit_blink = blink;
  lcd.blink(digit_blink);
}

void LCD_Controller::increment_digit_cursor() {
  digit_col++;
  if (digit_col > 8)
    digit_col = 0;
}

void LCD_Controller::decrement_digit_cursor() {
  digit_col--;
  if (digit_col < 0)
    digit_col = 9;
}
