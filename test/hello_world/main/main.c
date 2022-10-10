/**
 * @file main.c
 * @author Jesus Minjares (https://github.com/jminjares4)
 * @brief Test lcd driver
 * @version 0.1
 * @date 2022-10-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include "driver/esp_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void lcd_task(void *pvParameters)
{

  /* Create LCD object */
  lcd_t lcd;

  /* Set default pinout */
  lcdDefault(&lcd);

  /* Initialize LCD object */
  lcdInit(&lcd);

  /* Clear previous data on LCD */
  lcdClear(&lcd);

  /* Custom text */
  char buffer[16];
  float version = 1.0;
  char initial[2] = {'J', 'M'};
  sprintf(buffer, "Hello World!", version, initial[0], initial[1]);

  /* Set text */
  lcd_err_t ret = lcdSetText(&lcd, buffer, 0, 0);

  /* Check lcd status */
  assert_lcd(ret);

  /* create count variable */
  static int count = 0;

  while (1)
  {
    /* Store custom text */
    sprintf(buffer, "Count: %d", count++);

    /* Set custom text */
    ret = lcdSetText(&lcd, buffer, 0, 1);

    /* Check if lcd failed */
    assert_lcd(ret);

    /* 1 second delay */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  /* Create LCD task */
  xTaskCreate(lcd_task, "LCD task", 2048, NULL, 4, NULL);
}
