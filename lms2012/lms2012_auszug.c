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
#include  <stdio.h>


#define   ERROR(ErrorCode)    [ErrorCode]  =  #ErrorCode

char      ErrString[ERRORS][ERR_STRING_SIZE] =
{

  ERROR(TOO_MANY_ERRORS_TO_BUFFER),
  ERROR(TYPEDATA_TABEL_FULL),
  ERROR(TYPEDATA_FILE_NOT_FOUND),
  ERROR(ANALOG_DEVICE_FILE_NOT_FOUND),
  ERROR(ANALOG_SHARED_MEMORY),
  ERROR(UART_DEVICE_FILE_NOT_FOUND),
  ERROR(UART_SHARED_MEMORY),
  ERROR(IIC_DEVICE_FILE_NOT_FOUND),
  ERROR(IIC_SHARED_MEMORY),
  ERROR(DISPLAY_SHARED_MEMORY),
  ERROR(UI_SHARED_MEMORY),
  ERROR(UI_DEVICE_FILE_NOT_FOUND),
  ERROR(LCD_DEVICE_FILE_NOT_FOUND),
  ERROR(OUTPUT_SHARED_MEMORY),
  ERROR(COM_COULD_NOT_OPEN_FILE),
  ERROR(COM_NAME_TOO_SHORT),
  ERROR(COM_NAME_TOO_LONG),
  ERROR(COM_INTERNAL),
  ERROR(VM_INTERNAL),
  ERROR(VM_PROGRAM_VALIDATION),
  ERROR(VM_PROGRAM_NOT_STARTED),
  ERROR(VM_PROGRAM_FAIL_BREAK),
  ERROR(VM_PROGRAM_INSTRUCTION_BREAK),
  ERROR(VM_PROGRAM_NOT_FOUND),
  ERROR(SOUND_DEVICE_FILE_NOT_FOUND),
  ERROR(SOUND_SHARED_MEMORY),
  ERROR(FILE_OPEN_ERROR),
  ERROR(FILE_READ_ERROR),
  ERROR(FILE_WRITE_ERROR),
  ERROR(FILE_CLOSE_ERROR),
  ERROR(FILE_GET_HANDLE_ERROR),
  ERROR(FILE_NAME_ERROR),
  ERROR(USB_SHARED_MEMORY),
  ERROR(OUT_OF_MEMORY),

};


void      LogErrorNumber(ERR Err)
{
#if 0  
  UBYTE   Tmp;

  if (Err > TOO_MANY_ERRORS_TO_BUFFER)
  {
    Tmp  =  VMInstance.ErrorIn;
    if (++Tmp >= ERROR_BUFFER_SIZE)
    {
      Tmp  =  0;
    }
    if (Tmp != VMInstance.ErrorOut)
    {
      VMInstance.Errors[VMInstance.ErrorIn]   =  Err;
      VMInstance.ErrorIn                      =  Tmp;
    }
    else
    {
      snprintf(VMInstance.PrintBuffer,PRINTBUFFERSIZE,"\r\n#### %s ####\r\n\n",ErrString[TOO_MANY_ERRORS_TO_BUFFER]);
      VmPrint(VMInstance.PrintBuffer);
    }
  }
#else
  printf("\r\n#### %s ####\r\n\n",ErrString[TOO_MANY_ERRORS_TO_BUFFER]);
#endif
	
}

