#include "tsock_error.h"
#include <stdio.h>
#include <stdlib.h>

const char usage[] = "Usage: %s -p [-options] port \n"
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
void printUsage(char * cmd){
    fprintf(stderr, usage, cmd, cmd);
}

void printError(char * error, char * cmd){
    fprintf(stderr, "%s: %s\n", cmd, error);
}

void usageError(char * cmd){
    printUsage(cmd);
    exit(1);
}

void usageErrorDetailed(char * error, char * cmd){
    printError(error, cmd);
    printUsage(cmd);
    exit(2);
}

void ErrorDetailed(char * error, char * cmd){
    printError(error, cmd);
    exit(3);

}