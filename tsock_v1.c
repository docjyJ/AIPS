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

const char hello[] = "Hello serv !";

int main (int argc, char **argv) {
    Statue * s = parseParameter(argc, argv);
    openConnexion(s);


    sendto(s->socket, hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &s->adresse,
           sizeof(s->adresse));

    closeConnexion(s);
    return 0;
}

