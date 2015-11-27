/*
 * LEGO® MINDSTORMS EV3
 *
 * Copyright (C) 2010-2013 The LEGO Group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
 
#include  "lms2012.h"
#include  "d_lcd.h"

#include  <stdio.h>
#include  <string.h>
#include  <math.h>
#include  <fcntl.h>
#include  <unistd.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <errno.h>
#include <endian.h>
#include <linux/fb.h>


static          int             DispFile;
static unsigned char           *dbuf=NULL;
       struct fb_var_screeninfo var;
       struct fb_fix_screeninfo fix;


const UBYTE  PixelTab[] =
{
    0x00, // 000 00000000
    0xE0, // 001 11100000
    0x1C, // 010 00011100
    0xFC, // 011 11111100
    0x03, // 100 00000011
    0xE3, // 101 11100011
    0x1F, // 110 00011111
    0xFF  // 111 11111111
};

static void dLcdExec(LCD *pDisp)
{
  UBYTE   *pSrc;
  UBYTE   *pDst;
  ULONG   Pixels;
  UWORD   X;
  UWORD   Y;

  if (dbuf)
  {
    pSrc  =  (*pDisp).Lcd;
    pDst  =  dbuf;

    for (Y = 0;Y < 128;Y++)
    {
      for (X = 0;X < 7;X++)
      {
		//7*24Bits=168 Pixel konvertieren
        Pixels  =  (ULONG)*pSrc;
        pSrc++;
        Pixels |=  (ULONG)*pSrc << 8;
        pSrc++;
        Pixels |=  (ULONG)*pSrc << 16;
        pSrc++;

        *pDst   =  PixelTab[Pixels & 0x07];  //012
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //345
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //678
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //901
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //234
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //567
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //890
        pDst++;
        Pixels >>= 3;
        *pDst   =  PixelTab[Pixels & 0x07];  //123
        pDst++;
      }
      //7*24Bits=168 + 10 Pixel konvertieren
      Pixels  =  (ULONG)*pSrc;
      pSrc++;
      Pixels |=  (ULONG)*pSrc << 8;
      pSrc++;
      *pDst   =  PixelTab[Pixels & 0x07];    //012
      pDst++;
      Pixels >>= 3;
      *pDst   =  PixelTab[Pixels & 0x07];    //345
      pDst++;
      Pixels >>= 3;
      *pDst   =  PixelTab[Pixels & 0x07];    //678
      pDst++;
      Pixels >>= 3;
      *pDst   =  PixelTab[Pixels & 0x07];    //901
      pDst++;
    }
  }
}

void      dLcdUpdate(LCD *pDisp)
{
    dLcdExec(pDisp);
}


int dLcdInit(void)
{
  DispFile = open(LCD_DEVICE_NAME, O_RDWR);
  if (DispFile < 0) 
  {
	LogErrorNumber(LCD_DEVICE_FILE_NOT_FOUND);
	return -1;
  }

  if(ioctl(DispFile, _IOW('S',0, int), NULL) == -1)
  {
    LogErrorNumber(LCD_DEVICE_FILE_NOT_FOUND);
	close(DispFile);
	return -1;
  }
  if(ioctl(DispFile, FBIOGET_VSCREENINFO, &var) == -1) 
  {
    LogErrorNumber(LCD_DEVICE_FILE_NOT_FOUND);
	close(DispFile);
	return -1;
  }
  if(ioctl(DispFile, FBIOGET_FSCREENINFO, &fix) == -1) 
  {
    LogErrorNumber(LCD_DEVICE_FILE_NOT_FOUND);
	close(DispFile);
	return -1;
  }

  /* Display line length in bytes */
  //dll = fix.line_length;
  /* Image file line length in bytes */
  //fll = (var.xres >> 3) + 1;

  dbuf = (unsigned char *)mmap(0, var.yres * fix.line_length, PROT_WRITE | PROT_READ, MAP_SHARED, DispFile, 0);
  if (dbuf == MAP_FAILED) 
  {
    LogErrorNumber(LCD_DEVICE_FILE_NOT_FOUND);
	close(DispFile);
	return -1;
  }
  return 0;
}


UBYTE     dLcdRead(void)
{
  return (0);
}


void      dLcdExit(void)
{
  if (DispFile >= MIN_HANDLE)
  {
    close(DispFile);
  }
}

