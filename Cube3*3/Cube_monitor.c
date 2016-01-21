//
//  Cube_monitor.c
//  Cube3*3
//
//  Created by Ryoma on 20.01.16.
//  Copyright © 2016 Chunxia Yang. All rights reserved.
//
#ifdef __linux__
#include "Cube_monitor.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include  <sys/mman.h>
#include "lms2012.h"

const char IIC_PORT=0x04;
int iic_device_file;
IIC *pIic;
int i;

int monitor(){
    //Geräte initialisieren
    if((iic_device_file = open(IIC_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
    {
        printf("Failed to open IIC device\n");
        return -1;
    }
    pIic  =  (IIC*)mmap(0, sizeof(UART), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, iic_device_file, 0);
    if (pIic == MAP_FAILED)
    {
        printf("Mapping IIC device failed\n");
        return -1;
    }
    for(i = 0;i<100;i++)
    {
        printf("Value: %d \n", (unsigned char)pIic->Raw[IIC_PORT][pIic->Actual[IIC_PORT]][0]);
        sleep(1);
    }
    
    return 0;
}
#endif
int monitor_close(){
#ifdef __linux
    close(iic_device_file);
#endif
    return 0;
}