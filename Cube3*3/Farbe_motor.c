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
#endif
#include <unistd.h>
#include <stdlib.h>

#include "Farbe_motor.h"

#ifdef __linux__
const int MOTOR_SPEED_D=30; //power:0~100
const char MOTOR_PORT_D = 0x08; //Schwenkbarer Farbensensor Motor;
MOTORDATA *pMotorData;
int motor_file;
int encoder_file;
int Farbe_referenz=0;
int Farbe_ist=0;
const int Farbe_winkel[]={4,-690,-610,-580,-400};
#endif

int Farbe_init()
{
#ifdef __linux__
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
    motor_command[0]=opOUTPUT_SET_TYPE;
    motor_command[1]=MOTOR_PORT_D;
    motor_command[2]=TYPE_MINITACHO;
    write(motor_file,motor_command,3);// motor_speed Wert geben
    
    motor_command[0]=opOUTPUT_SPEED;
    motor_command[1]=MOTOR_PORT_D;
    motor_command[2]=MOTOR_SPEED_D;
    write(motor_file,motor_command,3);// motor_speed Wert geben
    
    motor_command[0]=opOUTPUT_START;
    motor_command[1]=MOTOR_PORT_D;
    write(motor_file,motor_command,2);// Motor starten
    
    int old_TachoSensor;
    old_TachoSensor=pMotorData[3].TachoSensor;
    usleep(2*1000000);
    while(old_TachoSensor!=pMotorData[3].TachoSensor){
        printf("Spd/Cnt/Snr: A=%d/%d/%d\n", pMotorData[3].Speed, pMotorData[3].TachoCounts, pMotorData[3].TachoSensor);
        old_TachoSensor=pMotorData[3].TachoSensor;
        usleep(1000);
    }
    
    motor_command[0]=opOUTPUT_STOP;
    motor_command[1]=MOTOR_PORT_D;
    write(motor_file,motor_command,2);  // Motor stoppen
    
    Farbe_referenz=pMotorData[3].TachoSensor;
    Farbe_ist = 0;
    printf("Farbe_referenzPosition angefahren %d\n\r",Farbe_referenz);
    
    //Farbe_setpos(0);
#endif
    return 0;
}


int Farbe_setpos(int Farbe_soll)
{
#ifdef __linux__
    char motor_command[4];
    motor_command[0]=opOUTPUT_SPEED;
    motor_command[1]=MOTOR_PORT_D;
    if(Farbe_winkel[Farbe_ist]<Farbe_winkel[Farbe_soll])
    {
        motor_command[2]=MOTOR_SPEED_D;
        write(motor_file,motor_command,3);  //motor_speed Wert geben
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_D;
        write(motor_file,motor_command,2);  //Motor starten
        
        while ((pMotorData[3].TachoSensor-Farbe_referenz) < Farbe_winkel[Farbe_soll])
        {
            //printf("Spd/Cnt/Snr: A=%d/%d/%d\n", pMotorData[3].Speed, pMotorData[3].TachoCounts, pMotorData[3].TachoSensor-Farbe_referenz);
            usleep(1000);
        }
    }
    else
    {
        motor_command[2]=-MOTOR_SPEED_D;
        write(motor_file,motor_command,3);  //motor_speed Wert geben
        
        motor_command[0]=opOUTPUT_START;
        motor_command[1]=MOTOR_PORT_D;
        write(motor_file,motor_command,2);  //Motor starten
        
        
        while ((pMotorData[3].TachoSensor-Farbe_referenz) > Farbe_winkel[Farbe_soll])
        {
            //printf("Spd/Cnt/Snr: A=%d/%d/%d\n", pMotorData[3].Speed, pMotorData[3].TachoCounts, pMotorData[3].TachoSensor-Farbe_referenz);
            usleep(1000);
        }
    }
    
    motor_command[0]=opOUTPUT_STOP;
    motor_command[1]=MOTOR_PORT_D;
    write(motor_file,motor_command,2); //Motor stopppen
    
    Farbe_ist=Farbe_soll;
#endif
    return 0;
}


int Farbe_close()
{
#ifdef __linux__
    char motor_command[4];
    
    
    motor_command[0] = opOUTPUT_STOP;
    motor_command[1] = MOTOR_PORT_D;
    motor_command[2]=0;
    write(motor_file,motor_command,3);
    
    close(encoder_file);
    close(motor_file);
#endif
    return 0;
}


