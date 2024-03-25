#ifndef SERVER_H
#define SERVER_H

#define UNIMPLEMENTED                                                         \
  do {                                                                        \
    fprintf(stderr, "%s:%d: %s is not implemented yet\n", __FILE__, __LINE__, \
        __func__);                                                            \
    abort();                                                                  \
  } while (0)

#define DEFAULT_PORT       6969 /* The default server port number */
#define DEFAULT_QUEUE_SIZE 5    /* The default queue size */
#define MIN_ALLOWED_PORT \
  1024 /* Ports below 1024 are reserved for privileged applications */
#define MAX_ALLOWED_PORT 49151 /* Ports above 49151 are ephemeral ports */
#define MIN_QUEUE_SIZE   1     /* The minimum queue size */
#define CRLF \
  "\r\n" /* Carriage return and line feed, end of line marker used by HTTP*/

/* Begin typedef declarations */

/* Represents the options that can be passed to the program */
typedef enum ServerOptions {
  WORKING_DIRECTORY = 'd', /* The working directory */
  PORT_NUMBER = 'p',       /* The port number */
  QUEUE_SIZE = 'q',        /* The queue size */
  OUT_OF_OPTIONS = -1,     /* The end of the options */
} ServerOptions;

/* Begin function prototype declarations */

/* Create a network service */
int create_service(short port, int queue_size);
int accept_connection(int sock_fd);
void handle_request(int new_sock_fd);
void run_service(int sock_fd);

#endif /* SERVER_H */