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
#define SYSCALL_ERROR -1

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
  char* working_directory = DEFAULT_WORKING_DIRECTORY;
  int port = DEFAULT_PORT;
  int queue_size = DEFAULT_QUEUE_SIZE;
  if (argc >= 2) {
    working_directory = argv[2];
  }

  /* Parse the command line arguments */
  while ((opt = getopt(argc, argv, "d:p:q:")) != OUT_OF_OPTIONS) {
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
      case QUEUE_SIZE:
        queue_size = strtol(optarg, NULL, 10);
        if (!(MIN_QUEUE_SIZE <= queue_size)) {
          fprintf(stderr, "Invalid queue size: %s\n", optarg);
          usage(argv[0]);
        }
        break;
      default:
        usage(argv[0]);
    }
  }

  /* Set stdout to be unbuffered so that logs are printed immediately */
  setbuf(stdout, NULL);

  /* Change the working directory */
  if (chdir(working_directory) == SYSCALL_ERROR) {
    perror("Error changing directory");
    exit(EXIT_FAILURE);
  } else {
    printf("Changed working directory to: %s\n", working_directory);
  }

  /* Create the server */
  int fd = create_service(port, queue_size);
  /* Run the server */
  run_service(fd);
  /* Close the server */
  close(fd);
  return EXIT_SUCCESS;
}
