#ifndef AIPS_LIB_TSOCK_H
#define AIPS_LIB_TSOCK_H

typedef struct {
    int messageNb;
    unsigned char isSink : 1;
    unsigned char isSource : 1;
    unsigned char isUdp : 1;
} Status;

Status * newStatus();

void setMessageNb(Status * s, int nb);
int getMessageNb(Status * s);

void setTcp(Status * s);
int isTcp(Status * s);

void setUdp(Status * s);
int isUdp(Status * s);

void setSink(Status * s);
int isSink(Status * s);

void setSource(Status * s);
int isSource(Status * s);

#endif //AIPS_LIB_TSOCK_H
