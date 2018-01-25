//
//  main.c
//  neo64tron
//
//  Created by Sam Anguiano on 1/23/18.
//  Copyright © 2018 dodecaplexor. All rights reserved.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void itoa(int x,char* target,int base) {
    char chars[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZß@";
    int size=(int)log(x)/log(base);
    char total[size];
    int temp=x;
    for(int i=size-1; i>=0; i--) {
        total[i]=chars[(int)(temp/pow(base,i))];
        temp%=(int)pow(base,i);
    }
    target=total;
}

void delay(int tim) {
    //usleep(1000*tim);
}

int randint(int min,int max) {
    int delta=-1;
    int outcome=0;
    while(delta<=0) {
        int foo=(int)pow(2,ceil(log2(max-min)));
        outcome=(int)(rand()%foo)+min;
        delta=max-outcome;
    }
    return outcome;
}

int mod(int x,int y) {
    int total=x%y;
    if(total<0) {
        total+=y;
    }
    return total;
}

int min(int x, int y) {
    if(y<x) {
        return y;
    }
    else {
        return x;
    }
}
int max(int x, int y) {
    if(y>x) {
        return y;
    }
    else {
        return x;
    }
}

int sign(int x) {
    if(x<0) {
        return -1;
    }
    else {
        return 1;
    }
}
int abs(int x) {
    return sign(x)*x;
}

char pixels[64] ={
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' '
};

int iter=0;

int safe=1;

void out(int x,int y,char bike) {
    if((x<0 || x>=8) && (y<0 || y>=8)) {
        safe=0;
    }
    else {
        if(pixels[y*8+x]!=' ') {
            safe=0;
        }
        pixels[y*8+x]=bike;
    }
}

void printpix() {
    for(int i=0; i<8; i++) {
        char minipix[8];
        for(int j=0; j<8; j++) {
            minipix[j]=pixels[i*8+j];
        }
        printf("%s\n",minipix);
    }
}

int bike1[3]={0,3,0};
int bike2[3]={7,4,2};
int bikepl[3]={8,4,15};
int bikep1[3][16]={
    {0,0,0,-1,-1,1,1,0},
    {1,-1,0,0},
    {0,0,-1,0,0,-1,-1,0,1,1,0,-1,-1,0}
};
int bikep2[3][16]={
    {0,0,0,-1,1,0,-1,0},
    {0,0,0,0},
    {0,0,0,0,0,0,0,-1,0,0,-1,-1,1,1,-1}
};

void clearpix() {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            out(i,j,' ');
        }
    }
    bike1[0]=0;
    bike1[1]=3;
    bike1[2]=0;
    bike2[0]=7;
    bike2[1]=4;
    bike2[2]=2;
    out(3,0,'X');
    out(4,7,'O');
    printpix();
}

int psin(int d) {
    return -(2*((int)(d%4)/2)-1)*d%2;
}
int pcos(int d) {
    return -(2*((int)((d+1)%4)/2)-1)*(d+1)%2;
}

void debugtxt() {
    char xstr[4];
    char ystr[4];
    char fstr[4];
    char dxstr[2];
    char dystr[2];
    itoa(bike1[0],xstr,10);
    itoa(bike1[1],ystr,10);
    itoa(bike1[2],fstr,10);
    itoa(pcos(bike1[2]),dxstr,10);
    itoa(psin(bike1[2]),dystr,10);
    char debugstr[50];
    strcpy(debugstr,"Red Bike: x:");
    strcat(debugstr,xstr);
    strcat(debugstr," y:");
    strcat(debugstr,ystr);
    strcat(debugstr," facing:");
    strcat(debugstr,fstr);
    strcat(debugstr," delta:");
    strcat(debugstr,dxstr);
    strcat(debugstr,",");
    strcat(debugstr,dystr);
    printf("%s\n",debugstr);
    itoa(bike2[0],xstr,10);
    itoa(bike2[1],ystr,10);
    itoa(bike2[2],fstr,10);
    itoa(pcos(bike2[2]),dxstr,10);
    itoa(psin(bike2[2]),dystr,10);
    strcpy(debugstr,"Blu Bike: x:");
    strcat(debugstr,xstr);
    strcat(debugstr," y:");
    strcat(debugstr,ystr);
    strcat(debugstr," facing:");
    strcat(debugstr,fstr);
    strcat(debugstr," delta:");
    strcat(debugstr,dxstr);
    strcat(debugstr,",");
    strcat(debugstr,dystr);
    printf("%s\n",debugstr);
}

void setup() {
    
}

void loop() {
    clearpix();
    safe=1;
    for(int i=0; i<bikepl[iter]; i++) {//preset paths
    //while(safe) {
        bike1[2]+=bikep1[iter][i];//preset paths
        //bike1[2]+=randint(0,2)+randint(0,2)-1;//random paths
        bike1[2]=mod(bike1[2],4);
        bike1[0]+=pcos(bike1[2]);
        bike1[1]+=psin(bike1[2]);
        bike2[2]+=bikep2[iter][i];//preset paths
        //bike2[2]+=randint(0,2)+randint(0,2)-1;//random paths
        bike2[2]=mod(bike2[2],4);
        bike2[0]+=pcos(bike2[2]);
        bike2[1]+=psin(bike2[2]);
        out(bike1[1],bike1[0],'X');
        out(bike2[1],bike2[0],'O');
        //debugtxt();
        delay(100);
        printpix();
    }
    iter++;
    iter=iter%3;
    delay(100);
}

int main() {
    setup();
    while(1) {
        loop();
    }
    return 0;
}
