//
//  print_mode.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include "print_mode.h"
#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include "Top_Cross.h"
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#ifdef __linux__
#define RUN_EV3    1
#else
#define RUN_MACOS  1
#endif

//0 =Weisse 1 =Balu 2 =Gelb 3 =Gruen 4 =Rot 5 =Orangen

#ifndef __linux__
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;
    
    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));
    
    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}



int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}
#endif
void wuerfel_print(int mode)
{
    
    int zeile;
    int spalte;
    
    setenv("XcodeColor", "YES", 0);
    
    printf("\n");
    printf(".........................................");
    printf("\n");
    printf("\n");
    for(zeile=0;zeile<12;zeile++)
    {
        for(spalte=0;spalte<9;spalte++)
        {
            char tmp=print_mode[mode][zeile][spalte];
            if(tmp==-1)
                printf("    ");
            else
                switch(color_data[tmp/9][tmp%9])
            {
#ifdef RUN_MACOS
                case 0:
                    printf("\033[fg0,0,0;   %d\033[;",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 1:
                    printf("\033[fg0,0,255;   %d\033[;",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 2:
                    printf("\033[fg255,255,0;   %d\033[;",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 3:
                    printf("\033[fg0,255,0;   %d\033[;",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 4:
                    printf("\033[fg255,0,0;   %d\033[;",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 5:
                    printf("\033[fg255,97,0;   %d\033[;",color_data[tmp/9][tmp%9]);
                    break;
#endif
#ifdef RUN_EV3
                case 0:
                    printf("\033[1;30m   %d\033[0m",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 1:
                    printf("\033[1;34m   %d\033[0m",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 2:
                    printf("\033[1;33m   %d\033[0m",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 3:
                    printf("\033[1;32m   %d\033[0m",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 4:
                    printf("\033[1;35m   %d\033[0m",color_data[tmp/9][tmp%9]);
                    break;
                    
                case 5:
                    printf("\033[1;31m   %d\033[0m",color_data[tmp/9][tmp%9]);
                    break;
#endif
            }
        }
        printf("\n\r");
    }
}
