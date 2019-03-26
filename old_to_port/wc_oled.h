#ifndef WC_OLED_H_
#define WC_OLED_H_

#include "gfxfont.h"
#include "pins.h"
#include "spi_abstract.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  uint16_t width;
  uint16_t height;
  uint16_t cursor;
  uint8_t* buffer;
  uint16_t buffer_size;
  GFXfont* font;
  uint8_t dc;
  uint8_t cs;
  uint8_t rst;
  bool spi;
  bool twi;
  uint8_t rotation;
}oled_t;

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

void oled_init(uint16_t w, uint16_t h, uint8_t dc, uint8_t cs, uint8_t rst);
void oled_refresh();
void oled_put_pixel(int16_t x, int16_t y, uint16_t color);
void oled_fast_pixels(uint16_t x, uint16_t y, uint8_t* data, uint16_t len, uint16_t offset, uint16_t color);
void oled_draw_bmp(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* bmp,uint16_t color);
uint8_t oled_draw_char(uint16_t x, uint16_t y, char c);
void oled_print(uint16_t x, uint16_t y, char * text);
void oled_print_R(uint16_t x, uint16_t y, char * text);
void oled_clear();
void oled_test();

void ssd1306_command(uint8_t cmd);
void ssd1306_data(uint8_t* data, uint8_t len );


#endif/* WC_OLED_H_ */
