#include <Adafruit_NeoPixel.h>
#include <time.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6

#define NUMPIXELS      64

#define BRIGHTNESS      25

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int safe=1;
char pixstor[64] ={
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' '
};

void setup() {
  Serial.begin(9600);
      randomSeed(analogRead(0));
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
}


int psin(int d) {
    return -(2*((int)(d%4)/2)-1)*d%2;
}
int pcos(int d) {
    return -(2*((int)((d+1)%4)/2)-1)*(d+1)%2;
}

int mod(int x,int y) {
  int total=x%y;
  if(total<0) {
    total+=y;
  }
  return total;
}

int bike1[3]={0,3,0};
int bike2[3]={7,4,2};
int bikepl[3]={8,4,15};
int bikep1[3][16]={
    {0,0,0,-1,-1,1,1,0},
    {1,-1,0,0},
    {0,0,0,-1,0,0,-1,-1,0,1,1,0,-1,-1,0}
};
int bikep2[3][16]={
    {0,0,0,-1,1,0,-1,0},
    {0,0,0,0},
    {0,0,0,0,0,0,0,-1,0,0,-1,-1,1,1,-1}
};

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
        Serial.println(debugstr);
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
        Serial.println(debugstr);
}

int iter=0;

void emptypix() {
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
  pixels.show();
}

void out(int x,int y,char bike) {
  if((x<0 or x>=8) and (y<0 or y>=8)) {
    safe=0;
  }
  else {
    if(pixstor[y*8+x]!=' ') {
      safe=0;
    }
    int r=0;
    int g=32;
    int b=0;
    if(bike=='X') {
      r=64;
    }
    else {
      if(bike=='O') {
        b=64;
      }
      else {
        g=0;
      }
    }
    pixels.setPixelColor(y*8+x,pixels.Color(r,g,b));
    pixstor[y*8+x]=bike;
  }
}

void loop() {
  randomSeed(analogRead(0));
    emptypix();
  safe=1;
    //for(int i=0; i<bikepl[iter]; i++) {preset paths
    while(safe) {
        //bike1[2]+=bikep1[iter][i];//preset paths
        bike1[2]+=rand()/(RAND_MAX/2)+rand()/(RAND_MAX/2)-1;//random paths
        bike1[2]=mod(bike1[2],4);
        bike1[0]+=pcos(bike1[2]);
        bike1[1]+=psin(bike1[2]);
        //bike2[2]+=bikep2[iter][i];//preset paths
        bike2[2]+=rand()/(RAND_MAX/2)+rand()/(RAND_MAX/2)-1;//random paths
        bike2[2]=mod(bike2[2],4);
        bike2[0]+=pcos(bike2[2]);
        bike2[1]+=psin(bike2[2]);
        out(bike1[1],bike1[0],'X');
        out(bike2[1],bike2[0],'O');
        debugtxt();
        delay(100);
        pixels.show();
    }
    iter++;
    iter=iter%3;
    delay(1000);
    Serial.println(safe);
}
