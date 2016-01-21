//
//  Color_search.c
//  Cube3*3
//
//  Created by 杨春霞 on 12.11.15.
//  Copyright © 2015 Chunxia Yang. All rights reserved.
//

#include "Color_search.h"
#include "main.h"



int Kantenstein_suchen(int Color1,int Color2)
{
    int kantPos=0; //kantPos=100*Schicht+10*Richtung erster Seite+Richtung zweiter Seite
    int c1=0,c2=0;
    
    //Erste_schicht
    c1= color_data[0][1]; c2= color_data[3][7];
    if (c1==Color1 && c2==Color2) {kantPos=103;}//weiss_Grün Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=130;}
    c1= color_data[0][3]; c2= color_data[4][7];
    if (c1==Color1 && c2==Color2) {kantPos=104;}//weiss_Rot Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=140;}
    c1= color_data[0][5]; c2= color_data[5][7];
    if (c1==Color1 && c2==Color2) {kantPos=105;}//weiss_Orange Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=150;}
    c1= color_data[0][7]; c2= color_data[1][1];
    if (c1==Color1 && c2==Color2) {kantPos=101;}//weiss_Blau Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=110;}
    
    
    //Zweite_schicht
    c1= color_data[4][3]; c2= color_data[1][3];
    if (c1==Color1 && c2==Color2) {kantPos=241;}//Rot_Blau Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=214;}
    c1= color_data[1][5]; c2= color_data[5][5];
    if (c1==Color1 && c2==Color2) {kantPos=215;}//Blau_Orange Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=251;}
    c1= color_data[5][3]; c2= color_data[3][5];
    if (c1==Color1 && c2==Color2) {kantPos=253;}//Orange_Grün Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=235;}
    c1= color_data[3][3]; c2= color_data[4][5];
    if (c1==Color1 && c2==Color2) {kantPos=234;}//Grün_Rot Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=243;}
    
    
    //Dritte_schicht
    c1= color_data[1][7]; c2= color_data[2][1];
    if (c1==Color1 && c2==Color2) {kantPos=312;}//Blau_Gelb Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=321;}
    c1= color_data[5][1]; c2= color_data[2][5];
    if (c1==Color1 && c2==Color2) {kantPos=352;}//Orange_Gelb Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=325;}
    c1= color_data[3][1]; c2= color_data[2][7];
    if (c1==Color1 && c2==Color2) {kantPos=332;}//Grün_Gelb Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=323;}
    c1= color_data[4][1]; c2= color_data[2][3];
    if (c1==Color1 && c2==Color2) {kantPos=342;}//Rot_Gelb Kantenstein
    if (c1==Color2 && c2==Color1) {kantPos=324;}
    
    return kantPos;
}


int Mittelstein_suchen(int Color)
{
    int i;
    int c1=0,Centrale=6;
    for (i=0; i<=5; i++) {
        c1= color_data[i][4];
        if(c1==Color) Centrale=i;
    }
    return Centrale;
}


int Eckenstein_suchen(int Color1, int Color2, int Color3 )
{
    int eckPos=0; //EckPos= 1000*Schicht+100*Richtung erster Seite+10*Richtung zweiter Seite+Richtung dritte Seite
    int Color_eck=100*Color1+10*Color2+Color3;
    
    //Erste Schichte
    if (Color_eck== 100*color_data[0][0]+10*color_data[4][8]+color_data[3][6])   //1043
        eckPos=1000*1+100*0+10*4+3;
    if (Color_eck== 100*color_data[4][8]+10*color_data[3][6]+color_data[0][0])   //1430
        eckPos=1000*1+100*4+10*3+0;
    if (Color_eck== 100*color_data[3][6]+10*color_data[0][0]+color_data[4][8])   //1304
        eckPos=1000*1+100*3+10*0+4;
    
    if (Color_eck== 100*color_data[0][2]+10*color_data[3][8]+color_data[5][6])   //1035
        eckPos=1000*1+100*0+10*3+5;
    if (Color_eck== 100*color_data[3][8]+10*color_data[5][6]+color_data[0][2])   //1350
        eckPos=1000*1+100*3+10*5+0;
    if (Color_eck== 100*color_data[5][6]+10*color_data[0][2]+color_data[3][8])   //1503
        eckPos=1000*1+100*5+10*0+3;
    
    if (Color_eck== 100*color_data[0][8]+10*color_data[5][8]+color_data[1][2])   //1051
        eckPos=1000*1+100*0+10*5+1;
    if (Color_eck== 100*color_data[5][8]+10*color_data[1][2]+color_data[0][8])   //1510
        eckPos=1000*1+100*5+10*1+0;
    if (Color_eck== 100*color_data[1][2]+10*color_data[0][8]+color_data[5][8])   //1105
        eckPos=1000*1+100*1+10*0+5;
    
    if (Color_eck== 100*color_data[0][6]+10*color_data[1][0]+color_data[4][6])   //1014
        eckPos=1000*1+100*0+10*1+4;
    if (Color_eck== 100*color_data[1][0]+10*color_data[4][6]+color_data[0][6])   //1140
        eckPos=1000*1+100*1+10*4+0;
    if (Color_eck== 100*color_data[4][6]+10*color_data[0][6]+color_data[1][0])   //1401
        eckPos=1000*1+100*4+10*0+1;
    
    //Zweite Schichte
    if (Color_eck== 100*color_data[2][0]+10*color_data[4][0]+color_data[1][6])   //3241
        eckPos=1000*3+100*2+10*4+1;
    if (Color_eck== 100*color_data[4][0]+10*color_data[1][6]+color_data[2][0])   //3412
        eckPos=1000*3+100*4+10*1+2;
    if (Color_eck== 100*color_data[1][6]+10*color_data[2][0]+color_data[4][0])   //3124
        eckPos=1000*3+100*1+10*2+4;
    
    if (Color_eck== 100*color_data[2][2]+10*color_data[1][8]+color_data[5][2])   //3215
        eckPos=1000*3+100*2+10*1+5;
    if (Color_eck== 100*color_data[1][8]+10*color_data[5][2]+color_data[2][2])   //3152
        eckPos=1000*3+100*1+10*5+2;
    if (Color_eck== 100*color_data[5][2]+10*color_data[2][2]+color_data[1][8])   //3521
        eckPos=1000*3+100*5+10*2+1;
    
    if (Color_eck== 100*color_data[2][8]+10*color_data[5][0]+color_data[3][2])   //3253
        eckPos=1000*3+100*2+10*5+3;
    if (Color_eck== 100*color_data[5][0]+10*color_data[3][2]+color_data[2][8])   //3532
        eckPos=1000*3+100*5+10*3+2;
    if (Color_eck== 100*color_data[3][2]+10*color_data[2][8]+color_data[5][0])   //3325
        eckPos=1000*3+100*3+10*2+5;
    
    if (Color_eck== 100*color_data[2][6]+10*color_data[3][0]+color_data[4][2])   //3234
        eckPos=1000*3+100*2+10*3+4;
    if (Color_eck== 100*color_data[4][2]+10*color_data[2][6]+color_data[3][0])   //3423
        eckPos=1000*3+100*4+10*2+3;
    if (Color_eck== 100*color_data[3][0]+10*color_data[4][2]+color_data[2][6])   //3342
        eckPos=1000*3+100*3+10*4+2;
    
    return eckPos;
    
}