void      dLcdScroll(LCD *pDisp,DATA16 Y0)
{

  memmove(pDisp->Lcd,&pDisp->Lcd[((LCD_WIDTH + 7) / 8) * Y0],(LCD_HEIGHT - Y0) * ((LCD_WIDTH + 7) / 8));
  memset (&pDisp->Lcd[(LCD_HEIGHT - Y0) * ((LCD_WIDTH + 7) / 8)],0,((LCD_WIDTH + 7) / 8) * Y0);
}


void      dLcdDrawPixel(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0)
{
  if ((X0 >= 0) && (X0 < LCD_WIDTH) && (Y0 >= 0) && (Y0 < LCD_HEIGHT))
  {
    if (Color)
    {
      pDisp->Lcd[(X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3)]  |=  (1 << (X0 % 8));
    }
    else
    {
      pDisp->Lcd[(X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3)]  &= ~(1 << (X0 % 8));
    }
  }
}


void      dLcdInversePixel(LCD *pDisp,DATA16 X0,DATA16 Y0)
{
  if ((X0 >= 0) && (X0 < LCD_WIDTH) && (Y0 >= 0) && (Y0 < LCD_HEIGHT))
  {
    pDisp->Lcd[(X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3)]  ^=  (1 << (X0 % 8));
  }
}


static DATA8     dLcdReadPixel(LCD *pDisp,DATA16 X0,DATA16 Y0)
{
  DATA8   Result = 0;

  if ((X0 >= 0) && (X0 < LCD_WIDTH) && (Y0 >= 0) && (Y0 < LCD_HEIGHT))
  {
    if ((pDisp->Lcd[(X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3)] & (1 << (X0 % 8))))
    {
      Result  =  1;
    }
  }
  return (Result);
}


void      dLcdDrawLine(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1)
{
  DATA32  XLength;
  DATA32  YLength;
  DATA16  XInc;
  DATA16  YInc;
  DATA32  Diff;
  DATA32  Tmp;


  if (X0 < X1)
  {
    XLength  =  (DATA32)X1 - (DATA32)X0;
    XInc     =  1;
  }
  else
  {
    XLength  =  (DATA32)X0 - (DATA32)X1;
    XInc     = -1;
  }
  if (Y0 < Y1)
  {
    YLength  =  (DATA32)Y1 - (DATA32)Y0;
    YInc     =  1;
  }
  else
  {
    YLength  =  (DATA32)Y0 - (DATA32)Y1;
    YInc     = -1;
  }
  Diff  =  XLength - YLength;

  dLcdDrawPixel(pDisp,Color,X0,Y0);

  while ((X0 != X1) || (Y0 != Y1))
  {
    Tmp  =  Diff << 1;
    if (Tmp > (-YLength))
    {
      Diff -=  YLength;
      X0   +=  XInc;
    }
    if (Tmp < XLength)
    {
      Diff +=  XLength;
      Y0   +=  YInc;
    }
    dLcdDrawPixel(pDisp,Color,X0,Y0);
  }
}


void      dLcdDrawDotLine(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1,DATA16 On,DATA16 Off)
{
  DATA32  XLength;
  DATA32  YLength;
  DATA16  XInc;
  DATA16  YInc;
  DATA32  Diff;
  DATA32  Tmp;
  DATA16  Count;

  if ((X0 != X1) && (Y0 != Y1))
  {
    dLcdDrawLine(pDisp,Color,X0,Y0,X1,Y1);
  }
  else
  {
    if (On < 0)
    {
      On  =  0;
    }
    if (On > 255)
    {
      On  =  255;
    }
    if (Off < 0)
    {
      Off  =  0;
    }
    if (Off > 255)
    {
      Off  =  255;
    }

    if (X0 < X1)
    {
      XLength  =  (DATA32)X1 - (DATA32)X0;
      XInc     =  1;
    }
    else
    {
      XLength  =  (DATA32)X0 - (DATA32)X1;
      XInc     = -1;
    }
    if (Y0 < Y1)
    {
      YLength  =  (DATA32)Y1 - (DATA32)Y0;
      YInc     =  1;
    }
    else
    {
      YLength  =  (DATA32)Y0 - (DATA32)Y1;
      YInc     = -1;
    }
    Diff  =  XLength - YLength;

    dLcdDrawPixel(pDisp,Color,X0,Y0);
    Count  =  1;

    while ((X0 != X1) || (Y0 != Y1))
    {
      Tmp  =  Diff << 1;
      if (Tmp > (-YLength))
      {
        Diff -=  YLength;
        X0   +=  XInc;
      }
      if (Tmp < XLength)
      {
        Diff +=  XLength;
        Y0   +=  YInc;
      }
      if (Count < (On + Off))
      {
        if (Count < On)
        {
          dLcdDrawPixel(pDisp,Color,X0,Y0);
        }
        else
        {
          dLcdDrawPixel(pDisp,1 - Color,X0,Y0);
        }
      }
      Count++;
      if (Count >= (On + Off))
      {
        Count  =  0;
      }
    }
  }
}


