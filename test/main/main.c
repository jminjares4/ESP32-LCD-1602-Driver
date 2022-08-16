/**
 * @file main.c
 * @author Jesus Minjares (https://github.com/jminjares4)
 * @brief Test lcd driver
 * @version 0.1
 * @date 2022-08-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include "driver/esp_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DEFAULT_LCD

void lcd_task(void *pvParameters)
{

  /* Create LCD object */
  lcd_t lcd;

#ifdef DEFAULT_LCD
  lcd_default(&lcd);
#else
  /* Set pins */
  gpio_num_t data[4] = {19, 18, 17, 16}; /* Data pins */
  gpio_num_t en = 22;                    /* Enable pin */
  gpio_num_t regSel = 23;                /* Register Select pin */

  /* Constructor LCD object */
  lcd_ctor(&lcd, data, en, regSel);

#endif

  /* Initialize LCD object */
  lcd_init(&lcd);

  /* Clear previous data on LCD */
  lcdClear(&lcd);

  /* Custom text */
  char buffer[16];
  float version = 1.0;
  char initial[2] = {'J', 'M'};
  sprintf(buffer, "ESP v%.2f %c%c", version, initial[0], initial[1]);

  lcdSetText(&lcd, buffer, 0, 0);

  int count = 0;

  while (1)
  {
    /* Display text */
    lcdSetText(&lcd, "Count: ", 0, 1);
    lcdSetInt(&lcd, count, 8, 1);
    count++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{

  /* Create LCD task */
  xTaskCreate(lcd_task, "LCD task", 2048, NULL, 4, NULL);
}
