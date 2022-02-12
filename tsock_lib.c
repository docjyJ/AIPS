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



void afficher_message(char *message, int lg) {
    int i;
    printf("message construit : ");
    for(i=0;i<lg;i++)
        printf("%c", message[i]);
    printf("\n");
}


Statue * parseParameter(int argc, char **argv){
    if(argc==1)
        usage(argv[0]);

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

    char *endptr;
    long long value;

    while ((i = getopt(argc, argv, "psun:l:")) != -1) {
        switch (i) {
            case 'p':
                if (s->isSource)
                    errorDetailed("you cannot use the -s option with the -p option", s->command);
                s->isSink = 1;
                break;

            case 's':
                if (s->isSink)
                    errorDetailed("you cannot use the -s option with the -p option", s->command);
                s->isSource = 1;
                break;

            case 'n':
                value = strtol(optarg, &endptr, 10);
                 if(*endptr != '\0')
                     errorDetailed("-n argument is not a number", s->command);
                 if(value < 1)
                     errorDetailed("the -n argument must be greater than or equal to 1", s->command);
                if(value > 99999)
                    errorDetailed("the -n argument must be less than or equal to 99999", s->command);

                s->messageNb = value;
                break;

            case 'l':
                value = strtol(optarg, &endptr, 10);
                if(*endptr != '\0')
                    errorDetailed("-n argument is not a number", s->command);
                if(value < 5)
                    errorDetailed("the -n argument must be greater than or equal to 5", s->command);
                if(value > 65507)
                    errorDetailed("the -n argument must be less than or equal to 65507", s->command);

                s->messageLength = value;
                break;

            case 'u':
                s->isUdp = 1;
                break;

            default:
                error();
                break;
        }
    }

    if (s->isSource-s->isSink) {
        i =  argc - optind;
        if (s->isSink){
            if(i > 1)
                errorDetailed("too many argument", s->command);
            if(i == 0)
                errorDetailed("missing port number", s->command);
            s->port = atoi(argv[optind]);
        }
        else {
            if(i > 2)
                errorDetailed("too many arguments", s->command);
            if(i == 0)
                errorDetailed("missing host name and port number", s->command);
            if(i == 1)
                errorDetailed("missing port number", s->command);
            s->host = argv[optind];
            s->port = atoi(argv[optind+1]);
        }
    }
    else
        errorDetailed("missing argument -s or -p option", s->command);

    return s;
};

void openConnexion(Statue * s){
    s->adresse.sin_family = AF_INET;
    s->adresse.sin_port = htons(s->port);
    if(s->host){
        struct hostent *he;
        if(!(he=gethostbyname(s->host)))
            errorNetwork(s->command);
        s->adresse.sin_addr.s_addr = *((unsigned long*)he->h_addr_list[0]);
    }
    else
        s->adresse.sin_addr.s_addr = INADDR_ANY;

    if (s->isUdp){
        if((s->socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
            errorNetwork(s->command);
        }
    }
}

void closeConnexion(Statue * s){
    close(s->socket);
    free(s);
}

void sendMessages(Statue * s, int nb){
    char message[s->messageLength+1];
    sprintf(message, "%5d", nb+1);
    char motif = (char)(97 + nb%26);
    for (int i=5;i<s->messageLength;i++)
        message[i] = motif;
    message[s->messageLength] = 0;

    if(sendto(s->socket, message, s->messageLength,
           MSG_CONFIRM, (const struct sockaddr *) &s->adresse,
           sizeof(struct sockaddr)) != s->messageLength)
        errorDetailed("fail to send data", s->command);
    printf("SOURCE: Envoi n°%5d (%5d) [%s]\n", nb+1, s->messageLength, message);
}