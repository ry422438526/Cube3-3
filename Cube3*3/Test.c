//
//  Test.c
//  Cube3*3
//
//  Created by Ryoma on 16/1/6.
//  Copyright © 2016年 Chunxia Yang. All rights reserved.
//

#include "Test.h"
#include "main.h"
#include "print_mode.h"


typedef enum
{
    WEISS=0,
    BLAU=1,
    GELB=2,
    GRUEN=3,
    ROT=4,
    ORANGE=5
}FARBE;

int test(){
    int test=-1;
    int i,j;
    int Farbe_r=0,Farbe_g=0,Farbe_b=0,Farbe_w=0,Farbe_o=0,Farbe_y=0;
    for (i=0; i<=5; i++) {
        for (j=0; j<=8; j++) {
            switch (color_data[i][j]) {
                case ROT:
                    Farbe_r++;
                    break;
                case BLAU:
                    Farbe_b++;
                    break;
                case GELB:
                    Farbe_y++;
                    break;
                case WEISS:
                    Farbe_w++;
                    break;
                case GRUEN:
                    Farbe_g++;
                    break;
                case ORANGE:
                    Farbe_o++;
                    break;
                    
                default:
                    break;
            }
        }
    }
    if ((Farbe_r==9)&&(Farbe_g==9)&&(Farbe_b==9)&&(Farbe_w==9)&&(Farbe_y==9)&&(Farbe_o==9)) {
        wuerfel_print(1);
        test=1;

    }
    else{
#ifdef __linux__
        wuerfel_print(1);
        test=0;
#endif
        printf("Das Farbe_Array ist nicht richtig, bitte neu scannen!\n\r");
    }
    return test;
}
