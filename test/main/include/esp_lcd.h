/**
 * @file esp_lcd.h
 * @author Jesus Minjares (https://github.com/jminjares4)
 * @brief Liquid Crystal Display header file
 * @version 0.1
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _ESP_LCD_H_
#define _ESP_LCD_H_

#include "driver/gpio.h"
#include <stdint.h>

#define LCD_DATA            0   /*!< LCD data */
#define LCD_CMD             1   /*!< LCD command */
#define LCD_DATA_LINE       4   /*!< 4-Bit data line */

typedef struct{
    uint32_t data[LCD_DATA_LINE];   /*!< LCD data line */
    uint32_t enable;                /*!< LCD enable pin */
    uint32_t regSel;                /*!< LCD register select */
}lcd_t;

void lcd_default(lcd_t *lcd);

void lcd_ctor(lcd_t *lcd, uint32_t data[LCD_DATA_LINE], uint32_t enable, uint32_t regSel);

void lcdSetText(lcd_t * const lcd, char * text, uint8_t x, uint8_t y);

void lcdSetInt(lcd_t * const lcd, int val, uint8_t x, uint8_t y);

void lcdClear(lcd_t * const lcd);

#endif