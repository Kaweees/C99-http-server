#ifndef SERVER_H
#define SERVER_H

#define UNIMPLEMENTED                                                         \
  do {                                                                        \
    fprintf(stderr, "%s:%d: %s is not implemented yet\n", __FILE__, __LINE__, \
        __func__);                                                            \
    abort();                                                                  \
  } while (0)

#define PORT 6969 /* The server port number */

/* Begin typedef declarations */

/* Represents the options that can be passed to the program */
typedef enum ServerOptions {
  WORKING_DIRECTORY = 'd', /* The working directory */
  PORT_NUMBER = 'p',       /* The port number */
  OUT_OF_OPTIONS = -1,    /* The end of the options */
} ServerOptions;

/* Begin function prototype declarations */

#endif /* SERVER_H */