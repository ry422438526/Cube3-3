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
#if 0
MOTORDATA *pMotorData;
int motor_file;
int encoder_file;
#else
OUTPUT_GLOBALS OutputInstance;
#endif
int Tel_ist=0;
int Tel_referenz=0;
const int dreh_winkel[]={0*90*3,1*90*3,-1*90*3,2*90*3,-2*90*3};//274
const int dreh_Korr_Winkel=10;  //Korrigieren Winkel
#endif


int DrehTel_init()
{
// /Users/yangchunxia/Documents/Cube3*3/lms2012/c_output.h
#ifdef __linux__
#if 0
    if ((motor_file = open(PWM_DEVICE_NAME, O_WRONLY))== -1)
    {
        printf("Failed to open device\n");
        return -1;
    }
    if((encoder_file = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
        return -1;
    pMotorData = (MOTORDATA*)mmap (0, sizeof(MOTORDATA)*vmOUTPUTS, PROT_READ|PROT_WRITE,MAP_FILE|MAP_SHARED,    encoder_file, 0);
    if (pMotorData == MAP_FAILED)
    {
        printf("Map failed\n");
        return -1;
    }
#else
    cOutputInit();
#endif
    Tel_referenz=cOutputGetCount(1);
    printf("DrehTeller_referenz Position angefahren: %d\n\r",Tel_referenz);
    Tel_ist=0;
    DrehTel_setpos(0);
#endif
    return 0;
}

















int DrehTel_setpos(int Tel_soll)
{
#ifdef __linux__
#if 0
    char motor_command[4];
    
    motor_command[0]=opOUTPUT_SPEED;
    motor_command[1]=MOTOR_PORT_B;
    if (dreh_winkel[Tel_soll]<=0)
    {
        motor_command[2]=-MOTOR_SPEED_B;
        write(motor_file,motor_command,3);    //Motor_speed Wert geben
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_B;
        write(motor_file,motor_command,2);    //Motor  srarten
        while((pMotorData[1].TachoSensor-Tel_referenz) >= (dreh_winkel[Tel_soll]-30))  //40
        {
            usleep(1000);
        }
        motor_command[0]=opOUTPUT_STOP;
        motor_command[1]=MOTOR_PORT_B;
        motor_command[2]=1;
        write(motor_file,motor_command,3);
        
        Tel_referenz=pMotorData[1].TachoSensor;
        
        motor_command[0]=opOUTPUT_SPEED;
        motor_command[1]=MOTOR_PORT_B;
        motor_command[2]=MOTOR_SPEED_B;
        write(motor_file,motor_command,3);
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_B;
        write(motor_file,motor_command,2);
        
        while(pMotorData[1].TachoSensor-Tel_referenz<30)
        {
            usleep(100);
        }
        
    }
    else
        
    {
        motor_command[2]=MOTOR_SPEED_B;
        write(motor_file,motor_command,3);    //Motor_speed Wert geben
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_B;
        write(motor_file,motor_command,2);    //Motor  srarten
        
        while((pMotorData[1].TachoSensor-Tel_referenz) < (dreh_winkel[Tel_soll]+30))
        {
            usleep(1000);
        }
        
        motor_command[0]=opOUTPUT_STOP;
        motor_command[1]=MOTOR_PORT_B;
        motor_command[2]=1;
        write(motor_file,motor_command,3);
        
        Tel_referenz=pMotorData[1].TachoSensor;
        
        motor_command[0]=opOUTPUT_SPEED;
        motor_command[1]=MOTOR_PORT_B;
        motor_command[2]=-MOTOR_SPEED_B;
        write(motor_file,motor_command,3);
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_B;
        write(motor_file,motor_command,2);
        
        while(pMotorData[1].TachoSensor-Tel_referenz>-30)
        {
            usleep(100);
        }
        
        
    }
    motor_command[0]=opOUTPUT_STOP;
    motor_command[1]=MOTOR_PORT_B;
    motor_command[2]=1;
    write(motor_file,motor_command,3);  //Motor stoppen
#else
    Tel_referenz=cOutputGetCount(1);
    if (dreh_winkel[Tel_soll]<=0)
    {
        cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        while((cOutputGetCount(1)-Tel_referenz) >= (dreh_winkel[Tel_soll]-30))
        {
            usleep(1000);
        }
        cOutputStop(MOTOR_PORT_B,1);
        
        Tel_referenz=cOutputGetCount(1);
        
        cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        usleep(10000);
        while(cOutputGetCount(1)-Tel_referenz<30)
        {
            usleep(100);
        }
        
    }
    else
        
    {
        cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        
        while((cOutputGetCount(1)-Tel_referenz) < (dreh_winkel[Tel_soll]+30))
        {
            usleep(1000);
        }
        
        cOutputStop(MOTOR_PORT_B,1);
        
        Tel_referenz=cOutputGetCount(1);
        
        cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        usleep(10000);
        while(cOutputGetCount(1)-Tel_referenz>-30)
        {
            usleep(100);
        }
        
    }
    cOutputStop(MOTOR_PORT_B,1);/*
    Tel_referenz=cOutputGetCount(1);
    if (dreh_winkel[Tel_soll]<=0)
    {
        cOutputSpeed(MOTOR_PORT_B,-MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        while((cOutputGetCount(1)-Tel_referenz) >= dreh_winkel[Tel_soll])
        {
            usleep(1000);
        }
        
    }
    else
        
    {
        cOutputSpeed(MOTOR_PORT_B,MOTOR_SPEED_B);
        cOutputStart(MOTOR_PORT_B);
        
        while((cOutputGetCount(1)-Tel_referenz) < dreh_winkel[Tel_soll])
        {
            usleep(1000);
        }
    }
    cOutputStop(MOTOR_PORT_B,1);*/
#endif
    Tel_ist=Tel_soll;
    Tel_referenz=cOutputGetCount(1);
    
    printf("Tel_referenz:%d \n\r",Tel_referenz);
    printf("Tel_ist: %d\n\r",Tel_ist);
    sleep(1);
#endif
    return 0;
}
#ifdef __linux__
int DrehTel_dreh(){
#if 0
    char motor_command[4];
    motor_command[0]=opOUTPUT_SPEED;
    motor_command[1]=MOTOR_PORT_B;
    motor_command[2]=MOTOR_SPEED_B;
    write(motor_file,motor_command,3);
    
    motor_command[0]=opOUTPUT_START;
    motor_command[1]=MOTOR_PORT_B;
    write(motor_file,motor_command,2);
    
    int oldTachosensor=OutputInstance.MotorData[1].TachoSensor;
    while (cOutputGetCount(3).TachoSensor-oldTachosensor<135) {
        usleep(100);
    }
    motor_command[0]=opOUTPUT_STOP;
    motor_command[1]=MOTOR_PORT_B;
    motor_command[2]=1;  //break
    write(motor_file,motor_command,3);
#else
    cOutputStepSpeed(MOTOR_PORT_B,MOTOR_SPEED_B, 15,105,15,1);
#endif
    
    return 0;
}


int DrehTel_close(){
#if 0
    char motor_command[4];
    motor_command[0] = opOUTPUT_STOP;
    motor_command[1] = MOTOR_PORT_B;
    motor_command[2] = 0;
    write(motor_file,motor_command,3);
    close(encoder_file);
    close(motor_file);
#else
    cOutputExit();
#endif
    return 0;
}
#endif