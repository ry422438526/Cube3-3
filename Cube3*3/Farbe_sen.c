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
}Farbe;

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
    printf("Device is ready \n\r");
    return 0;
}
#endif

int ColSen_getData()   //Center:zenteral Position
{
#ifdef __linux__
    int Color_color =0;
    int Color_data_r=0;
    int Color_data_g=0;
    int Color_data_b=0;
    int Color_data_blank=0;
    usleep(10000);
    sleep(1);
    Color_data_r=(unsigned char)pColorSensor->Raw[PORT][0][0]+(unsigned char)(pColorSensor->Raw[PORT][0][1]<<8);
    Color_data_g=(unsigned char)pColorSensor->Raw[PORT][0][2]+(unsigned char)(pColorSensor->Raw[PORT][0][3]<<8);
    Color_data_b=(unsigned char)pColorSensor->Raw[PORT][0][4]+(unsigned char)(pColorSensor->Raw[PORT][0][5]<<8);
    Color_data_blank=(unsigned int)pColorSensor->Raw[PORT][0][6]+(unsigned int)(pColorSensor->Raw[PORT][0][7]<<8);
    usleep(100000);
    Color_color=Color_col(Color_data_r*1.0,Color_data_g*1.0,Color_data_b*1.0);
    printf("color_data is (%5d, %5d, %5d, %5d)",Color_data_r,Color_data_g,Color_data_b,Color_data_blank);

#if 1
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
            printf("\033[1;31m   %d\033[0m",Color_color);
            break;
            
        case 5:
            printf("\033[1;35m   %d\033[0m",Color_color);
            break;
                    
        default:
            printf(" x ");
            break;
    }
#endif
    printf("\n\r");
    return Color_color;
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

FARBE Color_col(float rot, float gruen,float blau)
{
    int Farbe=-1;
    
    if ((rot>=20)||(gruen>=20)||(blau>=20)){
        if ((rot>blau*1.35)&&(gruen>blau*1.35))
        {
            if (((rot>gruen)&&(rot<gruen*1.1))||((gruen>=rot)&&(gruen<rot*1.1))){     //original werte 1.2
            Farbe= WEISS;
            goto fertig;
                
            }
        }
        if ((rot>2.5*gruen)&&(rot>4*blau)&&(gruen>2.1*blau)&&(blau>=10)){  //original grune>2.1*blue   1.9
            Farbe=  ORANGE;
            goto fertig;
        }
        if ((rot>2*gruen)&&(rot>3*blau)) {
            Farbe=  ROT;
            goto fertig;
            }
        if ((gruen>=2*rot)&&(gruen>=2*blau)) {
            Farbe=  GRUEN;
            goto fertig;
            }
        if (((blau>gruen)&&(blau<2*gruen)&&(blau>1.5*rot)&&(blau<2.5*rot))||((gruen>=blau)&&(gruen<2*blau)&&(gruen>1.5*rot)&&(gruen<2.5*rot))) {
            Farbe=  BLAU;
            goto fertig;
        }
        if(((rot>gruen/0.75)&&(rot<gruen/0.55))||((rot>blau/0.25)&&(rot<blau/0.15))){
            Farbe=  GELB;
            goto fertig;
        }
    }
    fertig:return Farbe;
}