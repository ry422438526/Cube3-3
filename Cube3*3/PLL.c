//
//  PLL.c
//  Cube3*3
//
//  Created by Ryoma on 20.12.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include "PLL.h"
#include "Color_search.h"
#include "Side_dreh.h"
#include "print_mode.h"


void PLL()
{
    change_topface(2);
    change_rightface(4);
    int i,j;
    int Color_col[4]={1,4,3,5};
    
    if ((color_data[1][0]!=color_data[1][2])&&(color_data[5][6]!=color_data[5][8])&&(color_data[3][6]!=color_data[3][8])&&(color_data[4][6]!=color_data[4][8])) {
        rs_dreh(1, 180);
        fs_dreh(1, 180);
        rs_dreh(-1, 90);
        rs_dreh(-1, 90);
        ls_dreh(1, 90);
        ls_dreh(1, 180);
        rs_dreh(-1, 90);
        rs_dreh(1, 90);
        ls_dreh(-1, 90);
    }
    
    if ((color_data[1][0]==color_data[1][2])&&(color_data[5][6]==color_data[5][8])&&(color_data[3][6]==color_data[3][8])&&(color_data[4][6]==color_data[4][8])) {
        switch (color_data[1][0]) {
            case 1:
                break;
            case 4:
                us_dreh(-1, 90);
                break;
            case 3:
                us_dreh(-1, 180);
                break;
            case 5:
                us_dreh(1, 90);
                break;
            default:
                break;
        }
        
    }else{
    wuerfel_print(1);        /////////////
    for (i=0; i<=3; i++)
    {
        change_topface(2);
        change_rightface(Color_col[(i+1)%4]);
        wuerfel_print(1);////////////////
        if(color_data[1][0]==color_data[1][2])
        {
            for (j=0; j<=3; j++)
            {
                if ((color_data[1][0]==color_data[Color_col[j]][4])&&(color_data[1][2]==color_data[Color_col[j]][4]))
                {
                    int color_c;
                    color_c=color_data[Color_col[j]][4];
                    switch (j)
                    {
                        case 0:
                            change_rightface(color_c);
                            wuerfel_print(1);/////////////////
                            break;
                        case 1:
                            us_dreh(1, 90);
                            change_topface(2);
                            change_rightface(color_c);
                            wuerfel_print(1);/////////////////
                            break;
                        case 2:
                            us_dreh(1, 180);
                            change_topface(2);
                            wuerfel_print(1);
                            change_rightface(color_c);
                            wuerfel_print(1);////////////////
                            break;
                        case 3:
                            us_dreh(-1, 90);
                            change_topface(2);
                            change_rightface(color_c);
                            wuerfel_print(1);////////////////
                            break;
                        default:
                            break;
                    }
                }
            }
            wuerfel_print(1);
            rs_dreh(1, 180);
            fs_dreh(1, 180);
            rs_dreh(-1, 90);
            rs_dreh(-1, 90);
            ls_dreh(1, 90);
            ls_dreh(1, 180);
            rs_dreh(-1, 90);
            rs_dreh(1, 90);
            ls_dreh(-1, 90);
            change_topface(2);
            change_rightface(4);
            switch (color_data[1][0]) {
                case 1:
                    break;
                case 4:
                    us_dreh(-1, 90);
                    break;
                case 3:
                    us_dreh(-1, 180);
                    break;
                case 5:
                    us_dreh(1, 90);
                    break;
                default:
                    break;
            }
            break;
        }
    }
}
    wuerfel_print(1);//////////////////////
    for (i=0; i<=3; i++) {
        change_topface(2);
        change_rightface(Color_col[(i+1)%4]);
      /*if ((color_data[3][6]==color_data[3][7]) && (color_data[3][7]==color_data[3][8])&&(color_data[3][7]==color_data[3][4])) {
        }*/
        wuerfel_print(1);//////////////////////
        if ((color_data[1][0]==color_data[1][1]) && (color_data[1][1]==color_data[1][2])&&(color_data[1][0]==color_data[1][4])) {
                drehen(1);
                drehen(1);
            wuerfel_print(1);//////////////////////
            if ((color_data[1][1]==color_data[5][4])&&(color_data[5][7]==color_data[4][4])&&(color_data[4][7]==color_data[1][4])) {
                rs_dreh(1, 90);
                rs_dreh(-1, 90);
                ls_dreh(1, 90);
                rs_dreh(1, 90);
                ls_dreh(1, 90);
                rs_dreh(1, 90);
                ls_dreh(1, 90);
                rs_dreh(-1, 90);
                ls_dreh(-1, 90);
                rs_dreh(-1, 90);
                ls_dreh(1, 180);
                wuerfel_print(1);//////////////////////            richtig
            }
            
            
            if ((color_data[1][1]==color_data[4][4])&&(color_data[5][7]==color_data[1][4])&&(color_data[4][7]==color_data[5][4])) {
                rs_dreh(-1, 180);
                rs_dreh(1, 90);
                ls_dreh(1, 90);
                rs_dreh(1, 90);
                ls_dreh(-1, 90);
                rs_dreh(-1, 90);
                ls_dreh(-1, 90);
                rs_dreh(-1, 90);
                ls_dreh(-1, 90);
                rs_dreh(1, 90);
                ls_dreh(-1, 90);
                wuerfel_print(1);//////////////////////  richtig
            }
        
        }
        
        if ((color_data[1][1]==color_data[3][4])&&(color_data[4][7]==color_data[5][4])&&(color_data[3][7]==color_data[1][4])&&(color_data[5][7]==color_data[4][4])) {
            ls_dreh(1, 180);
            us_dreh(1, 180);
            ls_dreh(1, 90);
            ls_dreh(1, 180);
            us_dreh(1, 180);
            rs_dreh(1, 180);
            ls_dreh(1, 180);
            us_dreh(1, 180);
            rs_dreh(1, 90);
            ls_dreh(1, 180);
            us_dreh(1, 180);
            wuerfel_print(1);//////////////////////    richtig
        }
        if ((color_data[1][1]==color_data[4][4])&&(color_data[4][7]==color_data[1][4])&&(color_data[3][7]==color_data[5][4])&&(color_data[5][7]==color_data[3][4])) {
            drehen(1);
            wuerfel_print(1);//////////////////////
        }
        
        if ((color_data[1][1]==color_data[5][4])&&(color_data[4][7]==color_data[3][4])&&(color_data[3][7]==color_data[4][4])&&(color_data[5][7]==color_data[1][4])) {
            ls_dreh(1, 180);
            us_dreh(1, 180);
            ls_dreh(1, 90);
            ls_dreh(1, 180);
            us_dreh(1, 180);
            rs_dreh(1, 90);
            ls_dreh(1, 90);
            us_dreh(-1, 90);
            fs_dreh(1, 180);
            ls_dreh(1, 180);
            us_dreh(1, 180);
            rs_dreh(1, 180);
            ls_dreh(-1, 90);
            us_dreh(1, 90);
            bs_dreh(1, 180);
            wuerfel_print(1);////////////////////// richtig
        }
        
    }
    
}