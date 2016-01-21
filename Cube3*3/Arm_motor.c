//
//  Arm_motor.c
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
#include "Arm_motor.h"


#ifdef __linux__
const int MOTOR_SPEED=20;//power:0~100
const char MOTOR_PORT_A = 0x01; //Ausfahrbarer Greifarm Motor;

OUTPUT_GLOBALS OutputInstance;

int arm_referenz=0;
int arm_ist=0;
const int arm_winkel[]={4,-83,-150,-240,-330};//-85   231   330
#endif


int arm_init()
{
#ifdef __linux__
    
    cOutputInit();
    cOutputSpeed(MOTOR_PORT_A,MOTOR_SPEED);
    cOutputStart(MOTOR_PORT_A);
    int old_TachoSensor;
    old_TachoSensor=cOutputGetCount(0);
    usleep(1000000);
    while(old_TachoSensor!=cOutputGetCount(0)){
        old_TachoSensor=cOutputGetCount(0);
        usleep(10000);
    }
    cOutputStop(MOTOR_PORT_A,1);
    arm_referenz=cOutputGetCount(0);
    arm_ist = 0;
    arm_setpos(4);
#endif
    return 0;
}




int arm_setpos(int arm_soll)
{
#ifdef __linux__
    if(arm_winkel[arm_ist]<arm_winkel[arm_soll]){
        cOutputSpeed(MOTOR_PORT_A,MOTOR_SPEED);
        cOutputStart(MOTOR_PORT_A);
        while ((cOutputGetCount(0)-arm_referenz) < arm_winkel[arm_soll])
        {
            usleep(10000);
        }
    }else{
        cOutputSpeed(MOTOR_PORT_A,-MOTOR_SPEED);
        cOutputStart(MOTOR_PORT_A);
        while ((cOutputGetCount(0)-arm_referenz) > arm_winkel[arm_soll])
        {
            usleep(10000);
        }
        
    }
    cOutputStop(MOTOR_PORT_A,1);
    arm_ist=arm_soll;
    usleep(100000);
#endif
    return 0;
}


int arm_close()
{
#ifdef __linux__
    cOutputExit();
#endif
    return 0;
}