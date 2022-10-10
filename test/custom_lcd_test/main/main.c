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
#include "esp_log.h"

/* LCD tag */
static const char *lcd_tag = "LCD tag";

/* Default lcd */
#define DEFAULT_LCD

void lcd_task(void *pvParameters)
{

  /* Create LCD object */
  lcd_t lcd;

#ifdef DEFAULT_LCD
  /* Set default pinout */
  lcdDefault(&lcd);
#else
  /* Set pins */
  gpio_num_t data[4] = {19, 18, 17, 16}; /* Data pins */
  gpio_num_t en = 22;                    /* Enable pin */
  gpio_num_t regSel = 23;                /* Register Select pin */

  /* Constructor LCD object */
  lcdCtor(&lcd, data, en, regSel);

#endif

  /* Initialize LCD object */
  lcdInit(&lcd);

  /* Clear previous data on LCD */
  lcdClear(&lcd);

  /* Custom text */
  char buffer[16];
  float version = 1.0;
  char initial[2] = {'J', 'M'};
  sprintf(buffer, "ESP v%.1f %c%c", version, initial[0], initial[1]);

  /* Set text */
  lcd_err_t ret = lcdSetText(&lcd, buffer, 0, 0);

  /* Check lcd status */
  assert_lcd(ret);

  /* Initialize count */
  int count = 0;

  /* 5 second run */
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  /* Free LCD */
  lcdFree(&lcd);

  /* Set text */
  ret = lcdSetText(&lcd, "Count: ", 0, 1);

  /* Check lcd status */
  assert_lcd(ret);

#ifdef DEFAULT_LCD
  lcdDefault(&lcd); /* Re-enable lcd default pins */
#else
  lcdCtor(&lcd, data, en, regSel); /* Re-enable lcd custom pins */
#endif
  lcdInit(&lcd);                                                      /* Re-intialize lcd */
  sprintf(buffer, "ESP v%.1f %c%c", version, initial[0], initial[1]); /* Reset custom text*/
  ret = lcdSetText(&lcd, buffer, 0, 0);                               /* Set text */

  /* Check lcd status */
  assert_lcd(ret);

  while (1)
  {
    /* Display text */
    ret = lcdSetText(&lcd, "Count: ", 0, 1);

    /* Check lcd status */
    assert_lcd(ret);
    ret = lcdSetInt(&lcd, count, 8, 1);

    /* Check lcd status */
    assert_lcd(ret);

    /* Increment count */
    count++;
    
    /* 1 second delay */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  /* Create LCD task */
  xTaskCreate(lcd_task, "LCD task", 2048, NULL, 4, NULL);
}
