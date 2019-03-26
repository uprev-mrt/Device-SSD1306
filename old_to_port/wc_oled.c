/*	wc_oled.c
 *	Created by: Jason Berger
 * source code for wisechip oled display
 */

#include "wc_oled.h"
#include "spi_abstract.h"
#include "twi_abstract.h"
#include <stdlib.h>
#include "delay.h"
//#include "xanadu32.h"
#include "boron.h"
#include "images.h"

oled_t m_oled;
uint8_t vccstate;
uint8_t OLED_BUS = 0;

#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }

static uint8_t buffer[1024];

void oled_init(uint16_t w, uint16_t h, uint8_t dc, uint8_t cs, uint8_t rst)
{
  m_oled.width = w;
  m_oled.height = h;
  m_oled.buffer_size = w*h/8;
  m_oled.buffer = &buffer;
  m_oled.cursor = 0;
  // for(int i=0; i < m_oled.buffer_size; i++)
  // {
  //   m_oled.buffer[i] = 0x00;
  // }

  m_oled.rotation = 0;
  m_oled.cs = cs;
  m_oled.dc = dc;
  m_oled.rst = rst;
  m_oled.spi = true;
  m_oled.twi = false;
  m_oled.font = &Boring_Boron32pt7b;

  pinMode(rst, OUTPUT);
  pinMode(m_oled.cs, OUTPUT);
  pinMode(m_oled.dc, OUTPUT);
  digitalWrite(m_oled.cs,HIGH);
  digitalWrite(m_oled.dc,HIGH);

//  digitalWrite(PC9,HIGH);
  digitalWrite(rst, HIGH);


  if(m_oled.spi)
  {
    spi_bb_init(PB5,PA6,PA5);

    //spi_master_init(2);
  }


  vccstate = SSD1306_SWITCHCAPVCC;

  // Init sequence
 ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
 ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
 ssd1306_command(0x80);                                  // the suggested ratio 0x80

 ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
 ssd1306_command(SSD1306_LCDHEIGHT - 1);

 ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
 ssd1306_command(0x0);                                   // no offset
 ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
 ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
 if (vccstate == SSD1306_EXTERNALVCC)
   { ssd1306_command(0x10); }
 else
   { ssd1306_command(0x14); }
 ssd1306_command(SSD1306_MEMORYMODE );                    // 0x20
 ssd1306_command(0x00);                                  // 0x0 act like ks0108
 ssd1306_command(SSD1306_SEGREMAP | 0x1);
 ssd1306_command(SSD1306_COMSCANDEC);


  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
 ssd1306_command(0x12);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x9F); }
  else
    { ssd1306_command(0xCF); }

  ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x22); }
  else
    { ssd1306_command(0xF1); }
  ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  ssd1306_command(0x40);
  ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6

  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
  //oled_refresh();

}

void oled_refresh()
{
  //  oled_refresh_sh1106();
    //return;
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);   // Column start address (0 = reset)
  ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0); // Page start address (0 = reset)
  ssd1306_command(7); // Page end address

  for(int i= 0; i < 64; i ++)
  {
    ssd1306_data(&m_oled.buffer[i*16],16);
  }
}



void oled_put_pixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= m_oled.width || (y < 0) || (y >= m_oled.height)))
    return;

  // check rotation, move pixel around if necessary
  switch (m_oled.rotation)
  {
    case 1:
      ssd1306_swap(x, y);
      x = m_oled.width - x - 1;
      break;
    case 2:
      x = m_oled.width - x - 1;
      y = m_oled.height - y - 1;
      break;
    case 3:
      ssd1306_swap(x, y);
      y = m_oled.height - y - 1;
      break;
  }

  // x is which column
    switch (color)
    {
      case WHITE:   buffer[x+ (y/8)*m_oled.width] |=  (1 << (y&7)); break;
      case BLACK:   buffer[x+ (y/8)*m_oled.width] &= ~(1 << (y&7)); break;
      case INVERSE: buffer[x+ (y/8)*m_oled.width] ^=  (1 << (y&7)); break;
    }

}

void oled_fast_pixels(uint16_t x, uint16_t y, uint8_t* data, uint16_t len, uint16_t offset, uint16_t color)
{
  uint8_t ind=offset/8;
  uint8_t bit_offset = offset% 8;
  uint8_t cur_byte;
  uint16_t xx = x;

  if((bit_offset % 8 )!= 0)
    cur_byte = data[ind++] << bit_offset;

  for(int i=0; i < len; i++)
  {
    if((i+bit_offset) % 8 == 0)
      cur_byte = data[ind++];

    if(cur_byte & 0x80)
      oled_put_pixel(xx,y,color);

    xx++;
    cur_byte = cur_byte<<1;
  }
}

