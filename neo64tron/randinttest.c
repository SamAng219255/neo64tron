#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
int main(int argc,char* argv[]) {
    srand((unsigned int)time(NULL));
    printf("%i\n",randint(0,atoi(argv[1])));
    return 0;
}
