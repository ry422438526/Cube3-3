//
//  Side_dreh.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include <stdio.h>
#include "Side_dreh.h"

#ifdef __linux__
#include "Arm_motor.h"
#include "Dreh_motor.h"
#endif

#include "main.h"
#include "unistd.h"
#include "print_mode.h"

int Count=0;
void klappen()
{
#ifdef __linux__
    arm_setpos(3);
    arm_setpos(2);
    arm_setpos(4);
#endif
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
    wuerfel_print(1);/////////////
}


void drehen_unter(int dir,int dreh_Angel)
{
#ifdef __linux__
    arm_setpos(3);
    if(dir==1)
    {
        DrehTel_setpos(2*dreh_Angel*3/270);
    }else{
        DrehTel_setpos(dreh_Angel*3/270);
    }
#endif
    int i,j,k;
    unsigned char temp_color_data[6][9];
    for (k=0; k<dreh_Angel/90;k++)
    {
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
                
                for (j=0; j<3; j++)
                {
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
    wuerfel_print(1);/////////////
}

void drehen(int dir)                       //drehen 90 Grad
{
#ifdef __linux__
    arm_setpos(4);
    if(dir==1)
    {
        DrehTel_setpos(1);
    }else{
        DrehTel_setpos(2);
    }
#endif
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
    wuerfel_print(1);/////////////
}

void us_dreh(int dir,int dreh_Angel_u)                        //Oben_Schicht
{
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    drehen_unter(dir, dreh_Angel_u);
    Count++;
    printf("Count=%d",Count);
    wuerfel_print(1);
}

void ds_dreh(int dir,int dreh_Angel_d)                        //Untern_Schicht
{
    drehen_unter(dir, dreh_Angel_d);
    wuerfel_print(1);
    Count++;
    printf("Count=%d",Count);
    
}

void fs_dreh(int dir,int dreh_Angel_f)                         //Vornen_Schicht
{
#ifdef __linux__
    arm_setpos(4);
    sleep(1);
#endif
    drehen(1);
#ifdef __linux__
    sleep(1);
#endif
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    drehen_unter(dir, dreh_Angel_f);
    Count++;
    printf("Count=%d",Count);
    wuerfel_print(1);
}

void bs_dreh(int dir,int dreh_Angel_b)                         //Hintern_Schicht
{
#ifdef __linux__
    arm_setpos(4);
    sleep(1);
#endif
    drehen(-1);
#ifdef __linux__
    sleep(1);
#endif
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    drehen_unter(dir, dreh_Angel_b);
    Count++;
    printf("Count=%d",Count);
    wuerfel_print(1);
    
}

void ls_dreh(int dir,int dreh_Angel_l)                         //linken_Schicht
{
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    drehen_unter(dir, dreh_Angel_l);
    Count++;
    printf("Count=%d",Count);
    wuerfel_print(1);
    
}

void rs_dreh(int dir,int dreh_Angel_r)                          //rechten_Schicht
{
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    klappen();
#ifdef __linux__
    sleep(1);
#endif
    drehen_unter(dir, dreh_Angel_r);
    Count++;
    printf("Count=%d",Count);
    wuerfel_print(1);
}

int change_topface(int center_color)
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
    return 0;
}

int change_rightface(int rightface)
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
    return 0;
}

