//
//  Side_dreh.h
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#ifndef Side_dreh_h
#define Side_dreh_h

#include <stdio.h>

void change_topface(int center_color);
void change_rightface(int rightface);
void klappen();
void drehen_unter(int dir);
void drehen(int dir);
void rs_dreh(int dir,int dreh_Angel_r);
void ls_dreh(int dir,int dreh_Angel_l);
void fs_dreh(int dir,int dreh_Angel_f);
void bs_dreh(int dir,int dreh_Angel_b);
void us_dreh(int dir,int dreh_Angel_u);
void ds_dreh(int dir,int dreh_Angel_d);
void wuefel_dreh(int achse,int dir,int dreh_Angel);

#endif /* Side_dreh_h */
