/**
  *@file sd1306.c
  *@brief driver for ssd1306 based oled display
  *@author Jason Berger
  *@date 3/26/2019
  */





#include "ssd1306.h"

void ssd1306_init(ssd1306_t* dev,ssd1306_hw_cfg_t* hw, uint16_t width, uint16_t height )
{

  //copy in hardware config
  memcpy(&dev->mHW, hw, sizeof(ssd1306_hw_cfg_t));

  //initialize graphics canvases as buffered so they maintain their own data
  mono_gfx_init_buffered(&dev->mCanvas, width, height);

  //TODO RESET DEVICE

  //vcc source
  uint8_t vccstate = SSD1306_SWITCHCAPVCC

  ssd1306_command(dev,SSD1306_DISPLAYOFF);                  // 0xAE
  ssd1306_command(dev,SSD1306_SETDISPLAYCLOCKDIV);          // 0xD5
  ssd1306_command(dev,0x80);                                // the suggested ratio 0x80
  ssd1306_command(dev,SSD1306_SETMULTIPLEX );               // 0xA8
  ssd1306_command(dev,height - 1);


  ssd1306_command(dev,SSD1306_SETDISPLAYOFFSET);            // 0xD3
  ssd1306_command(dev,0x0);                                 // no offset
  ssd1306_command(dev,SSD1306_SETSTARTLINE | 0x0);          // line #0
  ssd1306_command(dev,SSD1306_CHARGEPUMP );                 // 0x8D
  ssd1306_command(dev,SSD1306_MEMORYMODE);                  // 0x20
  ssd1306_command(dev,0x00);                                // 0x0 act like ks0108
  ssd1306_command(dev,SSD1306_SEGREMAP | 0x1,
  ssd1306_command(dev,SSD1306_COMSCANDEC );

  if((width == 128) && (height == 32))
  {
    ssd1306_command(dev,SSD1306_SETCOMPINS);                // 0xDA
    ssd1306_command(dev,0x02,
    ssd1306_command(dev,SSD1306_SETCONTRAST);               // 0x81
    ssd1306_command(dev,0x8F );
  }
  else if((width == 128) && (height == 64))
  {
    ssd1306_command(dev,SSD1306_SETCOMPINS);                // 0xDA
    ssd1306_command(dev,0x12,
    ssd1306_command(dev,SSD1306_SETCONTRAST );              // 0x81
    ssd1306_command(dev,(vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
  }
  else if((width == 96) && (height == 16))
  {
    ssd1306_command(dev,SSD1306_SETCOMPINS);                // 0xDA
    ssd1306_command(dev,0x2);   // ada x12
    ssd1306_command(dev,SSD1306_SETCONTRAST );              // 0x81
    ssd1306_command(dev,(vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF);
  }
  else
  {
    // Other screen varieties -- TBD
  }

  ssd1306_command(dev,SSD1306_SETPRECHARGE); // 0xd9
  ssd1306_command(dev,(vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
  ssd1306_command(dev,SSD1306_SETVCOMDETECT);              // 0xDB
  ssd1306_command(dev,0x40,
  ssd1306_command(dev,SSD1306_DISPLAYALLON_RESUME);        // 0xA4
  ssd1306_command(dev,SSD1306_NORMALDISPLAY);              // 0xA6
  ssd1306_command(dev,SSD1306_DEACTIVATE_SCROLL,
  ssd1306_command(dev,SSD1306_DISPLAYON );                 // Main screen turn on
}

void ssd1306_refresh(ssd1306_t* dev)
{

  ssd1306_command(dev,SSD1306_PAGEADDR);
  ssd1306_command(dev,0); // Page start address (0 = reset)
  ssd1306_command(dev,0xFF); // Page end address

  ssd1306_command(dev,SSD1306_COLUMNADDR);
  ssd1306_command(dev,0);   // Column start address (0 = reset)
  ssd1306_command(dev,dev->mCanvas.mWidth -1); // Column end address (127 = reset)

  int blockSize = dev->mCanvas.mWidth /8;

  for(int i= 0; i < dev->mCanvas.mHeight; i ++)
  {
    ssd1306_data(dev, &dev->mCanvas.mBuffer[i * blockSize],blockSize);
  }

  return MRT_STATUS_OK;
}

void ssd1306_invertDisplay(ssd1306_t* dev,boolean i);
void ssd1306_dim(ssd1306_t* dev,boolean dim);

void ssd1306_command(ssd1306_t* dev, uint8_t cmd)
{
  if(dev->mHw.mBusType == MRT_BUS_SPI)
  {
    MRT_GPIO_WRITE(dev->mHW.mDC,LOW);
    MRT_GPIO_WRITE(dev->mHW.mCS,LOW);
    MRT_DELAY_MS(1);
    MRT_SPI_TRANSMIT(dev->mHW.mSpi , &cmd, 1, 20);
    MRT_GPIO_WRITE(dev->mHW.mCS,HIGH);
    MRT_DELAY_MS(1);
  }
  else  /* I2C */
  {
    MRT_I2C_MEM_WRITE(dev->mHw.mI2c,dev->mHw.mI2cAddr ,SSD1306_CMD_REG,1,cmd,1);
  }
}

void ssd1306_data(ssd1306_t* dev, uint8_t* data, int len)
{
  if(dev->mHw.mBusType == MRT_BUS_SPI)
  {
    MRT_GPIO_WRITE(dev->mHW.mDC,HIGH);
    MRT_GPIO_WRITE(dev->mHW.mCS,LOW);
    MRT_DELAY_MS(1);
    MRT_SPI_TRANSMIT(dev->mHW.mSpi , data, len, 20);
    MRT_GPIO_WRITE(dev->mHW.mCS,HIGH);
    MRT_DELAY_MS(1);
  }
  else  /* I2C */
  {
    MRT_I2C_MEM_WRITE(dev->mHw.mI2c,dev->mHw.mI2cAddr ,SSD1306_DATA_REG,1,cmd,1);
  }
}
