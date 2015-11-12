//
//  Side_dreh.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include <stdio.h>
#include "Side_dreh.h"
#include "Arm_motor.h"
#include "Dreh_motor.h"
#include "unistd.h"
#include "main.h"
typedef enum
{
    U=0,
    F=1,
    D=2,
    B=3,
    L=4,
    R=5
}SURFACE;

int Surface=-1;

void klappen()
{
    //#ifdef __ARMEL__
#ifdef __linux__
    //Code zur Ansteuerung der Hardware
#endif
    arm_setpos(3);
    arm_setpos(2);
    arm_setpos(3);
    int i,j;
    unsigned char temp_color_data[6][9];
    for(i=0;i<3;i++)
    {
        for (j=0; j<3; j++)
        {
            temp_color_data[1][3*i+j]=color_data[1][3*j+(2-i)]; //
            temp_color_data[3][3*i+j]=color_data[3][3*(2-j)+i]; //
            temp_color_data[5][3*i+j]=color_data[2][3*(2-j)+i]; //
            temp_color_data[2][3*(2-j)+i]=color_data[4][3*(2-i)+(2-j)];//
            temp_color_data[4][3*(2-i)+(2-j)]=color_data[0][3*j+(2-i)];//
            temp_color_data[0][3*j+(2-i)]=color_data[5][3*i+j];//
        }
    }
    
    int a,b;
    for (a=0;a<6;a++)
    {
        for (b=0;b<9;b++)
        {
            color_data[a][b]=temp_color_data[a][b];
        }
    }
    
}


void drehen_unter(int dir)
{
    int i,j;
    unsigned char temp_color_data[6][9];
    if (dir==1)
    {
        for(i=0;i<3;i++)
        {
            temp_color_data[5][3*0+i]=color_data[1][3*2+(2-i)];
            temp_color_data[5][3*1+i]=color_data[5][3*1+i];
            temp_color_data[5][3*2+i]=color_data[5][3*2+i];
            
            temp_color_data[1][3*0+i]=color_data[1][3*0+i];
            temp_color_data[1][3*1+i]=color_data[1][3*1+i];
            temp_color_data[1][3*2+(2-i)]=color_data[4][3*0+i];
            
            temp_color_data[4][3*0+i]=color_data[3][3*0+i];
            temp_color_data[4][3*1+i]=color_data[4][3*1+i];
            temp_color_data[4][3*2+i]=color_data[4][3*2+i];
            
            temp_color_data[3][3*0+i]=color_data[5][3*0+i];
            temp_color_data[3][3*1+i]=color_data[3][3*1+i];
            temp_color_data[3][3*2+i]=color_data[3][3*2+i];
            
            for (j=0; j<3; j++) {
                temp_color_data[2][3*i+j]=color_data[2][3*(2-j)+i];
                temp_color_data[0][3*i+j]=color_data[0][3*i+j];
            }
        }
    }
    else
    {
        for(i=0;i<3;i++)
        {
            temp_color_data[5][3*0+i]=color_data[3][3*0+i];
            temp_color_data[5][3*1+i]=color_data[5][3*1+i];
            temp_color_data[5][3*2+i]=color_data[5][3*2+i];
            
            temp_color_data[3][3*0+i]=color_data[4][3*0+i];
            temp_color_data[3][3*1+i]=color_data[3][3*1+i];
            temp_color_data[3][3*2+i]=color_data[3][3*2+i];
            
            temp_color_data[4][3*0+i]=color_data[1][3*2+2-i];
            temp_color_data[4][3*1+i]=color_data[4][3*1+i];
            temp_color_data[4][3*2+i]=color_data[4][3*2+i];
            
            temp_color_data[1][3*0+i]=color_data[1][3*0+i];
            temp_color_data[1][3*1+i]=color_data[1][3*1+i];
            temp_color_data[1][3*2+2-i]=color_data[5][3*0+i];
            
            for (j=0; j<3; j++)
            {
                temp_color_data[2][3*i+j]=color_data[2][3*j+(2-i)];
                temp_color_data[0][3*i+j]=color_data[0][3*i+j];
            }
        }
        
    }
    
    int a,b;
    for (a=0;a<6;a++)
    {
        for (b=0;b<9;b++)
        {
            color_data[a][b]=temp_color_data[a][b];
        }
    }
}

void drehen(int dir)                       //drehen 90 Grad
{
    int i,j;
    unsigned char temp_color_data[6][9];
    if(dir==1)
    {
        for (i=0; i<3; i++) {
            for (j=0; j<3; j++) {
                temp_color_data[0][3*i+j]=color_data[0][3*(2-j)+i];
                temp_color_data[2][3*i+j]=color_data[2][3*j+(2-i)];
                temp_color_data[4][3*i+j]=color_data[1][3*(2-i)+(2-j)];
                temp_color_data[1][3*(2-i)+(2-j)]=color_data[5][3*i+j];
                temp_color_data[5][3*i+j]=color_data[3][3*i+j];
                temp_color_data[3][3*i+j]=color_data[4][3*i+j];
            }
        }
    }else{
        for (i=0; i<3; i++) {
            for (j=0; j<3; j++) {
                temp_color_data[0][3*i+j]=color_data[0][3*j+(2-i)];
                temp_color_data[2][3*i+j]=color_data[2][3*(2-j)+i];
                temp_color_data[5][3*i+j]=color_data[1][3*(2-i)+(2-j)];
                temp_color_data[1][3*(2-i)+(2-j)]=color_data[4][3*i+j];
                temp_color_data[4][3*i+j]=color_data[3][3*i+j];
                temp_color_data[3][3*i+j]=color_data[5][3*i+j];
            }
        }
    }
    int a,b;
    for (a=0;a<6;a++)
    {
        for (b=0;b<9;b++)
        {
            color_data[a][b]=temp_color_data[a][b];
        }
    }
}

