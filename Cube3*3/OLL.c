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

int OLL(){
    change_topface(2);
    change_rightface(4);
    wuerfel_print(1);
    if ((color_data[0][1]!=2)&&(color_data[0][3]!=2)&&(color_data[0][5]!=2)&&(color_data[0][7]!=2))
    {
        rs_dreh(-1, 90);
        fs_dreh(-1, 90);
        fs_dreh(-1, 90);
        rs_dreh(1, 90);
        ls_dreh(1, 90);
        fs_dreh(1, 90);
        change_topface(2);
    }
        if((color_data[0][1]==2)&&(color_data[0][3]!=2)&&(color_data[0][5]!=2)&&(color_data[0][7]==2)) //richtig
        {
            wuerfel_print(1);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            fs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            fs_dreh(1, 90);
            change_topface(2);
        }
        if ((color_data[0][1]!=2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]!=2)) // richtig
        {
            drehen(-1);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            fs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            fs_dreh(1, 90);
            change_topface(2);
        }
        if ((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]!=2)&&(color_data[0][7]!=2)) // richtig
        {
            rs_dreh(-1, 90);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            bs_dreh(1, 90);
            change_topface(2);
        }
        if((color_data[0][1]!=2)&&(color_data[0][3]==2)&&(color_data[0][5]!=2)&&(color_data[0][7]==2)) //richtig
        {
            drehen(1);
            rs_dreh(-1, 90);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            bs_dreh(1, 90);
            change_topface(2);
        }
        if((color_data[0][1]==2)&&(color_data[0][3]!=2)&&(color_data[0][5]==2)&&(color_data[0][7]!=2)) //richtig
        {
            drehen(-1);            
            rs_dreh(-1, 90);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            bs_dreh(1, 90);
            change_topface(2);
        }
        if((color_data[0][1]!=2)&&(color_data[0][3]!=2)&&(color_data[0][5]==2)&&(color_data[0][7]==2))//richtig
        {
            drehen(1);
            drehen(1);
            rs_dreh(-1, 90);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            bs_dreh(1, 90);
            change_topface(2);
        }
    
    wuerfel_print(1);
    
    int color_kanten[]={1,5,3,4};
    // durch Formular
    int i;
    for(i=0;i<=3;i++){
        
        change_topface(2);
        change_rightface(color_kanten[i]);
        wuerfel_print(1);
        
        if((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[1][0]==2)&&(color_data[1][2]==2)&&(color_data[3][6]==2)&&(color_data[3][8]==2))  // formular 21
        {
            rs_dreh(1, 90);
            rs_dreh(1, 90);
            ds_dreh(1, 90);
            ls_dreh(-1, 90);
            rs_dreh(-1, 90);
            ls_dreh(1, 90);
            rs_dreh(1, 90);
            ls_dreh(-1, 90);
            rs_dreh(-1, 90);
            ls_dreh(1, 90);
            rs_dreh(-1, 90);
            ls_dreh(-1, 90);
            goto fertig;
        }
        if ((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[4][6]==2)&&(color_data[4][8]==2)&&(color_data[3][8]==2)&&(color_data[1][2]==2)) //fomular 22
        {
            rs_dreh(1, 90);
            rs_dreh(-1, 90);
            ds_dreh(-1, 90);
            ls_dreh(-1, 180);
            rs_dreh(-1, 90);
            ls_dreh(1, 180);
            rs_dreh(-1, 90);
            ls_dreh(-1, 180);
            rs_dreh(1, 180);
            ls_dreh(1, 90);
            goto fertig;
        }
        if ((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[0][6]==2)&&(color_data[0][8]==2)&&(color_data[3][6]==2)&&(color_data[3][8]==2))//fomular 23
        {
            rs_dreh(1, 180);
            ls_dreh(-1, 90);
            rs_dreh(1, 90);
            rs_dreh(-1, 90);
            ds_dreh(-1, 90);
            ls_dreh(-1, 90);
            ls_dreh(1, 90);
            rs_dreh(1, 90);
            rs_dreh(-1, 90);
            ds_dreh(-1, 90);
            ls_dreh(1, 90);
            goto fertig;
        }
        if ((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[0][2]==2)&&(color_data[0][8]==2)&&(color_data[3][6]==2)&&(color_data[1][0]==2))//formular 24
        {
            ls_dreh(1, 90);
            fs_dreh(1, 90);
            ls_dreh(-1, 90);
            rs_dreh(-1, 90);
            rs_dreh(-1, 90);
            ls_dreh(1, 90); ///////////////////////
            ls_dreh(1, 90);
            rs_dreh(-1, 90);
            goto fertig;
        }
        if((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[0][6]==2)&&(color_data[0][2]==2)&&(color_data[4][8]==2)&&(color_data[1][2]==2))
            //formular 25
        {
            fs_dreh(-1, 90);
            bs_dreh(1, 90);
            rs_dreh(1, 90);
            rs_dreh(-1, 90);
            ls_dreh(-1, 90);
            ls_dreh(-1, 90);
            rs_dreh(1, 90);
            rs_dreh(1, 90);
            goto fertig;
        }
        if((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[0][2]==2)&&(color_data[4][8]==2)&&(color_data[1][0]==2)&&(color_data[5][8]==2))//formular 26
        {
            
            rs_dreh(1, 90);
            rs_dreh(-1, 90);
            ds_dreh(-1, 90);
            ls_dreh(-1, 90);
            rs_dreh(-1, 90);
            ls_dreh(1, 90);
            rs_dreh(-1, 90);
            ls_dreh(-1, 90);
            goto fertig;
        }
        if((color_data[0][1]==2)&&(color_data[0][3]==2)&&(color_data[0][5]==2)&&(color_data[0][7]==2)&&(color_data[0][8]==2)&&(color_data[4][6]==2)&&(color_data[3][6]==2)&&(color_data[5][6]==2))
            //formular 27
        {
            
            rs_dreh(-1, 90);
            rs_dreh(1, 180);
            ls_dreh(1, 90);
            rs_dreh(1, 90);
            ls_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            goto fertig;
        }
    }
       fertig:return 0;
}

