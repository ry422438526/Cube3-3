//
//  Farbe_motor.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#ifdef __linux__
#include "lms2012.h"
#include "../lms2012/c_output.h"
#endif
#include <unistd.h>
#include <stdlib.h>

#include "Farbe_motor.h"

#ifdef __linux__
const int MOTOR_SPEED_D=30; //power:0~100
const char MOTOR_PORT_D = 0x08; //Schwenkbarer Farbensensor Motor;
OUTPUT_GLOBALS OutputInstance;
int Farbe_referenz=0;
int Farbe_ist=0;
const int Farbe_winkel[]={4,-735,-645,-640,-610,-590,-400};
#endif

int Farbe_init()
{
#ifdef __linux__
    cOutputInit();
    cOutputSetType(3,TYPE_MINITACHO);
    cOutputSpeed(MOTOR_PORT_D,MOTOR_SPEED_D);
    cOutputStart(MOTOR_PORT_D);
    int old_TachoSensor;
    old_TachoSensor=cOutputGetCount(3);
    usleep(2*1000000);
    while(old_TachoSensor!=cOutputGetCount(3)){
        old_TachoSensor=cOutputGetCount(3);
        usleep(1000);
    }
    cOutputStop(MOTOR_PORT_D,1);
    Farbe_referenz=cOutputGetCount(3);
    Farbe_ist = 0;
#endif
    return 0;
}


int Farbe_setpos(int Farbe_soll)
{
#ifdef __linux__
    if(Farbe_winkel[Farbe_ist]<Farbe_winkel[Farbe_soll])
    {
        cOutputSpeed(MOTOR_PORT_D,MOTOR_SPEED_D);
        cOutputStart(MOTOR_PORT_D);
        
        while ((cOutputGetCount(3)-Farbe_referenz) < Farbe_winkel[Farbe_soll])
        {
            usleep(1000);
        }
    }
    else
    {
        cOutputSpeed(MOTOR_PORT_D,-MOTOR_SPEED_D);
        cOutputStart(MOTOR_PORT_D);
   
        while ((cOutputGetCount(3)-Farbe_referenz) > Farbe_winkel[Farbe_soll])
        {
            usleep(1000);
        }
    }
    cOutputStop(MOTOR_PORT_D,1);
    Farbe_ist=Farbe_soll;
#endif
    return 0;
}


int Farbe_close()
{
#ifdef __linux__
    cOutputExit();
#endif
    return 0;
}