static void      dLcdPlotPoints(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1)
{
  dLcdDrawPixel(pDisp,Color,X0 + X1,Y0 + Y1);
  dLcdDrawPixel(pDisp,Color,X0 - X1,Y0 + Y1);
  dLcdDrawPixel(pDisp,Color,X0 + X1,Y0 - Y1);
  dLcdDrawPixel(pDisp,Color,X0 - X1,Y0 - Y1);
  dLcdDrawPixel(pDisp,Color,X0 + Y1,Y0 + X1);
  dLcdDrawPixel(pDisp,Color,X0 - Y1,Y0 + X1);
  dLcdDrawPixel(pDisp,Color,X0 + Y1,Y0 - X1);
  dLcdDrawPixel(pDisp,Color,X0 - Y1,Y0 - X1);
}


void      dLcdDrawCircle(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 R)
{
  int     X = 0;
  int     Y = R;
  int     P = 3 - 2 * R;

  while (X<Y)
  {
    dLcdPlotPoints(pDisp,Color,X0,Y0,X,Y);
    if (P < 0)
    {
      P = P + 4 * X + 6;
    }
    else
    {
      P = P + 4 * (X - Y) + 10;
      Y = Y - 1;
    }
    X = X + 1;
  }
  dLcdPlotPoints(pDisp,Color,X0,Y0,X,Y);
}


typedef   struct
{
  const   char    *pFontBits;           // Pointer to start of font bitmap
  const   DATA16  FontHeight;           // Character height (all inclusive)
  const   DATA16  FontWidth;            // Character width (all inclusive)
  const   DATA16  FontHorz;             // Number of horizontal character in font bitmap
  const   DATA8   FontFirst;            // First character supported
  const   DATA8   FontLast;             // Last character supported
}
FONTINFO;


#include  "normal_font.xbm"
#include  "small_font.xbm"
#include  "large_font.xbm"
#include  "tiny_font.xbm"


FONTINFO  FontInfo[] =
{
  [NORMAL_FONT] = {
                    .pFontBits    = (const char*)normal_font_bits,
                    .FontHeight   = 9,
                    .FontWidth    = 8,
                    .FontHorz     = 16,
                    .FontFirst    = 0x20,
                    .FontLast     = 0x7F
                  },
  [SMALL_FONT] =  {
                    .pFontBits    = (const char*)small_font_bits,
                    .FontHeight   = 8,
                    .FontWidth    = 8,
                    .FontHorz     = 16,
                    .FontFirst    = 0x20,
                    .FontLast     = 0x7F
                  },
  [LARGE_FONT] =  {
                    .pFontBits    = (const char*)large_font_bits,
                    .FontHeight   = 16,
                    .FontWidth    = 16,
                    .FontHorz     = 16,
                    .FontFirst    = 0x20,
                    .FontLast     = 0x7F
                  },
  [TINY_FONT] =   {
                    .pFontBits    = (const char*)tiny_font_bits,
                    .FontHeight   = 7,
                    .FontWidth    = 5,
                    .FontHorz     = 16,
                    .FontFirst    = 0x20,
                    .FontLast     = 0x7F
                  },

};


DATA16    dLcdGetFontWidth(DATA8 Font)
{
  return (FontInfo[Font].FontWidth);
}


DATA16    dLcdGetFontHeight(DATA8 Font)
{
  return (FontInfo[Font].FontHeight);
}


