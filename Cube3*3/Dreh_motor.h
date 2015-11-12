//
//  Dreh_motor.h
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#ifndef Dreh_motor_h
#define Dreh_motor_h

#include <stdio.h>

int DrehTel_init();
int DrehTel_setpos(int Tel_soll);
int DrehTel_dreh();
int DrehTel_close();

#endif /* Dreh_motor_h */
