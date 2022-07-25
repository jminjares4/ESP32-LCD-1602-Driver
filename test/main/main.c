#include <stdio.h>
#include "include/esp_lcd.h"


#define DEFAULT_LCD


void app_main(void)
{
  
  /* Create LCD object */
  lcd_t lcd;

  #ifdef DEFAULT_LCD
      lcd_default(&lcd);
  #else
      uint32_t data[] = { 33, 32, 35, 34 }; /* Data pins */
      uint32_t enable = 39; /* Enable pin */
      uint32_t regSel = 36; /* Register Select pin */

      lcd_ctor(&lcd, data, enable, regSel);
  #endif

  /* Clear previous data on LCD */
  lcdClear(&lcd);
  
  /* Set text */
  lcdSetText(&lcd, "ESP-LCD!!!", 0,0);

  /* Set integer */ 
  lcdSetInt(&lcd, 10, 0, 10); 

  /* Set custom text */
  char buffer[16];
  float version = 1.0;
  char initial[2] = {'J', 'M'};
  sprintf(buffer, "ESP v%.2f %c%c", version, initial[0], initial[1]);

  while(1){
    /* infinite loop */
  }

}
