//
//  Arm_motor.h
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#ifndef Arm_motor_h
#define Arm_motor_h

#include <stdio.h>

int arm_init();
int arm_setpos(int arm_soll);
int arm_close();

#endif /* Arm_motor_h */
