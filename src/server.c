#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SYSCALL_FAILURE -1

/**
 * @brief Handler for the SIGCHLD signal, which waits for child processes to
 * exit in order to prevent zombie processes.
 * @param sig - the signal number
 */
void handle_sigchld(int sig) {
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

/**
 * @brief Create a network service that listens on the specified port
 *
 * @param port - the port to listen on
 * @param queue_size - the maximum number of pending connections in the socket's
 * @return int - a socket descriptor for the service
 */
int create_service(short port, int queue_size) {
  int sock_fd; /* Socket file descriptor */
  /* Socket address information */
  struct sockaddr_in sock_addr = {
      .sin_family = AF_INET,   /* IPv4-compliant protocol */
      .sin_port = htons(port), /* short, network byte order (big-endian) */
      .sin_addr = htonl(INADDR_ANY), /* long, host byte order (varies),
                                        listen to all interfaces */
      .sin_zero = {0} /* Padding to match size of struct sockaddr */
  };

  /**
   * Create the socket using IPv4 (AF_INET), and TCP (SOCK_STREAM). Use AF_INET6
   * for IPv6 and SOCK_DGRAM for UDP.
   */
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == SYSCALL_FAILURE) {
    perror("Error creating the server socket");
    exit(EXIT_FAILURE);
  }

  /**
   * Set the socket options to allow the reuse of the address. This is useful
   * when the server is restarted and the port is still in use.
   */
  int reuse = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) ==
      SYSCALL_FAILURE) {
    perror("Error setting the server socket options");
    exit(EXIT_FAILURE);
  }

  /**
   * Bind the socket to the address and port specified in the sockaddr_in
   * struct.
   */
  if (bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr)) ==
      SYSCALL_FAILURE) {
    perror("Error binding the server socket to the address");
    exit(EXIT_FAILURE);
  }

  /**
   * Listen for incoming connections on the socket. The second argument is the
   * maximum number of pending connections in the socket's listen queue.
   */
  if (listen(sock_fd, queue_size) == SYSCALL_FAILURE) {
    perror("Error listening on the server socket");
    exit(EXIT_FAILURE);
  }

  /* Set the signal handler for the server for when a child process dies */
  struct sigaction sigchld_action = {0};
  sigchld_action.sa_handler = handle_sigchld;
  sigemptyset(&sigchld_action.sa_mask);
  sigchld_action.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sigchld_action, NULL) == SYSCALL_FAILURE) {
    perror("Error setting the server SIGCHLD signal handler ");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port: %d\n", port);
  return sock_fd;
}