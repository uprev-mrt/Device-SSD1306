/**
  *@file wc_oled.c
  *@brief driver for wisechipe oled display
  *@author Jason Berger
  *@date 3/26/2019
  */

#include "wc_oled.h"
#include "string.h"
#include <stdlib.h>

uint8_t vccstate;


mrt_status_t wc_oled_init(wc_oled_t* dev, wc_oled_hw_cfg_t* hw, int width, int height)
{

    //copy in hardware config
    memcpy(&dev->mHW, hw, sizeof(wc_oled_hw_cfg_t));


    //initialize graphics canvases as buffered so they maintain their own data
    mono_gfx_init_buffered(&dev->mCanvas, width, height);

	MRT_GPIO_WRITE(dev->mHW.mRST,HIGH);


    dev->mFont = NULL;

    vccstate = SSD1306_EXTERNALVCC;

    // Init sequence
   wc_oled_cmd(dev,SSD1306_DISPLAYOFF);                    // 0xAE
   wc_oled_cmd(dev,SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
   wc_oled_cmd(dev,0x80);                                  // the suggested ratio 0x80

   wc_oled_cmd(dev,SSD1306_SETMULTIPLEX);                  // 0xA8
   wc_oled_cmd(dev,SSD1306_LCDHEIGHT - 1);

   wc_oled_cmd(dev,SSD1306_SETDISPLAYOFFSET);              // 0xD3
   wc_oled_cmd(dev,0x0);                                   // no offset
   wc_oled_cmd(dev,SSD1306_SETSTARTLINE | 0x0);            // line #0
   wc_oled_cmd(dev,SSD1306_CHARGEPUMP);                    // 0x8D
   if (vccstate == SSD1306_EXTERNALVCC)
   {
     wc_oled_cmd(dev,0x10);
   }
   else
   {
     wc_oled_cmd(dev,0x14);
   }
   wc_oled_cmd(dev,SSD1306_MEMORYMODE );                    // 0x20
   wc_oled_cmd(dev,0x00);                                  // 0x0 act like ks0108
   wc_oled_cmd(dev,SSD1306_SEGREMAP | 0x1);
   wc_oled_cmd(dev,SSD1306_COMSCANDEC);


   wc_oled_cmd(dev,SSD1306_SETCOMPINS);                    // 0xDA
   wc_oled_cmd(dev,0x12);
   wc_oled_cmd(dev,SSD1306_SETCONTRAST);                   // 0x81
   if (vccstate == SSD1306_EXTERNALVCC)
   {
     wc_oled_cmd(dev,0x9F);
   }
    else
    {
      wc_oled_cmd(dev,0xCF);
    }

    wc_oled_cmd(dev,SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC)
    {
      wc_oled_cmd(dev,0x22);
    }
    else
    {
      wc_oled_cmd(dev,0xF1);
    }
    wc_oled_cmd(dev,SSD1306_SETVCOMDETECT);                 // 0xDB
    wc_oled_cmd(dev,0x40);
    wc_oled_cmd(dev,SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    wc_oled_cmd(dev,SSD1306_NORMALDISPLAY);                 // 0xA6

    wc_oled_cmd(dev,SSD1306_DEACTIVATE_SCROLL);

    wc_oled_cmd(dev,SSD1306_DISPLAYON);//--turn on oled panel
    //oled_refresh();

  return MRT_STATUS_OK;
}


void wc_oled_cmd(wc_oled_t* dev, uint8_t cmd)
{

  MRT_GPIO_WRITE(dev->mHW.mDC,LOW);
  MRT_GPIO_WRITE(dev->mHW.mCS,LOW);
  MRT_DELAY_MS(1);
  MRT_SPI_TRANSMIT(dev->mHW.mSpi , &cmd, 1, 20);
  MRT_GPIO_WRITE(dev->mHW.mCS,HIGH);
  MRT_DELAY_MS(1);

}

void wc_oled_data(wc_oled_t* dev, uint8_t* data, int len)
{

  MRT_GPIO_WRITE(dev->mHW.mDC,HIGH);
  MRT_GPIO_WRITE(dev->mHW.mCS,LOW);
  MRT_DELAY_MS(1);
  MRT_SPI_TRANSMIT(dev->mHW.mSpi , data, len, 20);
  MRT_GPIO_WRITE(dev->mHW.mCS,HIGH);
  MRT_DELAY_MS(1);

}


mrt_status_t wc_oled_update(wc_oled_t* dev)
{
  //TODO
  //  oled_refresh_sh1106();
  //return;
  wc_oled_cmd(dev,SSD1306_COLUMNADDR);
  wc_oled_cmd(dev,0);   // Column start address (0 = reset)
  wc_oled_cmd(dev,SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

  wc_oled_cmd(dev,SSD1306_PAGEADDR);
  wc_oled_cmd(dev,0); // Page start address (0 = reset)
  wc_oled_cmd(dev,7); // Page end address

  for(int i= 0; i < 64; i ++)
  {
    wc_oled_data(dev, &dev->mCanvas.mBuffer[i*16],16);
  }
  return MRT_STATUS_OK;
}



mrt_status_t wc_oled_reset(wc_oled_t* dev)
{

  //TODO
  return MRT_STATUS_OK;
}

mrt_status_t wc_oled_clear(wc_oled_t* dev)
{
    mono_gfx_fill(&dev->mCanvas,0);
    return MRT_STATUS_OK;
}

mrt_status_t wc_oled_wait(wc_oled_t* dev, int timeout_ms)
{
  //TODO

  return MRT_STATUS_ERROR;
}
