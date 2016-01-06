//
//  F2L.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include "F2L.h"
#include "Color_search.h"
#include "Side_dreh.h"
#include "print_mode.h"

int i;
int color_ecken[4]={1,5,3,4};

int F2L(){
    int Getpos_e=0;
    int Getpos_k=0;
    for (i=0; i<=3; i++) {                                                          //nach 41 Formular
        change_topface(0);
        change_rightface(1);
        wuerfel_print(1);
        
        change_topface(2);
        change_rightface(color_ecken[i]);
        Getpos_e=Eckenstein_suchen(0, color_ecken[(i+1)%4], color_ecken[i%4]);
        Getpos_k=Kantenstein_suchen(color_ecken[(i+1)%4], color_ecken[i%4]);
        
        //festlegen die Position des Eckensteins zwischen 1 und 5
        if (Getpos_e==1350 || Getpos_e== 1503 || Getpos_e ==1035) {
            us_dreh(1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }else if (Getpos_e==1014 || Getpos_e== 1140 || Getpos_e ==1401){
            us_dreh(-1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }else if (Getpos_e==1430 || Getpos_e== 1043 || Getpos_e ==1304){
            us_dreh(1, 180);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }else if (Getpos_e==3234 || Getpos_e== 3423 || Getpos_e ==3342){
            ls_dreh(1, 90);
            ls_dreh(-1, 180);
            rs_dreh(-1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }else if (Getpos_e==3253 || Getpos_e== 3532 || Getpos_e ==3325){
            rs_dreh(-1, 90);
            rs_dreh(1, 180);
            ls_dreh(1, 90);
            rs_dreh(-1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }else if (Getpos_e==3241 || Getpos_e== 3124 || Getpos_e ==3412){
            ls_dreh(-1, 90);
            ls_dreh(-1, 90);
            rs_dreh(1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }
        Getpos_e=Eckenstein_suchen(0, color_ecken[(i+1)%4], color_ecken[i%4]);
        Getpos_k=Kantenstein_suchen(color_ecken[(i+1)%4], color_ecken[i%4]);
        if (Getpos_k==235||Getpos_k==253)
        {
            fs_dreh(1, 90);
            us_dreh(1, 90);
            rs_dreh(-1, 90);
            ls_dreh(-1, 90);
            us_dreh(-1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
            
        }else if(Getpos_k==234||Getpos_k==243)
        {
            fs_dreh(1, 90);
            bs_dreh(1, 90);
            fs_dreh(1, 90);
            rs_dreh(-1, 90);
            fs_dreh(-1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }else if (Getpos_k==241 ||Getpos_k==214)
        {
            fs_dreh(1, 90);
            ls_dreh(-1, 90);
            rs_dreh(-1, 90);
            change_topface(2);
            change_rightface(color_ecken[i]);
        }
        Getpos_e=Eckenstein_suchen(0, color_ecken[(i+1)%4], color_ecken[i%4]);
        Getpos_k=Kantenstein_suchen(color_ecken[(i+1)%4], color_ecken[i%4]);
        if (Getpos_e==3215 || Getpos_e==3521 || Getpos_e==3152) {
            if (Getpos_k==104) {
                us_dreh(-1, 90);
            }else if (Getpos_k==140){
                us_dreh(-1, 180);
            }else if(Getpos_k==103){
                us_dreh(1, 90);
            }else if (Getpos_k==130){
                us_dreh(1, 180);
            }else if(Getpos_k==150){
                us_dreh(1, 90);
            }else if (Getpos_k==101){
                us_dreh(-1, 90);
            }
            change_topface(2);
            change_rightface(color_ecken[i]);
        }
        
        Getpos_e=Eckenstein_suchen(0, color_ecken[(i+1)%4], color_ecken[i%4]);
        Getpos_k=Kantenstein_suchen(color_ecken[(i+1)%4], color_ecken[i%4]);
        
        wuerfel_print(1);
        
        if (Getpos_e==1510) {
            switch (Getpos_k) {
                case 101:
                    Formular2_2();
                    break;
                case 110:
                    Basis_3();
                    break;
                case 103:
                    Basis_1();
                    break;
                case 130:
                    Formular3_3();
                    break;
                case 104:
                    Formular2_1();
                    break;
                case 140:
                    Formular3_4();
                    break;
                case 105:
                    Formular2_3();
                    break;
                case 150:
                    Formular8_1();
                    break;
                case 215:
                    Formular6_3();
                    break;
                case 251:
                    Formular6_1();
                    break;
                default:
                    break;
            }
        }
        
        
        if (Getpos_e==1105) {
            switch (Getpos_k) {
                case 101:
                    Formular8_2();
                    break;
                case 110:
                    Formular2_6();
                    break;
                case 103:
                    Formular3_2();
                    break;
                case 130:
                    Formular2_4();
                    break;
                case 104:
                    Formular3_1();
                    break;
                case 140:
                    Basis_2();
                    break;
                case 105:
                    Basis_4();
                    break;
                case 150:
                    Formular2_5();
                    break;
                case 215:
                    Formular6_4();
                    break;
                case 251:
                    Formular6_2();
                    break;
                default:
                    break;
            }
        }
        
        if (Getpos_e==1051) {
            switch (Getpos_k) {
                case 101:
                    Formular11_2();
                    break;
                case 110:
                    Formular7_2();
                    break;
                case 103:
                    Formular4_2();
                    break;
                case 130:
                    Formular4_4();
                    break;
                case 104:
                    Formular4_3();
                    break;
                case 140:
                    Formular4_1();
                    break;
                case 105:
                    Formular7_1();
                    break;
                case 150:
                    Formular11_1();
                    break;
                case 215:
                    Formular9_2();
                    break;
                case 251:
                    Formular9_1();
                    break;
                default:
                    break;
            }
        }
        
        if(Getpos_e==3215){
            switch (Getpos_k) {
                case 110:
                    Formular5_1();
                    break;
                case 105:
                    Formular5_2();
                    break;
                case 251:
                    Formular12_5();
                    break;
                default:
                    break;
            }
        }
        
        if(Getpos_e==3152){
            switch (Getpos_k) {
                case 110:
                    Formular10_4();
                    break;
                case 105:
                    Formular10_1();
                    break;
                case 215:
                    Formular12_3();
                    break;
                case 251:
                    Formular12_4();
                    break;
                default:
                    break;
            }
        }
        if(Getpos_e==3521){
            switch (Getpos_k) {
                case 110:
                    Formular10_2();
                    break;
                case 105:
                    Formular10_3();
                    break;
                case 215:
                    Formular12_1();
                    break;
                case 251:
                    Formular12_2();
                    break;
                default:
                    break;
            }
        }
        
        wuerfel_print(1);
    }
    change_topface(0);
    wuerfel_print(1);
    return 0;
}

int Basis_1()      //Getpos_e==1510 && Getpos_k==103
{
    rs_dreh(1, 90);
    rs_dreh(1,90);
    ls_dreh(-1, 90);
    return 0;
}

int Basis_2()       //Getpos_e==1105 && Getpos_k==140
{
    
    drehen(-1);
    rs_dreh(-1, 90);
    rs_dreh(-1,90);
    ls_dreh(1, 90);
    return 0;
}

int Basis_3()     //Getpos_e==1510 && Getpos_k==110
{
    drehen(-1);
    us_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    
    return 0;
}

int Basis_4()         //Getpos_e==1105 && Getpos_k==105
{
    us_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    return 0;
}

void Formular2_1()            //Getpos_e==1510 && Getpos_k==104
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}

void Formular2_2()                 //Getpos_e==1510 && Getpos_k==101
{
    us_dreh(1, 90);
    drehen(1);
    ls_dreh(-1, 90);
    rs_dreh(1, 180);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}

void Formular2_3()               //Getpos_e==1510 && Getpos_k==105
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}

void Formular2_4()        //Getpos_e==1105 && Getpos_k==130
{
    us_dreh(1, 90);
    drehen(-1);           //ACHTEN
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular2_5()             //Getpos_e==1105 && Getpos_k==150
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 180);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular2_6()                   //Getpos_e==1105 && Getpos_k==110
{
    us_dreh(-1, 90);
    drehen(-1);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 90);         //?????????????????????????????????????????????????????????????
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular3_1()         //Getpos_e==1105 && Getpos_k==104
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 180);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
}

void Formular3_2()           //Getpos_e==1105 && Getpos_k==103
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
}

void Formular3_3()            //Getpos_e==1510 && Getpos_k==130
{
    us_dreh(-1, 90);
    drehen(-1);
    ls_dreh(-1, 90);
    rs_dreh(1, 180);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}

void Formular3_4()           //Getpos_e==1510 && Getpos_k==140
{
    us_dreh(1, 90);
    drehen(1);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}

void Formular4_1()           //Getpos_e==1051 && Getpos_k==140
{
    drehen(-1);    //
    us_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 180);
    ls_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}


void Formular4_2()         //Getpos_e==1051 && Getpos_k==103
{
    us_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 180);
    ls_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
}

