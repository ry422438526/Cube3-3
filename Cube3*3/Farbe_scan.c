//
//  Farbe_scan.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#ifdef __linux__
#include "lms2012.h"
#include <unistd.h>
#include "Farbe_scan.h"
#include "Farbe_motor.h"
#include "Farbe_sen.h"
#include "Arm_motor.h"
#include "Dreh_motor.h"
#endif
#include "main.h"

const char Color_position[6][9]=
{
    {4,2,1,0,3,6,7,8,5},
    {4,0,3,6,7,8,5,2,1},
    {4,6,7,8,5,2,1,0,3},
    {4,6,5,2,1,0,3,6,7},
    {4,6,7,8,5,2,1,0,3},
    {4,2,1,0,3,6,7,8,5}
};
int Center[]={0,5,2,3,4,1};

int farbe_scan()
{
#ifdef __linux__
    int i,j;
    arm_setpos(4);
    for (i=0; i<=2; i++)
    {
        Farbe_setpos(1);
        usleep(100000);
        color_data[Center[i]][(int)Color_position[Center[i]][0]]=ColSen_getData();
        usleep(100000);
        DrehTel_dreh();
        Farbe_setpos(3);
        sleep(1);
        for(j=1;j<=7;j++)
        {
            if (j%2 != 0){
                usleep(1000);
                Farbe_setpos(2);
                usleep(100000);
                color_data[Center[i]][(int)Color_position[Center[i]][j]]=ColSen_getData();
                usleep(10000);
                DrehTel_dreh();
                sleep(1);
            }
            else
            {
                usleep(1000);
                Farbe_setpos(3);
                usleep(100000);
                color_data[Center[i]][(int)Color_position[Center[i]][j]]=ColSen_getData();
                usleep(10000);
                DrehTel_dreh();
                sleep(1);
            }
        }
        Farbe_setpos(4);
        usleep(10000);
        arm_setpos(3);
        arm_setpos(2);
        arm_setpos(4);
    }
    
    usleep(10000);
    DrehTel_setpos(1);
    usleep(10000);
    arm_setpos(3);
    arm_setpos(2);
    arm_setpos(4);
    DrehTel_setpos(3);

    
    for (i=3; i<=5; i++) {
        Farbe_setpos(1);
        usleep(100000);
        color_data[Center[i]][(int)Color_position[Center[i]][0]]=ColSen_getData();
        usleep(100000);
        Farbe_setpos(3);
        DrehTel_dreh();
        sleep(1);
        for(j=1;j<=7;j++)
        {
            if (j%2 != 0){
                usleep(1000);
                Farbe_setpos(2);
                usleep(100000);
                color_data[Center[i]][(int)Color_position[Center[i]][j]]=ColSen_getData();
                usleep(10000);
                DrehTel_dreh();
                sleep(1);
            }
            else
            {
                usleep(1000);
                Farbe_setpos(3);
                usleep(100000);
                color_data[Center[i]][(int)Color_position[Center[i]][j]]=ColSen_getData();
                usleep(10000);
                DrehTel_dreh();
                sleep(1);
            }
        }
        Farbe_setpos(4);
        arm_setpos(3);
        arm_setpos(2);
        arm_setpos(4);
    }
    
    return color_data[6][9];
#endif
    
#ifndef __linux__
    return 0;
#endif
}