void oled_draw_bmp(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* bmp, uint16_t color)
{

  uint16_t src_cursor = 0;
  for(int i=0; i < h; i ++)
  {
    oled_fast_pixels(x,y+i, &bmp[src_cursor], w,0,color);
    src_cursor+= w/8;
  }

}

uint8_t oled_draw_char(uint16_t x, uint16_t y, char c)
{
  uint8_t ind = (uint8_t)c - 0x20;
  GFXglyph* glyph = &m_oled.font->glyph[ind];

  uint8_t width = glyph->width;
  uint8_t height = glyph->height;
  uint16_t src_cursor =  glyph->bitmapOffset;
  uint16_t adv = (width+7)/8;


  for(int i=0; i < glyph->height; i ++)
  {
    oled_fast_pixels(x+glyph->xOffset,y+glyph->yOffset+i, &m_oled.font->bitmap[src_cursor], width,width*i , WHITE);
    //src_cursor+= adv;
  }

  return glyph->xAdvance + 3;

}

void oled_print(uint16_t x, uint16_t y, char * text)
{
  uint16_t ind = 0;
  uint16_t xx =x;
  uint16_t yy = y;
  char c = text[ind++];
  while(c != 0)
  {
    if(c == '\n')
    {
      yy+= m_oled.font->yAdvance;
      xx = x;
    }
    else
    {
      xx += oled_draw_char(xx, yy, c);
    }

    c=text[ind++];
  }
}

//print text with right alignment
void oled_print_R(uint16_t x, uint16_t y, char * text)
{
  int len = strlen(text);
  uint8_t ind;
  GFXglyph* glyph;
  for(int i=0; i < len; i++)
  {
    ind = (uint8_t)text[i] -0x20;
    glyph = &m_oled.font->glyph[ind];
    x -= (glyph->xAdvance + 3);
  }
  oled_print(x,y,text);
}

void ssd1306_command(uint8_t cmd)
{
  if(m_oled.spi)
  {
    digitalWrite(m_oled.cs, HIGH);
    digitalWrite(m_oled.dc, LOW);
    digitalWrite(m_oled.cs, LOW);

    spi_transfer_byte(cmd);
    delay_us(15);

    digitalWrite(m_oled.cs, HIGH);
  }
  if(m_oled.twi)
  {
    uint8_t packet[2];
    packet[0] = 0;
    packet[1] = cmd;
    twi_master_transfer(SSD1306_ADDR, packet, 2, true);
    delay_us(15);
  }
}

void ssd1306_data(uint8_t* data, uint8_t len )
{

  if(m_oled.spi)
  {
    digitalWrite(m_oled.cs, HIGH);
    digitalWrite(m_oled.dc, HIGH);
    digitalWrite(m_oled.cs, LOW);

    spi_master_send(data,len);
    delay_us(len*10);

    digitalWrite(m_oled.cs,HIGH);
  }
  if(m_oled.twi)
  {
    uint8_t packet[len+1];
    packet[0] = 0x40;
    for(int i=0; i < len; i ++)
      {
        packet[1+i] = data[i];
      }
      twi_master_transfer(SSD1306_ADDR, packet, len+1, true);
  }


}

void oled_clear()
{
  for(int i=0;i < 1024; i++)
  {
    m_oled.buffer[i]=0;
  }
}
void oled_test()
{
  uint8_t test_pattern[] = {0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF};
  uint8_t test_pattern2[] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF};
  uint8_t clear[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for(int i=0; i < 64; i++)
  {
       oled_fast_pixels(0,i,clear,16*8,0, WHITE);
       //oled_put_pixel(i,i,WHITE);
      //  oled_draw_bmp(0,0,72,85,  num_three);
      //  oled_draw_bmp(0,0,72,85,  num_two);
      // for(int a=0; a < 128; a +=10)
      // {
      //   oled_put_pixel(a,i,WHITE);
      // }

  }

//  oled_refresh();

  // oled_draw_char(10,60,'!');
  // oled_draw_char(45,60,'1');
  // oled_draw_char(80,60,'3');
  oled_print(10,60,"145");

  oled_refresh();

}