void      dLcdDrawChar(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA8 Font,DATA8 Char)
{
  DATA16  CharWidth;
  DATA16  CharHeight;
  DATA16  CharByteIndex;
  DATA16  LcdByteIndex;
  UBYTE   CharByte;
  DATA16  Tmp,X,Y,TmpX,MaxX;


  CharWidth   =  FontInfo[Font].FontWidth;
  CharHeight  =  FontInfo[Font].FontHeight;

  if ((Char >= FontInfo[Font].FontFirst) && (Char <= FontInfo[Font].FontLast))
  {
    Char -=  FontInfo[Font].FontFirst;

    CharByteIndex  =  (Char % FontInfo[Font].FontHorz) * ((CharWidth + 7) / 8);
    CharByteIndex +=  ((Char / FontInfo[Font].FontHorz) * ((CharWidth + 7) / 8) * CharHeight * FontInfo[Font].FontHorz);

    if (((CharWidth % 8) == 0) && ((X0 % 8) == 0))
    { // Font aligned

      X0             =  (X0 >> 3) << 3;
      LcdByteIndex   =  (X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3);

      if (Color)
      {
        while (CharHeight)
        {
          Tmp  =  0;
          do
          {
            if (LcdByteIndex < sizeof(LCD))
            {
              pDisp->Lcd[LcdByteIndex + Tmp]  =  FontInfo[Font].pFontBits[CharByteIndex + Tmp];
            }
            Tmp++;
          }
          while (Tmp < (CharWidth / 8));

          CharByteIndex +=  (CharWidth * FontInfo[Font].FontHorz) / 8;
          LcdByteIndex  +=  ((LCD_WIDTH + 7) >> 3);
          CharHeight--;
        }
      }
      else
      {
        while (CharHeight)
        {
          Tmp  =  0;
          do
          {
            if (LcdByteIndex < sizeof(LCD))
            {
              pDisp->Lcd[LcdByteIndex + Tmp]  = ~FontInfo[Font].pFontBits[CharByteIndex + Tmp];
            }
            Tmp++;
          }
          while (Tmp < (CharWidth / 8));

          CharByteIndex +=  (CharWidth * FontInfo[Font].FontHorz) / 8;
          LcdByteIndex  +=  ((LCD_WIDTH + 7) >> 3);
          CharHeight--;
        }
      }
    }
    else
    { // Font not aligned

      MaxX          =  X0 + CharWidth;

      if (Color)
      {
        for (Y = 0;Y < CharHeight;Y++)
        {
          TmpX              =  X0;

          for (X = 0;X < ((CharWidth + 7) / 8);X++)
          {
            CharByte  =  FontInfo[Font].pFontBits[CharByteIndex + X];

            for (Tmp = 0;(Tmp < 8) && (TmpX < MaxX);Tmp++)
            {
              if (CharByte & 1)
              {
                dLcdDrawPixel(pDisp,1,TmpX,Y0);
              }
              else
              {
                dLcdDrawPixel(pDisp,0,TmpX,Y0);
              }
              CharByte >>= 1;
              TmpX++;
            }
          }
          Y0++;
          CharByteIndex +=  ((CharWidth + 7) / 8) * FontInfo[Font].FontHorz;

        }
      }
      else
      {
        for (Y = 0;Y < CharHeight;Y++)
        {
          TmpX              =  X0;

          for (X = 0;X < ((CharWidth + 7) / 8);X++)
          {
            CharByte  =  FontInfo[Font].pFontBits[CharByteIndex + X];

            for (Tmp = 0;(Tmp < 8) && (TmpX < MaxX);Tmp++)
            {
              if (CharByte & 1)
              {
                dLcdDrawPixel(pDisp,0,TmpX,Y0);
              }
              else
              {
                dLcdDrawPixel(pDisp,1,TmpX,Y0);
              }
              CharByte >>= 1;
              TmpX++;
            }
          }
          Y0++;
          CharByteIndex +=  ((CharWidth + 7) / 8) * FontInfo[Font].FontHorz;
        }
      }
    }
  }
}


void      dLcdDrawText(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA8 Font,DATA8 *pText)
{
  while (*pText)
  {
    if (X0 < (LCD_WIDTH - FontInfo[Font].FontWidth))
    {
      dLcdDrawChar(pDisp,Color,X0,Y0,Font,*pText);
      X0 +=  FontInfo[Font].FontWidth;
    }
    pText++;
  }
}


typedef   struct
{
  const   char    *pIconBits;
  const   DATA16  IconSize;
  const   DATA16  IconHeight;
  const   DATA16  IconWidth;
}
ICONINFO;

