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
    gpio_num_t data[LCD_DATA_LINE];   /*!< LCD data line */
    gpio_num_t en;                    /*!< LCD enable pin */
    gpio_num_t regSel;                /*!< LCD register select */
}lcd_t;

void lcd_default(lcd_t *lcd);

void lcd_init(lcd_t * const lcd);

void lcd_ctor(lcd_t *lcd, gpio_num_t data[LCD_DATA_LINE], gpio_num_t en, gpio_num_t regSel);

void lcdSetText(lcd_t * const lcd, char * text, int x, int y);

void lcdSetInt(lcd_t * const lcd, int val, int x, int y);

void lcdClear(lcd_t * const lcd);

void lcdTriggerEN(lcd_t * const lcd);

void lcdWriteCmd(lcd_t * const lcd, unsigned char cmd, uint8_t lcd_opt);

#endif