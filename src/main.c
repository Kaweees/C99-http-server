#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.h"

#define USAGE_STRING \
  "Usage: %s -d  <directory> -p <port>\n" /* Program usage string */
#define MIN_ALLOWED_PORT \
  1024 /* Ports below 1024 are reserved for privileged applications */
#define MAX_ALLOWED_PORT 49151 /* Ports above 49151 are ephemeral ports */

/**
 * Prints the proper usage of the program and exits unsuccessfully.
 */
void usage(char* prog_name) {
  fprintf(stderr, USAGE_STRING, prog_name);
  exit(EXIT_FAILURE);
}

/**
 * @brief Program entry point
 *
 * @param argc - the number of command line arguments
 * @param argv - an array of command line arguments
 * @return int - the exit status
 */
int main(int argc, char* argv[]) {
  enum ServerOptions opt;
  char* working_directory = ".";
  uint16_t port = PORT;
  if (argc >= 2) {
    working_directory = argv[2];
  }

  while ((opt = getopt(argc, argv, "d:p:")) != OUT_OF_OPTIONS) {
    switch (opt) {
      case WORKING_DIRECTORY:
        working_directory = optarg;
        break;
      case PORT_NUMBER:
        port = strtol(optarg, NULL, 10);
        if (!(MIN_ALLOWED_PORT <= port && port <= MAX_ALLOWED_PORT)) {
          fprintf(stderr, "Invalid port number: %s\n", optarg);
          usage(argv[0]);
        }
        break;
      default:
        usage(argv[0]);
    }
  }
  return EXIT_SUCCESS;
}