#include  "normal_icons.xbm"
#include  "small_icons.xbm"
#include  "large_icons.xbm"
#include  "menu_icons.xbm"
#include  "arrow_icons.xbm"


ICONINFO  IconInfo[] =
{
  [NORMAL_ICON] = {
                    .pIconBits    = normal_icons_bits,
                    .IconSize     = normal_icons_height,
                    .IconHeight   = 12,
                    .IconWidth    = normal_icons_width
                  },
  [SMALL_ICON] =  {
                    .pIconBits    = small_icons_bits,
                    .IconSize     = small_icons_height,
                    .IconHeight   = 8,
                    .IconWidth    = small_icons_width
                  },
  [LARGE_ICON] =  {
                    .pIconBits    = large_icons_bits,
                    .IconSize     = large_icons_height,
                    .IconHeight   = 22,
                    .IconWidth    = large_icons_width
                  },
  [MENU_ICON] =   {
                    .pIconBits    = menu_icons_bits,
                    .IconSize     = menu_icons_height,
                    .IconHeight   = 12,
                    .IconWidth    = menu_icons_width
                  },
  [ARROW_ICON] =  {
                    .pIconBits    = arrow_icons_bits,
                    .IconSize     = arrow_icons_height,
                    .IconHeight   = 12,
                    .IconWidth    = arrow_icons_width
                  },
};


UBYTE    *dLcdGetIconBits(DATA8 Type)
{
  UBYTE  *pResult;

  pResult  =  (UBYTE*)IconInfo[Type].pIconBits;

  return (pResult);
}


DATA16    dLcdGetIconWidth(DATA8 Type)
{
  return (IconInfo[Type].IconWidth);
}


DATA16    dLcdGetIconHeight(DATA8 Type)
{
  return (IconInfo[Type].IconHeight);
}


DATA16    dLcdGetNoOfIcons(DATA8 Type)
{
  return (IconInfo[Type].IconSize / IconInfo[Type].IconHeight);
}


void      dLcdDrawPicture(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 IconWidth,DATA16 IconHeight,UBYTE *pIconBits)
{
  DATA16  IconByteIndex;
  DATA16  LcdByteIndex;
  DATA16  Tmp;

  IconByteIndex  =  0;

  X0             =  (X0 >> 3) << 3;
  LcdByteIndex   =  (X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3);


  if (Color)
  {
    while (IconHeight)
    {
      for (Tmp = 0;Tmp < (IconWidth / 8);Tmp++)
      {
        pDisp->Lcd[LcdByteIndex + Tmp] =  pIconBits[IconByteIndex + Tmp];
      }

      IconByteIndex +=  IconWidth / 8;
      LcdByteIndex  +=  ((LCD_WIDTH + 7) >> 3);
      IconHeight--;
    }
  }
  else
  {
    while (IconHeight)
    {
      for (Tmp = 0;Tmp < (IconWidth / 8);Tmp++)
      {
        pDisp->Lcd[LcdByteIndex + Tmp] = ~pIconBits[IconByteIndex + Tmp];
      }

      IconByteIndex +=  IconWidth / 8;
      LcdByteIndex  +=  ((LCD_WIDTH + 7) >> 3);
      IconHeight--;
    }
  }
}


void      dLcdDrawIcon(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA8 Type,DATA8 No)
{
  DATA16  IconByteIndex;
  DATA16  IconHeight;
  DATA16  IconWidth;
  UBYTE   *pIconBits;

  IconHeight  =  dLcdGetIconHeight(Type);
  IconWidth   =  dLcdGetIconWidth(Type);

  if ((No >= 0) && (No <= dLcdGetNoOfIcons(Type)))
  {
    pIconBits      =  dLcdGetIconBits(Type);
    IconByteIndex  =  ((DATA16)No * IconWidth * IconHeight) / 8;
    dLcdDrawPicture(pDisp,Color,X0,Y0,IconWidth,IconHeight,&pIconBits[IconByteIndex]);
  }
}


void      dLcdGetBitmapSize(IP pBitmap,DATA16 *pWidth,DATA16 *pHeight)
{
  *pWidth     =  0;
  *pHeight    =  0;

  if (pBitmap)
  {

    *pWidth     =  (DATA16)pBitmap[0];
    *pHeight    =  (DATA16)pBitmap[1];
  }
}


