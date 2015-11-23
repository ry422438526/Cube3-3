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
const char PORT=0x0;
const char RGB_RAW=4;     //color mode
const char COLOR_SENSOR_TYPE=29;
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
    int i;
    int Color_data_r=0;
    int Color_data_g=0;
    int Color_data_b=0;
    Color_data_r=(unsigned char)pColorSensor->Raw[PORT][0][0]+(unsigned char)(pColorSensor->Raw[PORT][0][1]<<8);
    Color_data_g=(unsigned char)pColorSensor->Raw[PORT][0][2]+(unsigned char)(pColorSensor->Raw[PORT][0][3]<<8);
    Color_data_b=(unsigned char)pColorSensor->Raw[PORT][0][4]+(unsigned char)(pColorSensor->Raw[PORT][0][5]<<8);
    
    printf("COLOR_MAX is %d", MAX_Werte);
    printf("color_data is (%d, %d, %d) \n\r", Color_data_r,Color_data_g,Color_data_b);
    // usleep(10000);
    MAX_Werte=MAX_RGB(MAX_RGB(Color_data_r, Color_data_g),Color_data_b);
    rot   =(Color_data_r*1.0)/MAX_Werte;
    gruen =(Color_data_g*1.0)/MAX_Werte;
    blau  =(Color_data_b*1.0)/MAX_Werte;
    Color_color=Color_col(rot,gruen,blau);
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
        }else if (blau<0.1){
            Color_col=ROT;
        }else{
            Color_col=ORANGE;
        }
    }
    if(gruen==1)
    {
        if(rot<0.2)
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