void us_dreh(int dir,int dreh_Angel_u)                        //Oben_Schicht
{
    klappen();
    sleep(1);
    klappen();
    sleep(1);
    if(dir==1)
    {
        DrehTel_setpos(2*dreh_Angel_u*3/270);
    }else{
        DrehTel_setpos(dreh_Angel_u*3/270); // 90,180,270 gegen Uhrzeigersinn drehen
    }
}

void ds_dreh(int dir,int dreh_Angel_d)                        //Untern_Schicht
{
    arm_setpos(2);
    arm_setpos(3);
    if (dir==1) {
        DrehTel_setpos(2*dreh_Angel_d*3/270);// 90,180,270 gegen Uhrzeigersinn drehen
    }else{
        DrehTel_setpos(dreh_Angel_d*3/270);
    }
    
}

void fs_dreh(int dir,int dreh_Angel_f)                         //Vornen_Schicht
{
    arm_setpos(4);
    sleep(1);
    DrehTel_setpos(1); //90 im Uhrzeigersinn drehen
    drehen(1);
    sleep(1);
    klappen();
    sleep(1);
    if(dir==1){
        DrehTel_setpos(2*dreh_Angel_f*3/270);
    }else{
        DrehTel_setpos(dreh_Angel_f*3/270); // 90,180,270 im Uhrzeigersinn drehen
    }
    
}

void bs_dreh(int dir,int dreh_Angel_b)                         //Hintern_Schicht
{
    arm_setpos(4);
    sleep(1);
    DrehTel_setpos(2); //90 gegen Uhrzeigersinn drehen
    drehen(-1);
    sleep(1);
    klappen();
    sleep(1);
    if (dir==1) {
        DrehTel_setpos(2*dreh_Angel_b*3/270); // 90,180,270 gegen Uhrzeigersinn drehen
    }else{
        DrehTel_setpos(dreh_Angel_b*3/270);
    }
    
}

void ls_dreh(int dir,int dreh_Angel_l)                         //linken_Schicht
{
    klappen();
    sleep(1);
    if(dir==1){
        DrehTel_setpos(2*dreh_Angel_l*3/270); // 90,180,270 gegen Uhrzeigersinn drehen
    }else{
        DrehTel_setpos(dreh_Angel_l*3/270);
    }
    
}

void rs_dreh(int dir,int dreh_Angel_r)                          //rechten_Schicht
{
    
    
    klappen();
    sleep(1);
    klappen();
    sleep(1);
    klappen();
    sleep(1);
    if(dir==1){
        DrehTel_setpos(2*dreh_Angel_r*3/270); // 90,180,270 gegen Uhrzeigersinn drehen
    }else{
        DrehTel_setpos(dreh_Angel_r*3/270);
    }
}


void wuefel_dreh(int achse,int dir,int dreh_Angel)
{
    int i;
    switch (achse)
    {
        case 0:
            us_dreh(dir,dreh_Angel);
            for (i=0; i<dreh_Angel/90; i++) {
                drehen_unter(dir);
            }
            break;
        case 1:
            rs_dreh(dir,dreh_Angel);
            for (i=0; i<dreh_Angel/90; i++) {
                drehen_unter(dir);
            }
            break;
        case 2:
            ds_dreh(dir,dreh_Angel);
            for (i=0; i<dreh_Angel/90; i++) {
                drehen_unter(dir);
            }
            break;
        case 3:
            ls_dreh(dir,dreh_Angel);
            for (i=0; i<dreh_Angel/90; i++) {
                drehen_unter(dir);
            }
            break;
        case 4:
            fs_dreh(dir,dreh_Angel);
            for (i=0; i<dreh_Angel/90; i++) {
                drehen_unter(dir);
            }
            break;
        case 5:
            bs_dreh(dir,dreh_Angel);
            for (i=0; i<dreh_Angel/90; i++) {
                drehen_unter(dir);
            }
            break;
            
        default:
            break;
    }
    
}



void change_topface(int center_color)
{   int i;
    for (i=0; i<=5; i++) {
        if (color_data[i][4]==center_color)
        {
            switch (i) {
                case 0://ober Seite
                    break;
                case 1://vorne Seite
                    drehen(-1);
                    klappen();
                    break;
                case 2://untere Seite
                    klappen();
                    klappen();
                    break;
                case 3://hintere Seite
                    drehen(1);
                    klappen();
                    break;
                case 4://linke Seite
                    klappen();
                    klappen();
                    klappen();
                    break;
                case 5://rechte Seite
                    klappen();
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void change_rightface(int rightface)
{
    int i;
    for (i=0; i<=5; i++) {
        if (color_data[i][4]==rightface) {
            switch (i) {
                case 1://vorne Seite
                    drehen(-1);
                    break;
                case 3://hintere Seite
                    drehen(1);
                    break;
                case 4://linke Seite
                    drehen(1);
                    drehen(1);
                    break;
                default:
                    break;
            }
        }
    }
}

