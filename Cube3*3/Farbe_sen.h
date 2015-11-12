//
//  Farbe_sen.h
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#ifndef Farbe_sen_h
#define Farbe_sen_h

#include <stdio.h>
typedef enum
{
    WEISS=0,
    BLAU=1,
    GELB=2,
    GRUEN=3,
    ROT=4,
    ORANGE=5
} COLOR;

int ColSen_init();
int ColSen_getData();
int ColSen_close();

int Max_RGB(int a,int b);
COLOR Color_col(float rot, float gruen,float blau);

#endif /* Farbe_sen_h */
