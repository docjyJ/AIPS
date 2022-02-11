#include <stdlib.h>
#include "tsock_lib.h"

Status * newStatus(){
    Status * s = malloc(sizeof(Status));
    s->messageNb = 10;
    s->isSink = 0;
    s->isUdp = 0;
    s->isUdp = 0;
    return s;
}

void setMessageNb(Status * s, int nb){
    s->messageNb = nb;
}
int getMessageNb(Status * s){
    return s->messageNb;
}

void setTcp(Status * s){
    s->isUdp = 0;
}

int isTcp(Status * s){
    return (int) s->isUdp-1;
}

void setUdp(Status * s){
    s->isUdp = 1;
}

int isUdp(Status * s){
    return (int) s->isUdp;
}

void setSink(Status * s){
    s->isSink = 1;
    s->isSource = 0;
}

int isSink(Status * s){
    return (int) s->isSink;
}

void setSource(Status * s){
    s->isSink = 0;
    s->isSource = 1;
}

int isSource(Status * s){
    return (int) s->isSource;
}