void Formular4_3()               //Getpos_e==1051 && Getpos_k==104
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 180);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}

void Formular4_4()             //Getpos_e==1051 && Getpos_k==130
{
    drehen(-1);
    rs_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 180);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular6_1()      //Getpos_e==1510 && Getpos_k==251
{
    drehen(-1);
    us_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    drehen(1);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}


void Formular6_2()               //Getpos_e==1105 && Getpos_k==251
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}


void Formular6_3()               //Getpos_e==1510 && Getpos_k==215
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 180);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}

void Formular6_4()               //Getpos_e==1105 && Getpos_k==215
{
    us_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
}

void Formular7_1()               //Getpos_e==1051 && Getpos_k==105
{
    rs_dreh(1, 90);
    rs_dreh(-1, 180);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}


void Formular7_2()             //Getpos_e==1051 && Getpos_k==110
{
    drehen(-1);
    rs_dreh(-1, 90);
    rs_dreh(1, 180);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular8_1()            //Getpos_e==1510 && Getpos_k==150
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ds_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular8_2()           //Getpos_e==1105 && Getpos_k==101
{
    drehen(-1);
    rs_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ds_dreh(-1, 90);
    drehen(-1);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}

void Formular9_1()          //Getpos_e==1051 && Getpos_k==251
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(-1, 180);
    drehen(-1);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}

