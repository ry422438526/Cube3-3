//
//  Top_Cross.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include <stdio.h>
#include "Top_Cross.h"
#include "Side_dreh.h"
#include "Color_search.h"
#include "print_mode.h"

int Top_Cross()
{
    
    int Getpos_k=0;
    int color_kanten[4]={1,4,3,5};
    int i;
    
    for(i=0;i<=3;i++)
    {
        change_topface(0);
        change_rightface(color_kanten[i]);
        Getpos_k=Kantenstein_suchen(0, color_kanten[i]);
        switch (Getpos_k) {
            case 103:
                us_dreh(1, 90);
                break;
            case 130:
                bs_dreh(-1, 90);
                bs_dreh(-1, 90);
                break;
            case 104:
                if (i==0)
                {
                    us_dreh(1, 180);
                }else{
                    ls_dreh(1, 180);
                    rs_dreh(1, 180);
                    rs_dreh(1, 180);
                }
                break;
            case 140:
                ls_dreh(1, 90);
                fs_dreh(-1, 90);
                fs_dreh(1, 90);
                bs_dreh(1, 180);
                
                break;
            case 105:
                break;
            case 150:
                
                if (i==0) {
                    rs_dreh(-1, 90);
                    fs_dreh(-1, 90);
                    fs_dreh(-1, 90);
                }else{
                    rs_dreh(-1, 180);
                    ls_dreh(-1, 90);
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                    rs_dreh(1, 90);
                }
                break;
            case 101:
                if (i==0)
                {
                    us_dreh(-1, 90);
                }else{
                    fs_dreh(1, 180);
                    rs_dreh(1, 90);
                    fs_dreh(1, 180);
                }
                
                break;
            case 110:
                fs_dreh(1, 90);
                fs_dreh(1, 90);
                
                break;
            case 241:
                if (i==0) {
                    fs_dreh(1, 90);
                    ls_dreh(-1,90);
                }else{
                    fs_dreh(-1, 90);
                    rs_dreh(1, 90);
                    ls_dreh(1, 90);
                    fs_dreh(1, 180);
                }
                break;
            case 214:
                if (i<2) {
                    fs_dreh(1, 180);
                    fs_dreh(1, 90);
                }else{
                    fs_dreh(1, 180);
                    fs_dreh(1, 90);
                    rs_dreh(1, 180);
                }
                break;
            case 215:      //pruefen
                rs_dreh(1, 90);
                break;
            case 251:
                if (i==0) {
                    fs_dreh(1, 90);
                    rs_dreh(1, 90);
                    fs_dreh(1, 180);
                }else{
                    fs_dreh(1, 90);
                    rs_dreh(1, 90);
                    ls_dreh(-1, 90);
                    fs_dreh(1, 180);
                }
                
                break;
            case 253:      //pruefen
                if (i<2) {
                    rs_dreh(1, 180);
                    fs_dreh(1, 90);
                    bs_dreh(1, 90);
                    bs_dreh(1, 180);
                }else{
                    rs_dreh(1, 180);
                    fs_dreh(1, 90);
                    bs_dreh(1, 90);
                    rs_dreh(-1, 90);
                    bs_dreh(1, 180);
                }
                break;
            case 235:     //pruefen
                rs_dreh(-1, 90);
                break;
            case 234:     //pruefen
                if (i==0) {
                    bs_dreh(-1, 180);
                    bs_dreh(-1, 90);
                }else{
                    bs_dreh(-1, 180);
                    bs_dreh(-1, 90);
                    rs_dreh(1, 180);
                }
                break;
            case 243:     //pruefen
                if (i==0) {
                    bs_dreh(1, 90);
                    rs_dreh(-1, 90);
                    bs_dreh(-1, 180);
                }else{
                    bs_dreh(1, 90);
                    rs_dreh(-1, 90);
                    bs_dreh(-1, 180);
                    fs_dreh(-1, 90);
                }
                break;
            case 312:     //pruefen
                if (i==0) {
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                }else{
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                    rs_dreh(1, 90);
                }
                
                break;
            case 321:      //pruefen
                ds_dreh(1, 90);
                rs_dreh(1, 180);
                
                break;
            case 352:      //pruefen
                if (i<2) {
                    ds_dreh(-1, 90);
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                }else{
                    ds_dreh(-1, 90);
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                    rs_dreh(1, 90);
                }
                break;
            case 325:        //pruefen
                rs_dreh(1, 180);
                break;
            case 323:       //pruefen
                ds_dreh(-1, 90);
                rs_dreh(1, 180);
                break;
            case 332:       //pruefen
                if (i==0) {
                    bs_dreh(1, 90);
                    bs_dreh(-1, 90);
                }else{
                    bs_dreh(1, 90);
                    bs_dreh(-1, 90);
                    rs_dreh(-1, 90);
                }
                break;
            case 342:       //pruefen
                if (i<2) {
                    ds_dreh(1, 90);
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                }else{
                    ds_dreh(1, 90);
                    fs_dreh(-1, 90);
                    fs_dreh(1, 90);
                    rs_dreh(1, 90);
                }
                
                break;
            case 324:       //pruefen
                ds_dreh(1, 180);
                rs_dreh(1, 180);
                
                break;
            default:
                break;
        }
        printf("Getpos_k=%d",Getpos_k);
    }
    change_topface(0);
    change_rightface(5);
    int Getpos_k1=0;
    Getpos_k1=Kantenstein_suchen(0, 5);
    switch (Getpos_k1) {
        case 101:
            us_dreh(-1, 90);
            break;
        case 103:
            us_dreh(1, 90);
            break;
        case 104:
            us_dreh(1, 180);
            break;
        case 105:
            break;
            
        default:
            break;
    }
    return 0;
}

/*
int TC_Test(){
    int tc_test=-1;
    if ((color_data[0][1]==color_data[0][4])&&(color_data[0][3]==color_data[0][4])&&(color_data[0][5]==color_data[0][4])&&(color_data[0][7]==color_data[0][4])&&(color_data[1][1]==color_data[1][4])&&(color_data[5][7]==color_data[5][4])&&(color_data[3][7]==color_data[3][4])&&(color_data[4][7]==color_data[4][4])){
        tc_test=1;
    }else{
        tc_test=0;
    }
    return tc_test;
}*/
