#include <stdio.h>
#include "tsock_lib.h"

const char hello[] = "Hello serv !";

int main (int argc, char **argv) {
    Statue * s = parseParameter(argc, argv);
    openConnexion(s);
    if(s->isSource){
        printf("SOURCE : lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=%s, dest=%s\n",
               s->messageLength, s->port,s->messageNb, s->isUdp?"UDP":"TCP", s->host);
        for(int i = 0; i < s->messageNb; i++)
            sendMessages(s, i);
    }
    closeConnexion(s);
    return 0;
}