void      dLcdDrawBitmap(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,IP pBitmap)
{
  DATA16  BitmapWidth;
  DATA16  BitmapHeight;
  DATA16  BitmapByteIndex;
  UBYTE   *pBitmapBytes;
  UBYTE   BitmapByte;
  DATA16  Tmp,X,Y,TmpX,MaxX;

  DATA16  LcdByteIndex;

  if (pBitmap)
  {

    BitmapWidth   =  (DATA16)pBitmap[0];
    BitmapHeight  =  (DATA16)pBitmap[1];
    MaxX          =  X0 + BitmapWidth;
    pBitmapBytes  =  &pBitmap[2];

    if ((BitmapWidth >=0) && (BitmapHeight >= 0))
    {
      if ((X0 % 8) || (BitmapWidth % 8))
      { // X is not aligned

        BitmapWidth       =  ((BitmapWidth + 7) >> 3) << 3;

        if (Color)
        {
          for (Y = 0;Y < BitmapHeight;Y++)
          {
            BitmapByteIndex   =  (Y * BitmapWidth) / 8;
            TmpX              =  X0;

            for (X = 0;X < (BitmapWidth / 8);X++)
            {
              BitmapByte  =  pBitmapBytes[BitmapByteIndex + X];

              for (Tmp = 0;(Tmp < 8) && (TmpX < MaxX);Tmp++)
              {
                if (BitmapByte & 1)
                {
                  dLcdDrawPixel(pDisp,1,TmpX,Y0);
                }
                else
                {
                  dLcdDrawPixel(pDisp,0,TmpX,Y0);
                }
                BitmapByte >>= 1;
                TmpX++;
              }
            }
            Y0++;
          }
        }
        else
        {
          for (Y = 0;Y < BitmapHeight;Y++)
          {
            BitmapByteIndex   =  (Y * BitmapWidth) / 8;
            TmpX              =  X0;

            for (X = 0;X < (BitmapWidth / 8);X++)
            {
              BitmapByte  =  pBitmapBytes[BitmapByteIndex + X];

              for (Tmp = 0;(Tmp < 8) && (TmpX < MaxX);Tmp++)
              {
                if (BitmapByte & 1)
                {
                  dLcdDrawPixel(pDisp,0,TmpX,Y0);
                }
                else
                {
                  dLcdDrawPixel(pDisp,1,TmpX,Y0);
                }
                BitmapByte >>= 1;
                TmpX++;
              }
            }
            Y0++;
          }
        }
      }

      else
      { // X is byte aligned

        BitmapByteIndex   =  0;

        LcdByteIndex      =  (X0 >> 3) + Y0 * ((LCD_WIDTH + 7) >> 3);

        if (Color)
        {
          while (BitmapHeight)
          {
            X  =  X0;
            for (Tmp = 0;Tmp < (BitmapWidth / 8);Tmp++)
            {
              if (((LcdByteIndex + Tmp) < LCD_BUFFER_SIZE) && (X < LCD_WIDTH) && (X >= 0))
              {
                pDisp->Lcd[LcdByteIndex + Tmp]  =  pBitmapBytes[BitmapByteIndex + Tmp];
              }
              X +=  8;
            }

            BitmapByteIndex +=  BitmapWidth / 8;
            LcdByteIndex    +=  ((LCD_WIDTH + 7) >> 3);

            BitmapHeight--;
          }
        }
        else
        {
          while (BitmapHeight)
          {
            X  =  X0;
            for (Tmp = 0;Tmp < (BitmapWidth / 8);Tmp++)
            {
              if (((LcdByteIndex + Tmp) < LCD_BUFFER_SIZE) && (X < LCD_WIDTH) && (X >= 0))
              {
                pDisp->Lcd[LcdByteIndex + Tmp]  = ~pBitmapBytes[BitmapByteIndex + Tmp];
              }
              X +=  8;
            }

            BitmapByteIndex +=  BitmapWidth / 8;
            LcdByteIndex    +=  ((LCD_WIDTH + 7) >> 3);

            BitmapHeight--;
          }
        }
      }

    }
  }
}


