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

#include "tsock_error.h"

typedef struct {
    unsigned short messageLength;
    int messageNb : 17;
    int isSink : 1;
    int isSource : 1;
    int isUdp : 1;
} Statue;

int open(int isUdp, char* cmd){
    int s;
    if (isUdp) {
        if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            errorNetworkP(cmd);
    }
    else {
        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            errorNetworkP(cmd);
    }
    return s;
}

int main (int argc, char **argv) {
    /* Test du cas sans argument pour afficher l'usage */
    if(argc==1)
        usage(argv[0]);

    /* Initialisation des Variables */
    unsigned int i;
    char *buffer;
    long long value;
    int socket;
    struct sockaddr_in adresse;
    bzero(&adresse, sizeof(adresse));
    adresse.sin_family = AF_INET;
    Statue s;
    s.messageNb = 0;
    s.messageLength = 30;
    s.isSink = 0;
    s.isSource = 0;
    s.isUdp = 0;

    /* Récupération des options */
    while ((i = getopt(argc, argv, "psun:l:")) != -1) {
        switch (i) {
            case 'p':
                if (s.isSource)
                    errorDetailed("you cannot use the -s option with the -p option", argv[0]);
                s.isSink = 1;
                break;

            case 's':
                if (s.isSink)
                    errorDetailed("you cannot use the -s option with the -p option", argv[0]);
                s.isSource = 1;
                break;

            case 'n':
                value = strtol(optarg, &buffer, 10);
                if(*buffer != '\0')
                    errorDetailed("the -n argument is not a number", argv[0]);
                if(value < 1)
                    errorDetailed("the -n argument must be greater than or equal to 1", argv[0]);
                if(value > 99999)
                    errorDetailed("the -n argument must be less than or equal to 99999", argv[0]);
                s.messageNb = (int) value;
                break;

            case 'l':
                value = strtol(optarg, &buffer, 10);
                if(*buffer != '\0')
                    errorDetailed("the -l argument is not a number", argv[0]);
                if(value < 5)
                    errorDetailed("the -l argument must be greater than or equal to 5", argv[0]);
                if(value > 65507)
                    errorDetailed("the -l argument must be less than or equal to 65507", argv[0]);
                s.messageLength = value;
                break;

            case 'u':
                s.isUdp = 1;
                break;

            default:
                error();
                break;
        }
    }
    i =  argc - optind;

    /* Source */
    if (s.isSource){
        /* Valeur par default pour le nombre de messages */
        if(!s.messageNb)
            s.messageNb = 10;

        /* Recuperation des arguments */
        if(i > 2)
            errorDetailed("too many arguments", argv[0]);
        if(i == 0)
            errorDetailed("missing host name and port number", argv[0]);
        if(i == 1)
            errorDetailed("missing port number", argv[0]);

        /* Port */
        value = strtol(argv[optind+1], &buffer, 10);
        if(*buffer != '\0')
            errorDetailed("the port is not a number", argv[0]);
        if(value < 1024)
            errorDetailed("the port must be greater than or equal to 1024", argv[0]);
        if(value > 65535)
            errorDetailed("the port must be less than or equal to 65535", argv[0]);
        adresse.sin_port = htons((unsigned short) value);

        /* Host */
        struct hostent *he;
        if(!(he=gethostbyname(argv[optind])))
            errorNetworkH(argv[0]);
        else
            adresse.sin_addr.s_addr = *((unsigned long*)he->h_addr_list[0]);

        /* Socket */
        socket = open(s.isUdp, argv[0]);

        /* Affichage */
        printf("SOURCE : lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=%s, dest=%s\n",
               s.messageLength, (unsigned short) value, s.messageNb, s.isUdp?"UDP":"TCP", argv[optind]);

        /* Connexion au serveur */
        struct sockaddr *addAdresse = (struct sockaddr *) &adresse;
        int lenAdresse = sizeof(struct sockaddr);
        if(!s.isUdp){
            if (connect(socket, addAdresse, lenAdresse))
                errorNetworkP(argv[0]);
        }

        /* Envoi des messages */
        buffer = malloc((s.messageLength+1)*sizeof(char));
        for(i = 0; i < s.messageNb; i++) {
            sprintf(buffer, "%5d", i+1);
            memset(buffer+5,(char)(97 + i%26),s.messageLength-5);
            buffer[s.messageLength] = 0;

            if(sendto(socket, buffer, s.messageLength, 0, addAdresse, lenAdresse) != s.messageLength)
                errorNetworkP(argv[0]);

            printf("SOURCE: Envoi n°%5d (%5d) [%s]\n", i+1, s.messageLength, buffer);
        }

        /* Fermeture du socket */
        close(socket);
    }

    /* Puits */
    else if (s.isSink) {
        /* Recuperation des arguments */
        if(i > 1)
            errorDetailed("too many argument", argv[0]);
        if(i == 0)
            errorDetailed("missing port number", argv[0]);

        /* Port */
        value = strtol(argv[optind], &buffer, 10);
        if(*buffer != '\0')
            errorDetailed("the port is not a number", argv[0]);
        if(value < 1024)
            errorDetailed("the port must be greater than or equal to 1024", argv[0]);
        if(value > 65535)
            errorDetailed("the port must be less than or equal to 65535", argv[0]);
        adresse.sin_port = htons((unsigned short) value);

        /* Host */
        adresse.sin_addr.s_addr = INADDR_ANY;

        /* Socket */
        socket = open(s.isUdp, argv[0]);

        /* Écoute sur le port */
        if (bind(socket, (const struct sockaddr *) &adresse, sizeof(struct sockaddr)) < 0)
            errorNetworkP(argv[0]);

        /* Affichage */
        if(s.messageNb)
            printf("PUITS : lg_mesg=%d, port=%d, nb_receptions=%d, TP=%s\n",
                   s.messageLength, (unsigned short) value, s.messageNb, s.isUdp?"UDP":"TCP");
        else
            printf("PUITS : lg_mesg=%d, port=%d, nb_receptions=infini, TP=%s\n",
                   s.messageLength, (unsigned short) value, s.isUdp?"UDP":"TCP");

        /* Definition du client */
        unsigned int clientLen = sizeof(struct sockaddr);
        struct sockaddr client;
        struct sockaddr *addClient = &client;
        unsigned int *addClientLen = &clientLen;
        bzero(addClient, value);
        if(!s.isUdp){
            if (listen(socket, 0))
                errorNetworkP(argv[0]);

            if (accept(socket, addClient, addClientLen) < 0)
                errorNetworkP(argv[0]);

            addClient = NULL;
            clientLen = 0;
        }

        /* Envoi des messages */
        buffer = malloc((s.messageLength+1)*sizeof(char));
        i=0;
        while(!s.messageNb || i < s.messageNb){
            value = recvfrom(socket, buffer, s.messageLength, 0,  addClient,addClientLen);
            buffer[value] = '\0';
            printf("PUITS: Envoi n°%5d (%5d) [%s]\n", i+1, (int) strlen(buffer), buffer);
            if(i>99997)
                i=0;
            else
                i++;
        }
        free(buffer);

        /* Fermeture du socket */
        close(socket);
    }

    /* fin du programme */
    else
        errorDetailed("missing argument -s or -p option", argv[0]);
    return 0;
}

