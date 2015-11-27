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


#ifndef D_LCD_H_
#define D_LCD_H_

void      dLcdUpdate(LCD *pLcd);

int       dLcdInit(void);

UBYTE     dLcdRead(void);

void      dLcdExit(void);

void      dLcdScroll(LCD *pDisp,DATA16 Y0);

void      dLcdDrawPixel(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0);

void      dLcdInversePixel(LCD *pDisp,DATA16 X0,DATA16 Y0);

void      dLcdDrawLine(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1);

void      dLcdDrawDotLine(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1,DATA16 On,DATA16 Off);

void      dLcdRect(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1);

void      dLcdFillRect(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1);

void      dLcdInverseRect(LCD *pDisp,DATA16 X0,DATA16 Y0,DATA16 X1,DATA16 Y1);

void      dLcdDrawCircle(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 R);

DATA16    dLcdGetFontWidth(DATA8 Font);

DATA16    dLcdGetFontHeight(DATA8 Font);

void      dLcdDrawChar(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA8 Font,DATA8 Char);

void      dLcdDrawText(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA8 Font,DATA8 *pText);

DATA16    dLcdGetIconWidth(DATA8 Type);

DATA16    dLcdGetIconHeight(DATA8 Type);

void      dLcdDrawPicture(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 IconWidth,DATA16 IconHeight,UBYTE *pIconBits);

void      dLcdDrawIcon(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA8 Type,DATA8 No);

void      dLcdGetBitmapSize(IP pBitmap,DATA16 *pWidth,DATA16 *pHeight);

void      dLcdDrawBitmap(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,IP pBitmap);

void      dLcdDrawFilledCircle(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0,DATA16 R);


void      dLcdFlodfill(LCD *pDisp,DATA8 Color,DATA16 X0,DATA16 Y0);

#define   LCDClear(I)                   memset((I)->Lcd,0 - BG_COLOR,LCD_BUFFER_SIZE)

#define   LCDClearTopline(I)            memset((I)->Lcd,0 - BG_COLOR,LCD_TOPLINE_SIZE)

#define   LCDErase(I)                   memset(&((I)->Lcd[LCD_TOPLINE_SIZE]),0,LCD_BUFFER_SIZE - LCD_TOPLINE_SIZE)

#define   LCDCopy(S,D,L)                memcpy((void*)((D)->Lcd),(const void*)((S)->Lcd),L)  // Copy S to D

#endif /* D_LCD_H_ */