void      dLcdRect(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1)
{
  X1--;
  Y1--;
  dLcdDrawLine(pDisp,Color,X0,Y0,X0 + X1,Y0);
  dLcdDrawLine(pDisp,Color,X0 + X1,Y0,X0 + X1,Y0 + Y1);
  dLcdDrawLine(pDisp,Color,X0 + X1,Y0 + Y1,X0,Y0 + Y1);
  dLcdDrawLine(pDisp,Color,X0,Y0 + Y1,X0,Y0);
}


void      dLcdFillRect(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1)
{
  DATA16  StartX;
  DATA16  MaxX;
  DATA16  MaxY;

  StartX  =  X0;
  MaxX    =  X0 + X1;
  MaxY    =  Y0 + Y1;

  for (;Y0 < MaxY;Y0++)
  {
    for (X0 = StartX;X0 < MaxX;X0++)
    {
      dLcdDrawPixel(pDisp,Color,X0,Y0);
    }
  }
}


void      dLcdInverseRect(LCD *pDisp,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1)
{
  DATA16  StartX;
  DATA16  MaxX;
  DATA16  MaxY;

  StartX  =  X0;
  MaxX    =  X0 + X1;
  MaxY    =  Y0 + Y1;

  for (;Y0 < MaxY;Y0++)
  {
    for (X0 = StartX;X0 < MaxX;X0++)
    {
      dLcdInversePixel(pDisp,X0,Y0);
    }
  }
}


static void      dLcdPlotLines(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1)
{
  dLcdDrawLine(pDisp,Color,X0 - X1,Y0 + Y1,X0 + X1,Y0 + Y1);
  dLcdDrawLine(pDisp,Color,X0 - X1,Y0 - Y1,X0 + X1,Y0 - Y1);
  dLcdDrawLine(pDisp,Color,X0 - Y1,Y0 + X1,X0 + Y1,Y0 + X1);
  dLcdDrawLine(pDisp,Color,X0 - Y1,Y0 - X1,X0 + Y1,Y0 - X1);
}


void      dLcdDrawFilledCircle(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 R)
{
  int     X = 0;
  int     Y = R;
  int     P = 3 - 2 * R;

  while (X<Y)
  {
    dLcdPlotLines(pDisp,Color,X0,Y0,X,Y);
    if (P < 0)
    {
      P = P + 4 * X + 6;
    }
    else
    {
      P = P + 4 * (X - Y) + 10;
      Y = Y - 1;
    }
    X = X + 1;
  }
  dLcdPlotLines(pDisp,Color,X0,Y0,X,Y);
}


static DATA8     dLcdCheckPixel(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0)
{
  DATA8   Result = 0;

  if ((X0 >= 0) && (X0 < LCD_WIDTH) && (Y0 >= 0) && (Y0 < LCD_HEIGHT))
  {
    if (dLcdReadPixel(pDisp,X0,Y0) != Color)
    {
      Result  =  1;
    }
  }

  return (Result);
}


void      dLcdFlodfill(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0)
{
  DATA16  X;
  DATA16  Y;

  Y  =  Y0;
  X  =  X0;

  while (dLcdCheckPixel(pDisp,Color,X,Y))
  {
    while (dLcdCheckPixel(pDisp,Color,X,Y))
    {
      if (X != X0)
      {
        dLcdDrawPixel(pDisp,Color,X,Y);
      }
      X--;
    }
    X  =  X0;
    Y--;
  }

  Y  =  Y0;
  X  =  X0;

  while (dLcdCheckPixel(pDisp,Color,X,Y))
  {
    while (dLcdCheckPixel(pDisp,Color,X,Y))
    {
      if (X != X0)
      {
        dLcdDrawPixel(pDisp,Color,X,Y);
      }
      X--;
    }
    X  =  X0;
    Y++;
  }

  Y  =  Y0;
  X  =  X0;

  while (dLcdCheckPixel(pDisp,Color,X,Y))
  {
    while (dLcdCheckPixel(pDisp,Color,X,Y))
    {
      dLcdDrawPixel(pDisp,Color,X,Y);
      X++;
    }
    X  =  X0;
    Y--;
  }

  Y  =  Y0 + 1;
  X  =  X0;

  while (dLcdCheckPixel(pDisp,Color,X,Y))
  {
    while (dLcdCheckPixel(pDisp,Color,X,Y))
    {
      dLcdDrawPixel(pDisp,Color,X,Y);
      X++;
    }
    X  =  X0;
    Y++;
  }
}
