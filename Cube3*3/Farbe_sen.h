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
typedef int FARBE;
int ColSen_init();
int ColSen_getData();
int ColSen_close();

int MAX_RGB(int a,int b);
FARBE Color_col(float rot, float gruen,float blau);

#endif /* Farbe_sen_h */
