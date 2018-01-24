//
//  main.c
//  neo64tron
//
//  Created by Sam Anguiano on 1/23/18.
//  Copyright © 2018 dodecaplexor. All rights reserved.
//

#include <unistd.h>

#include <stdio.h>

void delay(int tim) {
    usleep(1000*tim);
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

void out(int n,char bike) {
    pixels[n]=bike;
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
            out(i*8+j,' ');
        }
    }
    bike1[0]=0;
    bike1[1]=3;
    bike1[2]=0;
    bike2[0]=7;
    bike2[1]=4;
    bike2[2]=2;
    pixels[3*8+0]='X';
    pixels[4*8+7]='O';
    printpix();
}

int psin(int d) {
    return -(2*((int)(d%4)/2)-1)*d%2;
}
int pcos(int d) {
    return -(2*((int)((d+1)%4)/2)-1)*(d+1)%2;
}

void setup() {
    
}

void loop() {
    clearpix();
    for(int i=0; i<bikepl[iter]; i++) {
        bike1[2]+=bikep1[iter][i];
        bike1[0]+=pcos(bike1[2]);
        bike1[1]+=psin(bike1[2]);
        bike2[2]+=bikep2[iter][i];
        bike2[0]+=pcos(bike2[2]);
        bike2[1]+=psin(bike2[2]);
        out(bike1[1]*8+bike1[0],'X');
        out(bike2[1]*8+bike2[0],'O');
        delay(100);
        printpix();
    }
    iter++;
    iter=iter%3;
    delay(1000);
}

int main() {
    setup();
    while(1) {
        loop();
    }
    return 0;
}
