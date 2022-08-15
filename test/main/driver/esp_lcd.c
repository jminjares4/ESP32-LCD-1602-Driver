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

#include "esp_lcd.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_STATE_LOW    0  /*!< Logic low */
#define GPIO_STATE_HIGH   1  /*!< Logic high */

void lcdTriggerEN(lcd_t * const lcd) {
    gpio_set_level( lcd->en, GPIO_STATE_LOW);
    vTaskDelay( 1 / portTICK_PERIOD_MS );
    gpio_set_level( lcd->en, GPIO_STATE_HIGH);
    vTaskDelay( 1 / portTICK_PERIOD_MS );
    gpio_set_level( lcd->en, GPIO_STATE_LOW);
    vTaskDelay( 1 / portTICK_PERIOD_MS );
}

void lownibble(lcd_t * const lcd , unsigned char x){
    // checkf if x is high for every bit
    uint8_t i, val = 0x01;
    for(i = 0; i < LCD_DATA_LINE; i++){
       gpio_set_level(lcd->data[i], (x >> i) & val);
    //    (x & (val >> i)) ? gpio_set_level( lcd->data[LCD_DATA_LINE - i ], GPIO_STATE_HIGH) : gpio_set_level( lcd->data[LCD_DATA_LINE - i], GPIO_STATE_LOW);
        // isHigh(x, bit >> i) ? gpio_set_level( lcd->data[i], GPIO_STATE_HIGH) : gpio_set_level( lcd->data[i], GPIO_STATE_LOW);
    }
}

void lcdWriteCmd(lcd_t * const lcd, unsigned char cmd, uint8_t lcd_opt){
    //CMD: 1, DATA: 0
    (lcd_opt == LCD_CMD) ? gpio_set_level( lcd->regSel, GPIO_STATE_LOW) : gpio_set_level( lcd->regSel, GPIO_STATE_HIGH);

    // upper bits 
    lownibble(lcd, cmd >> 4);
    lcdTriggerEN(lcd);

    //lower bits 
    lownibble(lcd, cmd);
    lcdTriggerEN(lcd);
    
    //set delay
    // lcd_opt ? delay(10) : delay(1);
    if(lcd_opt == LCD_CMD)
        vTaskDelay( 10 / portTICK_PERIOD_MS);

}

/**
 * @brief Initialize LCD object
 * 
 * @param lcd   pointer to LCD object
 * @note  Must constructor LCD object. @see lcd_ctor() and @see lcd_default()
 */
void lcd_init(lcd_t * const lcd){  
    /* 100 ms delay */  
    vTaskDelay( 100 / portTICK_PERIOD_MS); 
    
    //set 0x03 to LCD 
    gpio_set_level( lcd->en, GPIO_STATE_LOW);
    gpio_set_level( lcd->regSel, GPIO_STATE_LOW);
    gpio_set_level( lcd->data[0], GPIO_STATE_HIGH);
    gpio_set_level( lcd->data[1], GPIO_STATE_HIGH);
    gpio_set_level( lcd->data[2], GPIO_STATE_LOW);
    gpio_set_level( lcd->data[3], GPIO_STATE_LOW);

    // Send 0x03 3 times at 10ms then 100 us
	lcdTriggerEN(lcd); 
	vTaskDelay( 10 / portTICK_PERIOD_MS);
	lcdTriggerEN(lcd);
	vTaskDelay( 10 / portTICK_PERIOD_MS);
	lcdTriggerEN(lcd);
	vTaskDelay( 10 / portTICK_PERIOD_MS);

    //switch to 4-bit mode, 0x02
    gpio_set_level( lcd->en, GPIO_STATE_LOW);
    gpio_set_level( lcd->regSel, GPIO_STATE_LOW);
    gpio_set_level( lcd->data[0], GPIO_STATE_LOW);
    gpio_set_level( lcd->data[1], GPIO_STATE_HIGH);
    gpio_set_level( lcd->data[2], GPIO_STATE_LOW);
    gpio_set_level( lcd->data[3], GPIO_STATE_LOW);

    lcdTriggerEN(lcd);
    vTaskDelay( 10 / portTICK_PERIOD_MS);

    // Initialize LCD
    lcdWriteCmd(lcd, 0x28, LCD_CMD); // 4-bit, 2 line, 5x8
    lcdWriteCmd(lcd, 0x08, LCD_CMD); // Instruction Flow
    lcdWriteCmd(lcd, 0x01, LCD_CMD); // Clear LCD
    lcdWriteCmd(lcd, 0x06, LCD_CMD); // Auto-Increment
    lcdWriteCmd(lcd, 0x0C, LCD_CMD); // Display On, No blink

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
    gpio_num_t data[LCD_DATA_LINE] = { 33, 32, 35, 34 }; /* Data pins */
    gpio_num_t en = 39; /* Enable pin */
    gpio_num_t regSel = 36; /* Register Select pin */

    /* Instantiate lcd object with default pins */
    lcd_ctor(lcd, data, en, regSel);
}

/**
 * @brief LCD constructor
 * 
 * Detailed description starts here 
 * @param lcd       pointer to LCD object
 * @param data      lcd data array 
 * @param en        lcd en
 * @param regSel    register select
 * @return          None
 */
void lcd_ctor(lcd_t *lcd, gpio_num_t data[LCD_DATA_LINE], gpio_num_t en, gpio_num_t regSel){
    /* Map each data pin to LCD object */
    int i;
    for(i = 0; i < LCD_DATA_LINE; i++){
        lcd->data[i] = data[i];
    }

    /* Map enable and register select pin */
    lcd->en = en;
    lcd->regSel = regSel;
 
    /* Select en and register select pin */
    esp_rom_gpio_pad_select_gpio(lcd->en);
    esp_rom_gpio_pad_select_gpio(lcd->regSel);

    /* Set en and register select pin as output */
    gpio_set_direction( lcd->en, GPIO_MODE_OUTPUT);
    gpio_set_direction( lcd->regSel, GPIO_MODE_OUTPUT);

    /* Set en and register select pin as low */
    gpio_set_level( lcd->en, GPIO_STATE_LOW);
    gpio_set_level( lcd->regSel, GPIO_STATE_LOW);

    /* Select all data pins */
    for(i = 0; i < LCD_DATA_LINE; i++){
        esp_rom_gpio_pad_select_gpio(lcd->data[i]);
    }
    /* Set all data pins as output */
    for(i = 0; i < LCD_DATA_LINE; i++){
        gpio_set_direction( lcd->data[i], GPIO_MODE_OUTPUT);
    }
    /* Set all data pins output as low */
    for(i = 0; i < LCD_DATA_LINE; i++){
        gpio_set_level( lcd->data[i], GPIO_STATE_LOW);
    }
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
void lcdSetText(lcd_t * const lcd, char * text, int x, int y){
    int i;
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
void lcdSetInt(lcd_t * const lcd, int val, int x, int y){
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
    lcdWriteCmd(lcd, 0x01, LCD_CMD);
}
