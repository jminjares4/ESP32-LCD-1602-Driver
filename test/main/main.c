/**
 * @file main.c
 * @author Jesus Minjares (https://github.com/jminjares4)
 * @brief Test driver code
 * @version 0.1
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include "driver/esp_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void lcd_task(void *pvParameters){

  /* Create LCD object */
  lcd_t lcd;

  gpio_num_t data[4] = { 19, 18, 17, 16 }; /* Data pins */
  gpio_num_t en = 22; /* Enable pin */
  gpio_num_t regSel = 23; /* Register Select pin */

  /* Constructor LCD object */
  lcd_ctor(&lcd, data, en, regSel);

  /* Initialize LCD object */
  lcd_init(&lcd);

  /* Clear previous data on LCD */
  lcdClear(&lcd);

  lcdSetText(&lcd, "ESP-LCD!!!", 5,0);

  int count = 0;
  
  while(1){
    /* Display text */
    char buffer[16];
    sprintf(buffer, "Count: %d", count);
    lcdSetText(&lcd, buffer, 0,1);
    count++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }


}

void app_main(void)
{
  xTaskCreate(lcd_task, "LCD task", 2048, NULL, 4, NULL);
}
