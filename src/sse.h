/*
 * This file is part of the sse package, copyright (c) 2011, 2012, @radiospiel.
 * It is copyrighted under the terms of the modified BSD license, see LICENSE.BSD.
 *
 * For more information see https://https://github.com/radiospiel/sse.
 */

#ifndef SSE_H
#define SSE_H

#define MAX_HEADERS 100

/* response limit: 128 kByte */
#define RESPONSE_LIMIT  128 * 1024

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define DECLARE_OBJECT(T, name) extern struct T name
#define DEFINE_OBJECT(T, name)  struct T name = T ## _Initializer

/*
 * Aplication options
 */
struct Options {
  const char *arg0;           // process name
  const char *url;            // URL to get
  int         limit;          // event limit
  int         verbosity;      // verbosity
  int         allow_insecure; // allow insecure connections
  const char *ssl_cert;       // SSL cert file
  const char *ca_info;        // CA cert file
  char       **command;       // command to run (if any)
};

#define Options_Initializer {0,0,0,0,0,0,0,0}
DECLARE_OBJECT(Options, options);

#define FD_STDIN    0
#define FD_STDOUT   1
#define FD_STDERR   2

/*
 * put some data into the SSE parser
 */
extern void parse_sse(char *ptr, size_t size);

/*
 * Callback for SSE events.
 */
extern void on_sse_event(char** headers, const char* data, const char* reply_url);

/*
 * Write \a dataLen bytes from \a data to \a fd.
 */
extern int write_all(int fd, const char* data, unsigned dataLen);

/*
 * read data from fd handle, return a malloced area in the pResult 
 * buffer - this must be freed by the caller - and returns the number
 * of bytes read.
 */
extern int read_all(int fd, char** pResult, size_t limit);

/*
 * print a 0-limited array of text \a lines to the \a output FILE handle.
 */
extern void fprint_list(FILE* output, char** lines);

/*
 * Write out an error message using perror(3) and exit 
 * the process via exit(3).
 */
extern void die(const char* msg);

/*
 * Write out an error message using perror(3) and exit 
 * the process via _exit(2).
 */
extern void _die(const char* msg);

/*
 * When \a options.verbosity is greater than or equal \a verbosity this 
 * function writes out \a data to stderr. After each newline the \a sep
 * string will be printed, if it is set.
 */
extern void logger(int verbosity, const char* data, unsigned len, const char* sep);

#endif
