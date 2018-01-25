//
//  tronlearning.c
//  neo64tron
//
//  Created by Sam Anguiano on 1/24/18.
//  Copyright © 2018 dodecaplexor. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int pi=3.141592653589793238462643383279502884197169;

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

int inv(int x) {
    return -1*(1-abs(x))*sign(x);
}

int gather(int x,int d, int way) {
    int outcome=x;
    switch(way) {
        case 0 :
            outcome=x*d;
            break;
        case 1 :
            outcome=inv(inv(x)*inv(d));
            break;
        case 2 :
            outcome=inv(inv(x)*inv(d))*inv(x*d);
            break;
        case 3 :
            outcome=inv(x*d);
            break;
        case 4 :
            outcome=inv(x)*inv(d);
            break;
        case 5 :
            outcome=inv(inv(inv(x)*inv(d))*inv(x*d));
            break;
    }
    return outcome;
}

typedef struct {
    int id;
    int neurons[8];
    int neuroncount;
    int gathering;
    float value;
    int persistant;
} node;
typedef struct {
    node nodes[9];
    int neurons;
    int fitness;
} nnet;

int runAI(nnet ai1, nnet ai2,int iter,int leniter) {
    int fitness=cos(pi*iter/leniter)*ai1.neurons;
    
    return fitness;
}

nnet pool[1024];

int compare(const void *s1, const void *s2) {
    nnet *e1 = (nnet *)s1;
    nnet *e2 = (nnet *)s2;
    return e1->fitness - e2->fitness;
}

void sortPool() {
    qsort(pool, 1024, sizeof(nnet), compare);
}

void testPool(int iter,int leniter) {
    for(int i=0; i<1024; i++) {
        pool[i].fitness=0;
        pool[i].fitness+=runAI(pool[i],pool[randint(0, 1024)],iter,leniter);
        pool[i].fitness+=runAI(pool[i],pool[randint(0, 1024)],iter,leniter);
        pool[i].fitness+=runAI(pool[i],pool[randint(0, 1024)],iter,leniter);
        pool[i].fitness+=runAI(pool[i],pool[randint(0, 1024)],iter,leniter);
    }
}

void growPool(int i) {
    if(i==1) {
        for(int i=0; i<1024; i++) {
            nnet newnet;
            newnet.fitness=0;
            newnet.neurons=0;
            for(int j=0; j<9; j++) {
                node newnode;
                newnode.gathering=randint(0,6);
                newnode.id=j;
                newnode.persistant=randint(0,2);
                newnode.value=(float)randint(-100,101)/100.0;
                int neuroncount=0;
                while(randint(0,2) && neuroncount<9) {
                    newnode.neurons[neuroncount]=randint(0, 9);
                    neuroncount++;
                    newnet.neurons++;
                }
                newnode.neuroncount=neuroncount;
                newnet.nodes[j]=newnode;
            }
            pool[i]=newnet;
        }
    }
    else {
        for(int i=0; i<512; i++) {
            int mutation=randint(0,4);
            nnet newnet=pool[i];
            int cnode=randint(0, 9);
            switch(mutation){
                case 0:
                    newnet.nodes[cnode].gathering=mod(newnet.nodes[randint(0,9)].gathering+randint(0,2)*2-1,6);
                    break;
                case 1:
                    newnet.nodes[cnode].persistant=randint(0,2);
                    break;
                case 2:
                    newnet.nodes[cnode].value+=min(max((randint(0,2)*2-1)/randint(2,42),-1),1);
                    break;
                case 3:
                    if(randint(0,2)) {
                        if(newnet.nodes[cnode].neuroncount<8) {
                            newnet.nodes[cnode].neurons[newnet.nodes[cnode].neuroncount]=randint(0, 9);
                            newnet.nodes[cnode].neuroncount++;
                            newnet.neurons++;
                        }
                        else {
                            newnet.nodes[cnode].neurons[randint(0,8)]=randint(0, 9);
                        }
                    }
                    else if(newnet.nodes[cnode].neuroncount>0) {
                        int rneuron=randint(0,newnet.nodes[cnode].neuroncount);
                        for(int j=rneuron; j<newnet.nodes[cnode].neuroncount-1; j++) {
                            newnet.nodes[cnode].neurons[j]=newnet.nodes[cnode].neurons[j+1];
                        }
                        newnet.nodes[cnode].neuroncount--;
                        newnet.neurons--;
                    }
                    break;
            }
            pool[i+512]=newnet;
        }
    }
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    int len=atoi(argv[1]);
    for(int i=1; i<=len; i++) {
        growPool(i);
        testPool(i,len);
        sortPool();
        if(len%i==0) {
            printf("Iteration: %i\n",i);
        }
    }
    return 0;
}
