//
//  Dreh_motor.c
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
#include <unistd.h>
#endif
#include "Dreh_motor.h"

#ifdef __linux__
const int MOTOR_SPEED_B=35;
const char MOTOR_PORT_B=0x02; //Drehteller Motor;
OUTPUT_GLOBALS OutputInstance;
int Tel_ist=0;
int Tel_soll=0;
int Tel_referenz=0;
const int dreh_winkel[]={0*90*3,1*90*3,-1*90*3,2*90*3,-2*90*3};//274
const int dreh_Korr_Winkel=30;  //Korrigieren Winkel
#endif


int DrehTel_init()
{
#ifdef __linux__
    cOutputInit();
    Tel_referenz=cOutputGetCount(1);
    Tel_ist=0;
    DrehTel_setpos(0);
#endif
    return 0;
}

int DrehTel_setpos(int Tel_soll)
{
#ifdef __linux__
#if 1
    int Old_Tel_referenz=0;
    Old_Tel_referenz=cOutputGetCount(1);
    if (dreh_winkel[Tel_soll]<=0)
    {
        cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        while((cOutputGetCount(1)-Old_Tel_referenz) >= (dreh_winkel[Tel_soll]-dreh_Korr_Winkel))
        {
            usleep(1000);
        }
        cOutputStop(MOTOR_PORT_B,1);
        
        Tel_referenz=cOutputGetCount(1);
        
        cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        usleep(10000);
        while(cOutputGetCount(1)-Tel_referenz<dreh_Korr_Winkel)
        {
            usleep(100);
        }
        
    }
    else
        
    {
        cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        
        while((cOutputGetCount(1)-Old_Tel_referenz) < (dreh_winkel[Tel_soll]+dreh_Korr_Winkel))
        {
            usleep(1000);
        }
        
        cOutputStop(MOTOR_PORT_B,1);
        
        Tel_referenz=cOutputGetCount(1);
        
        cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        usleep(10000);
        while(cOutputGetCount(1)-Tel_referenz>-dreh_Korr_Winkel)
        {
            usleep(100);
        }
        
    }
    cOutputStop(MOTOR_PORT_B,1);
    while(cOutputGetCount(1)!=dreh_winkel[Tel_soll]+Old_Tel_referenz){
        if((cOutputGetCount(1)-(dreh_winkel[Tel_soll]+Old_Tel_referenz))<0){
            cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
            cOutputStart(MOTOR_PORT_B);
            while((cOutputGetCount(1)-(dreh_winkel[Tel_soll]+Old_Tel_referenz))<0)
            {
                usleep(10);
            }
            cOutputStop(MOTOR_PORT_B,1);
        }else{
            cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
            cOutputStart(MOTOR_PORT_B);
            while((cOutputGetCount(1)-(dreh_winkel[Tel_soll]+Old_Tel_referenz))>0)
            {
                usleep(10);
            }
            cOutputStop(MOTOR_PORT_B,1);
            
        }
    }

    Tel_ist=Tel_soll;
    Old_Tel_referenz=cOutputGetCount(1);
#else
    Tel_referenz=cOutputGetCount(1);
    cOutputStepSpeed(MOTOR_PORT_B,MOTOR_SPEED_B,((dreh_winkel[Tel_soll]+dreh_Korr_Winkel)/9),(((dreh_winkel[Tel_soll]+dreh_Korr_Winkel)/9)*7),((dreh_winkel[Tel_soll]+dreh_Korr_Winkel)/9),1);
    usleep(1000);
    cOutputStepSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B,(dreh_Korr_Winkel/10),((dreh_Korr_Winkel/10)*8),(dreh_Korr_Winkel/10),1);
    cOutputStop(MOTOR_PORT_B,1);
    while(cOutputGetCount(1)!=dreh_winkel[Tel_soll]+Tel_referenz){
        if((cOutputGetCount(1)-(dreh_winkel[Tel_soll]+Tel_referenz))<0){
            cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
            cOutputStart(MOTOR_PORT_B);
            while((cOutputGetCount(1)-(dreh_winkel[Tel_soll]+Tel_referenz))<0)
            {
                usleep(10);
            }
            cOutputStop(MOTOR_PORT_B,1);
        }else{
            cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
            cOutputStart(MOTOR_PORT_B);
            while((cOutputGetCount(1)-(dreh_winkel[Tel_soll]+Tel_referenz))>0)
            {
                usleep(10);
            }
            cOutputStop(MOTOR_PORT_B,1);
            
        }
    }
    Tel_ist=Tel_soll;
    Tel_referenz=cOutputGetCount(1);
    
#endif
   usleep(10000);
#endif
    return 0;
}
#ifdef __linux__
int DrehTel_dreh(){
    cOutputStepSpeed(MOTOR_PORT_B,MOTOR_SPEED_B, 15,105,15,1);
    return 0;
}


int DrehTel_close(){
    cOutputExit();
    return 0;
}
#endif