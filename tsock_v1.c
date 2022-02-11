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

void error(char* error, char * cmd, int code){
    printf("%s\nusage: %s [-p|-s][-n ##]\n", error, cmd);
    exit(code);
}

int main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
    Status * status = newStatus();
	while ((c = getopt(argc, argv, "pn:s")) != -1) {
		switch (c) {
            case 'p':
                if (isSource(status))
                    error("Too many argument! (-s with -p)", argv[0], 3);
                setSink(status);
                break;

            case 's':
                if (isSink(status))
                    error("Too many argument! (-s with -p)", argv[0], 3);
                setSource(status);
                break;

            case 'n':
                setMessageNb(status, optind);
                break;

            case 'u':
                setUdp(status);
                break;


            case ':':
                error("Missing argument!", argv[0], 2);
                break;

            case '?':
                error("Unrecognized option!", argv[0], 1);
                break;

            default:
                error("intern error!", argv[0], 42);
                break;
		}
	}

	if (isSource(status)-isSink(status)) {
        if (isSource(status))
            printf("on est dans le source\n");
        else
            printf("on est dans le puits\n");
	}
    else {
        error("Missing argument! (-s or -p)", argv[0], 3);
    }

    return 0;
}

