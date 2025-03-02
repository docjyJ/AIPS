#include "tsock_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

const char usageTxt[] = "Usage: %s -p [-options] port \n"
                        "       %s -s [-options] host port \n"
                        "Options :\n"
                        "  -u    : utilise le service du protocole UDP, par défaut, le protocole TCP est utilisé\n"
                        "  -l ## : longueur (en octets) du message à émettre en émission ou longueur maximale du message à lire en réception (par défaut : 30)\n"
                        "  -n ## : définit le nombre de messages soit à émettre pour la source (par défaut : 10), soit à lire pour le puits (par défaut : infini)\n";

char* errotCMD = NULL;

void usage(){
    printf(usageTxt, errotCMD, errotCMD);
    exit(EXIT_SUCCESS);
}

void errorDetailed(char * error){
    fprintf(stderr, "%s: %s\n", errotCMD, error);
    exit(EXIT_FAILURE);
}

void error(){
    exit(EXIT_FAILURE);
}

void errorNetworkH(char * error){
    fprintf(stderr, "%s: ", errotCMD);
    herror(error);
    exit(EXIT_FAILURE);
}

void errorNetworkP(char * error){
    fprintf(stderr, "%s: ", errotCMD);
    perror(error);
    exit(EXIT_FAILURE);
}