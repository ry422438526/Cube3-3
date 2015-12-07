//
//  Farbe_sen.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include "Farbe_sen.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#ifdef __linux__
#include "lms2012.h"
#include <unistd.h>
#endif
#include "main.h"
typedef enum
{
    WEISS=0,
    BLAU=1,
    GELB=2,
    GRUEN=3,
    ROT=4,
    ORANGE=5
}FABER;

#ifdef __linux__
 int  PORT=0x0;
const DATA8 RGB_RAW=4;     //color mode
const DATA8 COLOR_SENSOR_TYPE=29;
const int MAX_SAMPLES =10;
int file;
UART *pColorSensor;
DEVCON DevCon;   //Configuration parameters
#endif

#ifdef __linux__
int ColSen_init()
{

    if((file = open(UART_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
    {
        printf("Failed to open device\n");
        return -1;
    }
    
    pColorSensor =(UART*)mmap(0, sizeof(UART),PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED,file,0);
    
    if (pColorSensor == MAP_FAILED)
    {
        printf("Failed to map Color Sensor device\n");
        return -1;
    }
    DevCon.Mode[PORT]=RGB_RAW;      //DeviceConnection
    DevCon.Connection[PORT]=CONN_INPUT_UART;   //Universal Asynchronous Receiver/Transmitter
    DevCon.Type[PORT]=COLOR_SENSOR_TYPE;
    ioctl(file,UART_SET_CONN,&DevCon);
    printf("Device is ready \n");
    return 0;
}
#endif

int ColSen_getData()   //Center:zenteral Position
{
#ifdef __linux__
    int MAX_Werte=0;
    float rot,gruen,blau;
    int Color_color;
    int Color_data_r=0;
    int Color_data_g=0;
    int Color_data_b=0;
    Color_data_r=(unsigned char)pColorSensor->Raw[PORT][0][0]+(unsigned char)(pColorSensor->Raw[PORT][0][1]<<8);
    Color_data_g=(unsigned char)pColorSensor->Raw[PORT][0][2]+(unsigned char)(pColorSensor->Raw[PORT][0][3]<<8);
    Color_data_b=(unsigned char)pColorSensor->Raw[PORT][0][4]+(unsigned char)(pColorSensor->Raw[PORT][0][5]<<8);
    //Color_data_blank=(unsigned int)pColorSensor->Raw[PORT][0][6]+(unsigned int)(pColorSensor->Raw[PORT][0][7]<<8);
    // usleep(10000);
    MAX_Werte=MAX_RGB(MAX_RGB(Color_data_r, Color_data_g),Color_data_b);
    rot   =(Color_data_r*1.0)/MAX_Werte;
    gruen =(Color_data_g*1.0)/MAX_Werte;
    blau  =(Color_data_b*1.0)/MAX_Werte;
    Color_color=Color_col(rot,gruen,blau);
    printf("COLOR_MAX is %d", MAX_Werte);
    printf("color_data is (%d, %d, %d)", Color_data_r,Color_data_g,Color_data_b);
    switch(Color_color){
        case 0:
            printf("\033[1;30m   %d\033[0m",Color_color);
            break;
            
        case 1:
            printf("\033[1;34m   %d\033[0m",Color_color);
            break;
            
        case 2:
            printf("\033[1;33m   %d\033[0m",Color_color);
            break;
            
        case 3:
            printf("\033[1;32m   %d\033[0m",Color_color);
            break;
            
        case 4:
            printf("\033[1;35m   %d\033[0m",Color_color);
            break;
            
        case 5:
            printf("\033[1;31m   %d\033[0m",Color_color);
            break;
    }
    printf("\n\r");
    return Color_color;
    // senden daten zu PC
#endif
    
#ifndef __linux__
    return 0;
#endif
}

int ColSen_close()
{
    printf("Close the Color Sensor");
#ifdef __linux__
    close(file);
#endif
    return 0;
}


int MAX_RGB(int a,int b)
{
    if (a>=b) {
        return a;
    }else{
        return b;
    }
}

FARBE Color_col(float rot, float gruen,float blau)
{
    
    int Color_col=0;
    if (rot==1) {
        if(gruen>0.5)
        {
            Color_col=GELB;
        }else if ((gruen<0.3)||(blau<0.1)){
            Color_col=ROT;
        }else{
            Color_col=ORANGE;
        }
    }
    if(gruen==1)
    {
        if(rot<0.3)
        {
            Color_col=GRUEN;
        }else if (blau>0.8){
            Color_col=BLAU;
        }else{
            Color_col=WEISS;
        }
    }
    return Color_col;
}




/*
DATAF     cInputCalculateColor(COLORSTRUCT *pC)
{
    DATAF   Result ;
    
    
    Result  =  DATAF_NAN;
    
    // Color Sensor values has been calculated -
    // now calculate the color and put it in Sensor value
    if (((pC->SensorRaw[RED]) > (pC->SensorRaw[BLUE] )) &&
        ((pC->SensorRaw[RED]) > (pC->SensorRaw[GREEN])))
    {
        
        // If all 3 colors are less than 65 OR (Less that 110 and bg less than 40)
        if (((pC->SensorRaw[RED])   < 65) ||
            (((pC->SensorRaw[BLANK]) < 40) && ((pC->SensorRaw[RED])  < 110)))
        {
            Result  =  (DATAF)BLACKCOLOR;
        }
        else
        {
            if (((((pC->SensorRaw[BLUE]) >> 2)  + ((pC->SensorRaw[BLUE]) >> 3) + (pC->SensorRaw[BLUE])) < (pC->SensorRaw[GREEN])) &&
                ((((pC->SensorRaw[GREEN]) << 1)) > (pC->SensorRaw[RED])))
            {
                Result  =  (DATAF)YELLOWCOLOR;
            }
            else
            {
                
                if ((((pC->SensorRaw[GREEN]) << 1) - ((pC->SensorRaw[GREEN]) >> 2)) < (pC->SensorRaw[RED]))
                {
                    
                    Result  =  (DATAF)REDCOLOR;
                }
                else
                {
                    
                    if ((((pC->SensorRaw[BLUE]) < 70) ||
                         ((pC->SensorRaw[GREEN]) < 70)) ||
                        (((pC->SensorRaw[BLANK]) < 140) && ((pC->SensorRaw[RED]) < 140)))
                    {
                        Result  =  (DATAF)BLACKCOLOR;
                    }
                    else
                    {
                        Result  =  (DATAF)WHITECOLOR;
                    }
                }
            }
        }
    }
    else
    {
        
        // Red is not the dominant color
        if ((pC->SensorRaw[GREEN]) > (pC->SensorRaw[BLUE]))
        {
            
            // Green is the dominant color
            // If all 3 colors are less than 40 OR (Less that 70 and bg less than 20)
            if (((pC->SensorRaw[GREEN])  < 40) ||
                (((pC->SensorRaw[BLANK]) < 30) && ((pC->SensorRaw[GREEN])  < 70)))
            {
                Result  =  (DATAF)BLACKCOLOR;
            }
            else
            {
                if ((((pC->SensorRaw[BLUE]) << 1)) < (pC->SensorRaw[RED]))
                {
                    Result  =  (DATAF)YELLOWCOLOR;
                }
                else
                {
                    if ((((pC->SensorRaw[RED]) + ((pC->SensorRaw[RED])>>2)) < (pC->SensorRaw[GREEN])) ||
                        (((pC->SensorRaw[BLUE]) + ((pC->SensorRaw[BLUE])>>2)) < (pC->SensorRaw[GREEN])))
                    {
                        Result  =  (DATAF)GREENCOLOR;
                    }
                    else
                    {
                        if ((((pC->SensorRaw[RED]) < 70) ||
                             ((pC->SensorRaw[BLUE]) < 70)) ||
                            (((pC->SensorRaw[BLANK]) < 140) && ((pC->SensorRaw[GREEN]) < 140)))
                        {
                            Result  =  (DATAF)BLACKCOLOR;
                        }
                        else
                        {
                            Result  =  (DATAF)WHITECOLOR;
                        }
                    }
                }
            }
        }
        else
        {
            
            // Blue is the most dominant color
            // Colors can be blue, white or black
            // If all 3 colors are less than 48 OR (Less that 85 and bg less than 25)
            if (((pC->SensorRaw[BLUE])   < 48) ||
                (((pC->SensorRaw[BLANK]) < 25) && ((pC->SensorRaw[BLUE])  < 85)))
            {
                Result  =  (DATAF)BLACKCOLOR;
            }
            else
            {
                if ((((((pC->SensorRaw[RED]) * 48) >> 5) < (pC->SensorRaw[BLUE])) &&
                     ((((pC->SensorRaw[GREEN]) * 48) >> 5) < (pC->SensorRaw[BLUE])))
                    ||
                    (((((pC->SensorRaw[RED])   * 58) >> 5) < (pC->SensorRaw[BLUE])) ||
                     ((((pC->SensorRaw[GREEN]) * 58) >> 5) < (pC->SensorRaw[BLUE]))))
                {
                    Result  =  (DATAF)BLUECOLOR;
                }
                else
                {
                    
                    // Color is white or Black
                    if ((((pC->SensorRaw[RED])  < 60) ||
                         ((pC->SensorRaw[GREEN]) < 60)) ||
                        (((pC->SensorRaw[BLANK]) < 110) && ((pC->SensorRaw[BLUE]) < 120)))
                    {
                        Result  =  (DATAF)BLACKCOLOR;
                    }
                    else
                    {
                        if ((((pC->SensorRaw[RED])  + ((pC->SensorRaw[RED])   >> 3)) < (pC->SensorRaw[BLUE])) ||
                            (((pC->SensorRaw[GREEN]) + ((pC->SensorRaw[GREEN]) >> 3)) < (pC->SensorRaw[BLUE])))
                        {
                            Result  =  (DATAF)BLUECOLOR;
                        }
                        else
                        {
                            Result  =  (DATAF)WHITECOLOR;
                        }
                    }
                }
            }
        }
    }
    
    
    return (Result);
}
*/


