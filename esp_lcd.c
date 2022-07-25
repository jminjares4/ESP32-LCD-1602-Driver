/**
 * @file esp_lcd.c
 * @author Jesus Minjares (https://github.com/jminjares4)
 * @brief Liquid Crystal Display source file
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "include/esp_lcd.h"
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define GPIO_STATE_LOW    0  /*!< Logic low */
#define GPIO_STATE_HIGH   1  /*!< Logic high */

#define isHigh(x,y) (x & y) /*!< Check bit is HIGH */

static void lownibble(lcd_t * const lcd , unsigned char x){
    // checkf if x is high for every bit
    uint8_t i, bit;
    for(i = 0, bit = 0x08; i < LCD_DATA_LINE; i++)
        isHigh(x, bit >> i) ? gpio_set_level(lcd->data[i], GPIO_STATE_HIGH) 
                            : gpio_set_level(lcd->data[i], GPIO_STATE_LOW);
    
}

static void delay(uint32_t x){
    vTaskDelay(x / portTICK_PERIOD_MS);
}

static void lcdTriggerEN(lcd_t * const lcd) {
    gpio_set_level(lcd->enable, GPIO_STATE_HIGH);
    gpio_set_level(lcd->enable, GPIO_STATE_LOW);
}

static void lcdWriteCmd(lcd_t *const lcd, unsigned char cmd, uint8_t lcd_opt){
    //CMD: 1, DATA: 0
    lcd_opt ? gpio_set_level(lcd->regSel, GPIO_STATE_LOW) : gpio_set_level(lcd->regSel, GPIO_STATE_HIGH);

    // upper bits 
    lownibble(lcd, cmd >> 4);
    lcdTriggerEN(lcd);

    //lower bits 
    lownibble(lcd, cmd);
    lcdTriggerEN(lcd);
    
    //set delay
    lcd_opt ? delay(5) : delay(1);
}

/**
 * @brief LCD default constructor
 * @param lcd    pointer to LCD object
 * @note  This function call will use hardcode pins. If you want
 *        to use custom pins @see lcd_ctor
 * 
 * @return None
 */
void lcd_default(lcd_t *lcd){
    /* Default pins */
    uint32_t data[LCD_DATA_LINE] = { 33, 32, 35, 34 }; /* Data pins */
    uint32_t enable = 39; /* Enable pin */
    uint32_t regSel = 36; /* Register Select pin */

    /* Instantiate lcd object with default pins */
    lcd_ctor(lcd, data, enable, regSel);
}

/**
 * @brief LCD constructor
 * 
 * Detailed description starts here 
 * @param lcd       pointer to LCD object
 * @param data      lcd data array 
 * @param enable    lcd enable
 * @param regSel    register select
 * @return          None
 */
void lcd_ctor(lcd_t *lcd, uint32_t data[LCD_DATA_LINE], uint32_t enable, uint32_t regSel)
{
    delay(100);
    
    int i;
    for(i = 0; i < LCD_DATA_LINE; i++)
        lcd->data[i] = data[i];

    lcd->enable = enable;
    lcd->regSel = regSel;
 
    for(i = 0; i < LCD_DATA_LINE; i++){
        esp_rom_gpio_pad_select_gpio(lcd->data[i]);
        gpio_set_direction(lcd->data[i], GPIO_MODE_OUTPUT);
    }

    esp_rom_gpio_pad_select_gpio(lcd->enable);
    esp_rom_gpio_pad_select_gpio(lcd->regSel);
    gpio_set_direction(lcd->enable, GPIO_MODE_OUTPUT);
    gpio_set_direction(lcd->regSel, GPIO_MODE_OUTPUT);

    //set 0x03 to LCD 
    gpio_set_level(lcd->data[0], GPIO_STATE_HIGH);
    gpio_set_level(lcd->data[1], GPIO_STATE_HIGH);

    // Send 0x03 3 tilcds at 5ms then 100 us
	lcdTriggerEN(lcd); 
	delay(5);
	lcdTriggerEN(lcd);
	delay(5);
	lcdTriggerEN(lcd);
	delay(5);

    //switch to 4-bit mode
    gpio_set_level(lcd->data[0], GPIO_STATE_LOW);
    gpio_set_level(lcd->data[1], GPIO_STATE_LOW);
    lcdTriggerEN(lcd);
    delay(5);

    // Initialize LCD
    lcdWriteCmd(lcd, 0x28, LCD_CMD); // 4-bit, 2 line, 5x8
    lcdWriteCmd(lcd, 0x08, LCD_CMD); // Instruction Flow
    lcdWriteCmd(lcd, 0x01, LCD_CMD); // Clear LCD
    lcdWriteCmd(lcd, 0x06, LCD_CMD); // Auto-Increlcdnt
    lcdWriteCmd(lcd, 0x0C, LCD_CMD); // Display On, No blink

}

/**
 * @brief Set text 
 * 
 * Detailed description starts here
 * @param lcd   pointer to LCD object
 * @param text  string text
 * @param x     location at x-axis
 * @param y     location at y-axis
 * @return      None
 */
void lcdSetText(lcd_t * const lcd, char * text, uint8_t x, uint8_t y){
    uint8_t i;
	if (x < 16) {
		x |= 0x80; // Set LCD for first line write
		switch (y){
		case 1:
			x |= 0x40; // Set LCD for second line write
			break;
		case 2:
			x |= 0x60; // Set LCD for first line write reverse
			break;
		case 3:
			x |= 0x20; // Set LCD for second line write reverse
			break;
		}
		lcdWriteCmd(lcd, x, LCD_CMD);
	}
	i = 0;

	while (text[i] != '\0') {
		lcdWriteCmd(lcd, text[i], LCD_DATA);
		i++;
	}
}

/**
 * @brief Set integer
 * 
 * Detailed description starts here 
 * @param lcd    pointer to LCD object
 * @param val   integer value to be displayed
 * @param x     location at x-axis
 * @param y     location at y-axis
 * @return      None
 */
void lcdSetInt(lcd_t * const lcd, int val, uint8_t x, uint8_t y){
    char buffer[16];
    sprintf(buffer, "%d", val);
    lcdSetText(lcd, buffer, x, y);
}

/**
 * @brief Clear LCD screen
 * Detailed description starts here 
 * @param lcd    pointer to LCD object
 * @return      None
 */
void lcdClear(lcd_t * const lcd){
    lcdWriteCmd(lcd,0x01,LCD_CMD);
}
