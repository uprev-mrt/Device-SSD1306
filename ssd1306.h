/**
  *@file sd1306.h
  *@brief driver for ssd1306 based oled display
  *@author Jason Berger
  *@date 3/26/2019
  */

#pragma once
#include "Platforms/Common/mrt_platform.h"
#include "Utilities/GFX/MonoGfx/mono_gfx.h"

#define BLACK                          0 // Draw 'off' pixels
#define WHITE                          1 // Draw 'on' pixels
#define INVERSE                        2 // Invert pixels

#define SSD1306_CMD_REG 0               // i2c register to send command
#define SSD1306_DATA_REG 0x40           // I2C register to send data

#define SSD1306_MEMORYMODE          0x20 // See datasheet
#define SSD1306_COLUMNADDR          0x21 // See datasheet
#define SSD1306_PAGEADDR            0x22 // See datasheet
#define SSD1306_SETCONTRAST         0x81 // See datasheet
#define SSD1306_CHARGEPUMP          0x8D // See datasheet
#define SSD1306_SEGREMAP            0xA0 // See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 // See datasheet
#define SSD1306_DISPLAYALLON        0xA5 // Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 // See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 // See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 // See datasheet
#define SSD1306_DISPLAYOFF          0xAE // See datasheet
#define SSD1306_DISPLAYON           0xAF // See datasheet
#define SSD1306_COMSCANINC          0xC0 // Not currently used
#define SSD1306_COMSCANDEC          0xC8 // See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 // See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 // See datasheet
#define SSD1306_SETPRECHARGE        0xD9 // See datasheet
#define SSD1306_SETCOMPINS          0xDA // See datasheet
#define SSD1306_SETVCOMDETECT       0xDB // See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 // Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 // Not currently used
#define SSD1306_SETSTARTLINE        0x40 // See datasheet

#define SSD1306_EXTERNALVCC         0x01 // External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02 // Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 // Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 // Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 // Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A // Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range


typedef struct{
  mrt_bus_type_e mBusType;  //This device can use I2C or SPI
  /* spi config */
  mrt_spi_handle_t mSpi;  //handle for spi bus
  mrt_gpio_t mCS;         //gpio for chip select
  mrt_gpio_t mDC;         //Data/Command mode pin
  mrt_gpio_t mRST;        //Reset (active low )
  mrt_gpio_t mBusy;       //signal to indicate display is busy
  /* I2C config */
  mrt_i2c_handle_t mI2c;
  uint8_t mI2cAddr;
}ssd1306_hw_cfg_t;

typedef struct{
  mono_gfx_t mCanvas;
  const GFXfont* mFont;
  wc_oled_hw_cfg_t mHW;
}ssd1306_t;

void ssd1306_init(ssd1306_t* dev,ssd1306_hw_cfg_t* hw, uint16_t width, uint16_t height );
void ssd1306_refresh(ssd1306_t* dev);
void ssd1306_clearDisplay(ssd1306_t* dev);
void ssd1306_invertDisplay(ssd1306_t* dev,boolean i);
void ssd1306_dim(ssd1306_t* dev,boolean dim);
void ssd1306_command(ssd1306_t* dev, uint8_t cmd);
void ssd1306_data(ssd1306_t* dev, uint8_t* data, int len);
