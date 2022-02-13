#ifndef AIPS_TSOCK_ERROR_H
#define AIPS_TSOCK_ERROR_H

void usage(char * cmd);

void errorDetailed(char * error, char * cmd);

void error();

void errorNetworkH(char * cmd);

void errorNetworkP(char * cmd);

#endif //AIPS_TSOCK_ERROR_H
