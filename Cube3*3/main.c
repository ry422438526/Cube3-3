//
//  main.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include <unistd.h>
#include <stdlib.h>

#include "Top_Cross.h"
#include "F2L.h"
#include "OLL.h"
#include "PLL.h"
#include "print_mode.h"
#include "Color_search.h"
#include "stdio.h"
#include "main.h"
#include "Test.h"
#ifdef  __linux__
#include "Arm_motor.h"
#include "Dreh_motor.h"
#include "Farbe_motor.h"
#include "Farbe_scan.h"
#include "Side_dreh.h"
#include "Farbe_sen.h"
#endif

const  char print_mode[][12][9]=
{
    {{36,37,38,27,28,29,45,46,47},
        {39,40,41,30,31,32,48,49,50},
        {42,43,44,33,34,35,51,52,53},
        {-1,-1,-1, 0, 1, 2,-1,-1,-1},
        {-1,-1,-1, 3, 4, 5,-1,-1,-1},
        {-1,-1,-1, 6, 7, 8,-1,-1,-1},
        {-1,-1,-1, 9,10,11,-1,-1,-1},
        {-1,-1,-1,12,13,14,-1,-1,-1},
        {-1,-1,-1,15,16,17,-1,-1,-1},
        {-1,-1,-1,18,19,20,-1,-1,-1},
        {-1,-1,-1,21,22,23,-1,-1,-1},
        {-1,-1,-1,24,25,26,-1,-1,-1} },
    
    {{-1,-1,-1,27,28,29,-1,-1,-1},
        {-1,-1,-1,30,31,32,-1,-1,-1},
        {-1,-1,-1,33,34,35,-1,-1,-1},
        {38,41,44, 0, 1, 2,51,48,45},
        {37,40,43, 3, 4, 5,52,49,46},
        {36,39,42, 6, 7, 8,53,50,47},
        {-1,-1,-1, 9,10,11,-1,-1,-1},
        {-1,-1,-1,12,13,14,-1,-1,-1},
        {-1,-1,-1,15,16,17,-1,-1,-1},
        {-1,-1,-1,18,19,20,-1,-1,-1},
        {-1,-1,-1,21,22,23,-1,-1,-1},
        {-1,-1,-1,24,25,26,-1,-1,-1} },
    
    {{-1,-1,-1,27,28,29,-1,-1,-1},
        {-1,-1,-1,30,31,32,-1,-1,-1},
        {-1,-1,-1,33,34,35,-1,-1,-1},
        {-1,-1,-1, 0, 1, 2,-1,-1,-1},
        {-1,-1,-1, 3, 4, 5,-1,-1,-1},
        {-1,-1,-1, 6, 7, 8,-1,-1,-1},
        {44,43,42, 9,10,11,53,52,51},
        {41,40,39,12,13,14,50,49,48},
        {38,37,36,15,16,17,47,46,45},
        {-1,-1,-1,18,19,20,-1,-1,-1},
        {-1,-1,-1,21,22,23,-1,-1,-1},
        {-1,-1,-1,24,25,26,-1,-1,-1} },
    
    {{-1,-1,-1,27,28,29,-1,-1,-1},
        {-1,-1,-1,30,31,32,-1,-1,-1},
        {-1,-1,-1,33,34,35,-1,-1,-1},
        {-1,-1,-1, 0, 1, 2,-1,-1,-1},
        {-1,-1,-1, 3, 4, 5,-1,-1,-1},
        {-1,-1,-1, 6, 7, 8,-1,-1,-1},
        {-1,-1,-1, 9,10,11,-1,-1,-1},
        {-1,-1,-1,12,13,14,-1,-1,-1},
        {-1,-1,-1,15,16,17,-1,-1,-1},
        {42,39,36,18,19,20,47,50,53},
        {43,40,37,21,22,23,46,49,52},
        {44,41,38,24,25,26,45,48,51} },            //print_mode
    
};
#ifdef __linux__
unsigned char color_data[6][9];
#endif

#ifndef __linux__
unsigned char color_data[6][9]={
    /*{3,2,2,3,4,4,4,3,5},
    {2,5,3,2,2,1,0,3,1},
    {5,2,4,0,5,2,2,1,1},
    {5,0,0,0,0,0,4,4,1},
    {1,3,3,5,3,5,3,4,0},
    {4,1,2,4,1,5,5,1,0}*/
     {2,0,0,0,0,0,2,0,0},
     {4,1,3,4,1,5,0,2,2},
     {5,5,5,3,2,4,4,1,3},
     {2,5,0,1,3,1,3,3,1},
     {1,2,3,3,4,2,1,4,5},
     {5,2,1,4,5,3,4,5,4}
    
     /*{2,0,0,0,0,0,3,0,0},
     {5,1,3,3,1,5,2,2,1},
     {5,3,5,5,2,1,1,5,1},
     {2,2,2,4,3,1,4,3,1},
     {3,1,4,4,4,2,0,4,3},
     {5,2,0,4,5,3,4,5,4}*/
    
    /*{0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,5,5,1},
    {3,2,2,2,2,2,3,2,2},
    {4,4,1,3,3,3,3,3,3},
    {2,1,2,4,4,4,4,4,4},
    {4,3,5,5,5,5,5,5,5}*/
    
    /*{0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,4,5,3},
    {1,2,2,2,2,2,2,2,5},
    {1,1,2,3,3,3,3,3,3},
    {2,3,5,4,4,4,4,4,4},
    {3,4,4,5,5,5,5,5,5}*/
    
    /*{0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,3,5,2},
    {2,2,5,2,2,2,2,2,4},
    {3,3,2,3,3,3,3,3,3},
    {5,4,4,4,4,4,4,4,4},
    {1,1,1,5,5,5,5,5,5}*/
};

#endif

int main(int argc, const char * argv[]) {
    set_conio_terminal_mode();
    
#ifdef __linux__
    Farbe_init();
    arm_init();
    DrehTel_init();
    ColSen_init();
    color_data[6][9]=farbe_scan();
    if (test()==0) {
        arm_close();
        Farbe_close();
        DrehTel_close();
        ColSen_close();
        exit(0);
    }else
    {
        change_topface(0);
        change_rightface(5);
        wuerfel_print(1);
        Top_Cross();
        F2L();
        OLL();
        PLL();
        arm_setpos(4);
    }
    exit(0);
#endif
#ifndef __linux__
        change_topface(0);
        change_rightface(5);
        wuerfel_print(1);
        Top_Cross();
        F2L();
        OLL();
        PLL();
        exit(0);
#endif
#ifndef __linux__
     while(1)
    {
        if(kbhit())
        {
            switch(getch())
            {
                case 'q':
                case 'Q':
                    return 0;
                    
                case '0':
                    wuerfel_print(0);
                    break;
                case '1':
                    wuerfel_print(1);
                    break;
                case '2':
                    wuerfel_print(2);
                    break;
                case '3':
                    wuerfel_print(3);
                    break;
                    
            }
        }
        
    }
#endif
    return 0;
}




