//
//  OLL.c
//  Cube3*3
//
//  Created by Ryoma on 09.12.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include "OLL.h"
#include "Color_search.h"
#include "Side_dreh.h"
#include "print_mode.h"


int n=0,m=0;  //n=die Kantenstein des unteren Seites; m=die richtigen Menge der untersn Seites

void OLL(){
    change_topface(2);
    change_rightface(4);
    n=nSum();
    m=mSum();
    if (n==0)
    {
        fs_dreh(1, 90);
        fs_dreh(1, 90);
        bs_dreh(1, 90);
        rs_dreh(-1, 90);
        ls_dreh(-1, 90);
        bs_dreh(-1, 90);
        us_dreh(1, 90);
        fs_dreh(1, 90);
        bs_dreh(1, 90);
        rs_dreh(-1, 90);
        ls_dreh(-1, 90);
        bs_dreh(-1, 90);
        us_dreh(1, 90);
        fs_dreh(1, 90);
        bs_dreh(1, 90);
        rs_dreh(-1, 90);
        ls_dreh(-1, 90);
        bs_dreh(-1, 90);
    }
    if(n==2)
    {
        
    }
    if(n==4)
    {
        break;
    }
}

int  mSum(){
    int i,j,p,sum_m=0;
    for (i=0; i<=2; i++) {
        for (j=0; j<=2; j++) {
            p=Mittelstein_suchen(2);
            if (color_data[p][(3*i+j)]) {
                sum_m++;
            }
        }
    }
    return sum_m;
}

int nSum(){
    int i,p,sum_n=0; // Position der Gelbseit
    int color_kanten[]={1,3,5,7};
    p=Mittelstein_suchen(2);
    for (i=0; i<=3; i++) {
        if (color_data[p][color_kanten[i]]) {
            sum_n++;
        }
    }
    return sum_n;
}

