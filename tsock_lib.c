/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
#include "tsock_lib.h"
#include "tsock_error.h"


void construire_message(char *message, char motif, int lg) {
    int i;
    for (i=0;i<lg;i++)
        message[i] = motif;
}
void afficher_message(char *message, int lg) {
    int i;
    printf("message construit : ");
    for (i=0;i<lg;i++)
        printf("%c", message[i]); printf("\n");
}


Statue * parseParameter(int argc, char **argv){
    int i;
    Statue * s = malloc(sizeof(Statue));
    memset(&s->adresse, 0, sizeof(s->adresse));
    s->command = argv[0];
    s->host = NULL;
    s->messageNb = 10;
    s->messageLength = 30;
    s->isSink = 0;
    s->isSource = 0;
    s->isUdp = 0;

    while ((i = getopt(argc, argv, "psun:l:")) != -1) {
        switch (i) {
            case 'p':
                if (s->isSource)
                    usageErrorDetailed("Too many argument! (-s and -p)", s->command);
                s->isSink = 1;
                break;

            case 's':
                if (s->isSink)
                    usageErrorDetailed("Too many argument! (-s and -p)", s->command);
                s->isSource = 1;
                break;

            case 'n':
                s->messageNb = atoi(optarg); /* TODO Gestion d'erreur */
                break;

            case 'l':
                s->messageNb = atoi(optarg); /* TODO Gestion d'erreur */
                break;

            case 'u':
                s->isUdp = 1;
                break;

            default:
                usageError(s->command);
                break;
        }
    }

    if (s->isSource-s->isSink) {
        i =  argc - optind;
        printf("%d", i);
        if (s->isSink){
            if(i < 1)
                usageErrorDetailed("Too many argument!", s->command);
            if(i > 1)
                usageErrorDetailed("Missing argument!", s->command);
            s->port = atoi(argv[optind]);
        }
        else {
            if(i < 2)
                usageErrorDetailed("Too many argument!", s->command);
            if(i > 2)
                usageErrorDetailed("Missing argument!", s->command);
            s->host = argv[optind];
            s->port = atoi(argv[optind+1]);
        }
    }
    else {
        usageErrorDetailed("Missing argument! (-s or -p)", s->command);
    }
    return s;
};

void openConnexion(Statue * s){
    // Filling server information
    s->adresse.sin_family = AF_INET;
    s->adresse.sin_port = htons(s->port);
    if(s->host){
        struct hostent *he;

        if(!(he = gethostbyname(s->host))){
            herror("gethostbyname");
            exit(EXIT_FAILURE);
        }

        s->adresse.sin_addr.s_addr = *((unsigned long*)he->h_addr_list[0]);

    }
    else{
        s->adresse.sin_addr.s_addr = INADDR_ANY;
    }

    if (s->isUdp         ){
        if((s->socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
    }
}

void closeConnexion(Statue * s){
    close(s->socket);
    free(s);
}
