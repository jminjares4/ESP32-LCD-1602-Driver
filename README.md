
<div align="center">
  <img src="images/lcd.png" height="250" width="500">
</div>

## **Description**
This repository contains a custom lcd driver for 16x02 lcd. The driver has the most common function calls
that are use for LCDs. ESP32 have limited GPIO pins, therefore the driver was developed to use 4-bit mode which requires a total of 6 GPIO pins. However, there are two configurations for the driver: default and custom. The default configurations uses [this](#lcd-16x02-pinout) pinout while the custom allows the user to select whichever GPIO are avaiable simple by using `lcd_ctor()`.

### **ESP32 Pinout**
~~~
                                         +-----------------------+
                                         | O      | USB |      O |
                                         |        -------        |
                                     3V3 | [ ]               [ ] | VIN
                                     GND | [ ]               [ ] | GND
     Touch3 / HSPI_CS0 / ADC2_3 / GPIO15 | [ ]               [ ] | GPIO13 / ADC2_4 / HSPI_ID / Touch4
 CS / Touch2 / HSPI_WP / ADC2_2 /  GPIO2 | [ ]               [ ] | GPIO12 / ADC2_5 / HSPI_Q / Touch5
      Touch0 / HSPI_HD / ADC2_0 /  GPIO4 | [ ]               [ ] | GPIO14 / ADC2_6 / HSPI_CLK / Touch6
                         U2_RXD / GPIO16 | [ ]               [ ] | GPIO27 / ADC2_7 / Touch7
                         U2_TXD / GPIO17 | [ ]               [ ] | GPIO26 / ADC2_9 / DAC2
                      V_SPI_CS0 /  GPIO5 | [ ]  ___________  [ ] | GPIO25 / ADC2_8 / DAC1
                SCK / V_SPI_CLK / GPIO18 | [ ] |           | [ ] | GPIO33 / ADC1_5 / Touch8 / XTAL32
        U0_CTS / MSIO / V_SPI_Q / GPIO19 | [ ] |           | [ ] | GPIO32 / ADC1_4 / Touch9 / XTAL32
                 SDA / V_SPI_HD / GPIO21 | [ ] |           | [ ] | GPIO35 / ADC1_7 
                  CLK2 / U0_RXD /  GPIO3 | [ ] |           | [ ] | GPIO34 / ADC1_6 
                  CLK3 / U0_TXD /  GPIO1 | [ ] |           | [ ] | GPIO39 / ADC1_3 / SensVN 
        SCL / U0_RTS / V_SPI_WP / GPIO22 | [ ] |           | [ ] | GPIO36 / ADC1_0 / SensVP 
                MOSI / V_SPI_WP / GPIO23 | [ ] |___________| [ ] | EN 
                                         |                       |
                                         |  |  |  ____  ____  |  |
                                         |  |  |  |  |  |  |  |  |
                                         |  |__|__|  |__|  |__|  |
                                         | O                   O |
                                         +-----------------------+
~~~


## **LCD 16x02 Pinout**

The following pinout is the default configuration of the ESP-LCD driver. However, the driver 
is customizable to allow users to change GPIO pins if necessary.

| **ESP32 Pins**                            | **LCD Pin** |  **Description**  |   
| :---:                                     |  :--:       |   :---:           |
| `GND`, 10k Potentiometer Output (V&#177;) |      1      | GND               |  
| `VCC`, 10k Potentiometer Output (V&#177;) |      2      | VCC               |    
| 10k Potentiometer Output (V<sub>o</sub>)  |      3      | Contrast          |    
| `GPIO36`                                  |      4      | RS (Register Select): 0–Command, 1-Data |    
| `GND`                                     |      5      | R/W (Read/Write): 0 – Write, 1 - Read   |    
| `GPIO39`                                  |      6      | Clock Enable      |    
| `NC`                                      |      7      | Data 0            |
| `NC`                                      |      8      | Data 1            |    
| `NC`                                      |      9      | Data 2            |    
| `NC`                                      |      10     | Data 3            |    
| `GPIO33`                                  |      11     | Data 4            |    
| `GPIO32`                                  |      12     | Data 5            |    
| `GPIO35`                                  |      13     | Data 6            |    
| `GPIO34`                                  |      14     | Data 7            |    
| 100&#8486; to `VCC`                       |      15     | Backlight Anode (+)  |    
| `GND`                                     |      16     | Backlight Cathode (-)|    
  * LCD default pin configuration
  
## **LCD Main Functions**

| **Function**  | **Description**                 |
| :---          | :---                            |
| lcd_default() | Default pinout                  |
| lcd_ctor()    | Customizable pinout constructor |
| lcdSetText()  | Set text                        |
| lcdSetInt     | Set integer                     |
| lcdClear()    | Clear previous data             |

## **How to use LCD Driver**
The follow section of code demostrate how to use the lcd driver with default and custom configuration.
~~~c
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
~~~

## **Add ESP-LCD to ESP32 Project**
1) Copy driver folder
2) Paste into esp project
3) Edit project CMakeLists.txt to use esp_lcd:
```cmake
idf_component_register(SRCS "main.c"
                            "driver/esp_lcd.c"
                    INCLUDE_DIRS ".")
```

## **Authors:**
* [**Jesus Minjares** :zap:](https://github.com/jminjares4)<br>
  * Master of Science in Computer Engineering<br>
[![Outlook](https://img.shields.io/badge/Microsoft_Outlook-0078D4?style=for-the-badge&logo=microsoft-outlook&logoColor=white&style=flat)](mailto:jminjares4@miners.utep.edu) 
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)

