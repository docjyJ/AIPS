#ifndef AIPS_LIB_TSOCK_H
#define AIPS_LIB_TSOCK_H

typedef struct {
    char * command;
    int socket;
    struct sockaddr_in adresse;
    char * host;
    int port;
    int messageNb;
    int messageLength;
    unsigned char isSink : 1;
    unsigned char isSource : 1;
    unsigned char isUdp : 1;
} Statue;

Statue * parseParameter(int argc, char **argv);
void openConnexion(Statue * s);
void closeConnexion(Statue * s);

#endif //AIPS_LIB_TSOCK_H
