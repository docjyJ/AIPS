#ifndef AIPS_TSOCK_ERROR_H
#define AIPS_TSOCK_ERROR_H

extern char* errotCMD;

void usage();

void errorDetailed(char * error);

void error();

void errorNetworkH(char * error);

void errorNetworkP(char * error);

#endif //AIPS_TSOCK_ERROR_H
