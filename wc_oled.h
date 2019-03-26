/**
  *@file wc_oled.h
  *@brief driver for wise chip oled display (ssd1306)
  *@author Jason Berger
  *@date 3/26/2019
  */

#include "Platforms/Common/mrt_platform.h"
#include "Utilities/GFX/MonoGfx/mono_gfx.h"

#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64
#define SSD1306_ADDR        0x78

#define OLED_SPI 0x1
#define OLED_TWI  0x2
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define WHITE 0
#define BLACK 1
#define INVERSE 2

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

typedef struct{
  mrt_spi_handle_t mSpi;  //handle for spi bus
  mrt_gpio_t mCS;         //gpio for chip select
  mrt_gpio_t mDC;         //Data/Command mode pin
  mrt_gpio_t mRST;        //Reset (active low )
  mrt_gpio_t mBusy;       //signal to indicate display is busy
}wc_oled_hw_cfg_t;

typedef struct{
  mono_gfx_t mCanvas;
  const GFXfont* mFont;
  wc_oled_hw_cfg_t mHW;
}wc_oled_t;

/**
  *@brief initialize lcd driver
  *@param dev ptr to device descriptor
  *@param hw wc_oled hardware config
  *@param width width (in pixels) of display buffer
  *@param height height (in pixels) of display buffer
  *@return MRT_STATUS_OK
  */
mrt_status_t wc_oled_init(wc_oled_t* dev, wc_oled_hw_cfg_t* hw, int width, int height);

/**
  *@brief sends command over spi bus to device
  *@param dev ptr to wc_oled device
  *@param cmd command to send
  */
void wc_oled_cmd(wc_oled_t* dev, uint8_t cmd);

/**
  *@brief sends data byte over spi bus to device
  *@param dev ptr to wc_oled device
  *@param cmd command to send
  */
void wc_oled_data(wc_oled_t* dev, uint8_t data);

/**
  *@brief updates the device with the local buffer
  *@param dev ptr to device descriptor
  *@return MRT_STATUS_OK
  */
mrt_status_t wc_oled_update(wc_oled_t* dev);


/**
  *@brief performs reset of the device by setting reset pin
  *@param dev ptr to device descriptor
  *@return status of operation
  */
mrt_status_t wc_oled_reset(wc_oled_t* dev);

/**
  *@brief Draws a bitmap to the buffer
  *@param dev ptr to device descriptor
  *@param x x coord to begin drawing at
  *@param y y coord to begin drawing at
  *@param bmp bitmap to draw
  *@return status of operation
  */
mrt_status_t wc_oled_draw_bmp(wc_oled_t* dev, uint16_t x, uint16_t y, GFXBmp* bmp, ink_color_e color);

/**
  *@brief Draws rendered text to the buffer
  *@param dev ptr to device descriptor
  *@param x x coord to begin drawing at
  *@param y y coord to begin drawing at
  *@param text text to be written
  *@return status of operation
  */
mrt_status_t wc_oled_print(wc_oled_t* dev, uint16_t x, uint16_t y, const char * text, ink_color_e color);

/**
  *@brief clears buffer and display
  *@param dev ptr to device
  *@return status of operation
  */
mrt_status_t wc_oled_clear(wc_oled_t* dev);


/**
  *@brief fill buffer with value
  *@param dev ptr to device
  *@param val value to write
  *@return status of operation
  */
mrt_status_t wc_oled_fill(wc_oled_t* dev, ink_color_e color, uint8_t val);

/**
  *@brief waits for device to not be busy
  *@param dev ptr to device
  *@return status of operation
  */
mrt_status_t wc_oled_wait(wc_oled_t* dev, int timeout_ms);

/**
  *@brief puts device into sleep mode
  *@param dev ptr to device
  *@return status
  */
mrt_status_t wc_oled_sleep(wc_oled_t* dev);
