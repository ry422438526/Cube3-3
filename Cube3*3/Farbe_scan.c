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
#include "print_mode.h"
#include "Side_dreh.h"

int farbe_scan()
{
#ifdef __linux__
    int i;
    arm_setpos(4);
    for (i=0; i<=2; i++) {
        Farbe_setpos(1);
        color_data[0][4]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][2]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][1]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][0]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][3]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][6]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][7]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][8]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][5]=ColSen_getData();
        Farbe_setpos(6);
        usleep(10000);
        wuerfel_print(1);
        klappen();
        arm_setpos(3);
        arm_setpos(4);
    }
    usleep(100000);
    drehen(-1);
    usleep(100000);
    klappen();
    usleep(100000);
    drehen(-1);
    drehen(-1);
    arm_setpos(3);
    arm_setpos(4);
    usleep(100000);
    for (i=3; i<=5; i++) {
        Farbe_setpos(1);
        color_data[0][4]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][2]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][1]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][0]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][3]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][6]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][7]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(5);
        color_data[0][8]=ColSen_getData();
        DrehTel_dreh();
        Farbe_setpos(3);
        color_data[0][5]=ColSen_getData();
        Farbe_setpos(6);
        wuerfel_print(1);
        usleep(10000);
        klappen();
        arm_setpos(3);
        arm_setpos(4);
    }
    return color_data[6][9];
#endif
#ifndef __linux__
    return 0;
#endif
}

 
 
 