void Formular9_2()       //Getpos_e==1051 && Getpos_k==215
{
    int i;
    us_dreh(1,90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    for (i=0; i<2; i++) {
        rs_dreh(1, 90);
        ls_dreh(1, 90);
        rs_dreh(-1, 90);
        ls_dreh(-1, 90);
    }
}

void Formular11_1()  //Getpos_e==1051 && Getpos_k==150
{
    rs_dreh(1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    drehen(1);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}


void Formular11_2()         //Getpos_e==1051 && Getpos_k==101
{
    us_dreh(1, 180);
    ls_dreh(1, 180);
    rs_dreh(1, 180);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 180);
}


void Formular5_1()        //Getpos_e==3215 && Getpos_k==110
{
    us_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}

void Formular5_2()         //Getpos_e==3215 && Getpos_k==105
{
    us_dreh(-1, 90);
    fs_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
}


void Formular10_1()          //Getpos_e==3152 && Getpos_k==105
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    
}

void Formular10_2()           //Getpos_e==3521 && Getpos_k==110
{
    drehen(-1);
    rs_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
}

void Formular10_3()         //Getpos_e==3521 && Getpos_k==105
{
    rs_dreh(1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_1();
}


void Formular10_4()     //Getpos_e==3152 && Getpos_k==110
{
    drehen(-1);
    rs_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

void Formular12_1()     //Getpos_e==3521 && Getpos_k==215
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 180);
    ls_dreh(-1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
    
}


void Formular12_2()           //Getpos_e==3521 && Getpos_k==251
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ds_dreh(1, 90);
    drehen(1);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_3();
    
}

void Formular12_3()        //Getpos_e==3152 && Getpos_k==215
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_4();
    
}


void Formular12_4()     //Getpos_e==3152 && Getpos_k==251
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    fs_dreh(-1, 90);
    rs_dreh(-1, 90);
    ls_dreh(1, 90);
}


void Formular12_5()        //Getpos_e==3215 && Getpos_k==251
{
    rs_dreh(1, 90);
    rs_dreh(-1, 90);
    ds_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    ls_dreh(1, 90);
    rs_dreh(-1, 90);
    ds_dreh(-1, 90);
    ls_dreh(-1, 90);
    rs_dreh(1, 90);
    change_topface(2);
    change_rightface(color_ecken[i]);
    Basis_2();
}

