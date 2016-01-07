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

#if 0
MOTORDATA *pMotorData;
int motor_file;
int encoder_file;
#else
OUTPUT_GLOBALS OutputInstance;
#endif

int arm_referenz=0;
int arm_ist=0;
const int arm_winkel[]={4,-83,-150,-240,-330};//-85   231   330
#endif


int arm_init()
{
#ifdef __linux__
/*
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
    char motor_command[4];
    motor_command[0]=opOUTPUT_SPEED;
    motor_command[1]=MOTOR_PORT_A;
    motor_command[2]=MOTOR_SPEED;
    write(motor_file,motor_command,3);// motor_speed Wert geben
    
    motor_command[0]=opOUTPUT_START;
    motor_command[1]=MOTOR_PORT_A;
    write(motor_file,motor_command,2);// Motor starten
    
    int old_TachoSensor;
    old_TachoSensor=pMotorData[0].TachoSensor;
    
    usleep(1000000);
    while(old_TachoSensor!=pMotorData[0].TachoSensor){
        old_TachoSensor=pMotorData[0].TachoSensor;
        usleep(10000);
    }
    
    motor_command[0]=opOUTPUT_STOP;
    motor_command[1]=MOTOR_PORT_A;
    motor_command[2]=1;
    write(motor_file,motor_command,3);  // Motor stoppen
*/
    
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
    printf("Arm_referenzPosition angefahren %d\n\r",arm_referenz);
#endif
    return 0;
}




int arm_setpos(int arm_soll)
{
#ifdef __linux__
#if 0
    char motor_command[4];
    
    motor_command[0]=opOUTPUT_SPEED;
    motor_command[1]=MOTOR_PORT_A;
    if(arm_winkel[arm_ist]<arm_winkel[arm_soll])
    {
        motor_command[2]=MOTOR_SPEED;
        write(motor_file,motor_command,3);  //motor_speed Wert geben
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_A;
        write(motor_file,motor_command,2);  //Motor starten
        
        while ((pMotorData[0].TachoSensor-arm_referenz) < arm_winkel[arm_soll])
        {
            /* printf("Spd/Cnt/Snr: A=%d/%d/%d\n", pMotorData[0].Speed, pMotorData[0].TachoCounts, pMotorData[0].TachoSensor-arm_referenz);*/
            usleep(10000);
        }
    }
    else
    {
        motor_command[2]=-MOTOR_SPEED;
        write(motor_file,motor_command,3);  //motor_speed Wert geben
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_A;
        write(motor_file,motor_command,2);  //Motor starten
        
        
        while ((pMotorData[0].TachoSensor-arm_referenz) > arm_winkel[arm_soll])
        {
            /*printf("Spd/Cnt/Snr: A=%d/%d/%d\n", pMotorData[0].Speed, pMotorData[0].TachoCounts, pMotorData[0].TachoSensor-arm_referenz);*/
            usleep(10000);
        }
    }
    
    motor_command[0]=opOUTPUT_STOP;
    motor_command[1]=MOTOR_PORT_A;
    motor_command[2]=1;
    write(motor_file,motor_command,3); //Motor stopppen
#else
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
#endif
    arm_ist=arm_soll;
    usleep(100000);
#endif
    return 0;
}


int arm_close()
{
#ifdef __linux__
#if 0
    char motor_command[4];
    
    motor_command[0] = opOUTPUT_STOP;
    motor_command[1] = MOTOR_PORT_A;
    motor_command[2]=0;
    write(motor_file,motor_command,3);
    
    close(encoder_file);
    close(motor_file);
#else
    cOutputExit();
#endif
    
#endif
    return 0;
}