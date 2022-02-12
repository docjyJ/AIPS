#include "tsock_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

const char usageTxt[] = "Usage: %s -p [-options] port \n"
                     "       %s -s [-options] host port \n"
                     "\n"
                     "Options communes:\n"
                     "       -uutilise le protocole UDP (défaut: TCP) \n"
                     "       -l ##    longueur du tampon lu ou écrit (défaut: 30)\n"
                     "\n"
                     "Options spécifiques à la source (-s) \n"
                     "       -d : Impose au service de transport TCP d'envoyer les données sans délai \n"
                     "       -n ##    définit le nombre d'émission (défaut: 10)\n"
                     "       -t ##définit la taille des buffer d'émission du niveau transport (par défaut, celle du système \n"
                     "\n"
                     "Options spécifiques au puits (-p) \n"
                     "       -n ## définit le nombre de réception (défaut: infini)\n"
                     "       -t ##définit la taille des buffers de réception du niveau transport (par défaut, celle du système\n";

void usage(char * cmd){
    printf(usageTxt, cmd, cmd);
    exit(EXIT_SUCCESS);
}

void errorDetailed(char * error, char * cmd){
    fprintf(stderr, "%s: %s\n", cmd, error);
    exit(EXIT_FAILURE);
}

void error(){
    exit(EXIT_FAILURE);
}

void errorNetwork(char * cmd){
    herror(cmd);
    exit(EXIT_FAILURE);
}