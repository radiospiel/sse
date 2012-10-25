/*
 * This file is part of the sse package, copyright (c) 2011, 2012, @radiospiel.
 * It is copyrighted under the terms of the modified BSD license, see LICENSE.BSD.
 *
 * For more information see https://https://github.com/radiospiel/sse.
 */

#include "http.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "sse.h"

DEFINE_OBJECT(Options, options);

static void usage();

int main(int argc, char** argv) 
{
  char *data = NULL;

  /* === parse arguments =========================================== */

  options.arg0 = *argv;

  while(1) {
    int ch = getopt(argc, argv, "vic:a:?h");
    if(ch == -1) break;
    
    switch (ch) {
    case 'c': options.ssl_cert = optarg; break;
    case 'a': options.ca_info = optarg; break;
    case 'i': options.allow_insecure = 1; break;
    case 'v': options.verbosity += 1; break;
    case '?':
    case 'h':
    default:
      usage();
    }
  }

  argc -= optind;
  argv += optind;

  if(*argv) {
    options.url = *argv++;
  }
  
  if(*argv) {
    data = *argv;
  }

  if(!options.url) usage();

  /* === read data from stdin, if needed =========================== */
  
  char* buf = 0;
  
  unsigned dataLength = 0;
  if(data)
    dataLength = strlen(data);
  else {
    /*
     * If the event data is not passed in from the command line we
     * read it from stdin.
     * 
     * TODO: set up a libcurl callback instead of reading here;
     * see http://curl.haxx.se/libcurl/c/post-callback.html
     */
    dataLength = read_all(FD_STDIN, &data, RESPONSE_LIMIT);
    if(dataLength <= 0)
      die("read");
    buf = data;
  } 
  
  /* === send request to server ==================================== */

  const char* headers[] = {
    "Content-Type:",
    NULL
  };

  http(HTTP_POST, options.url, headers, data, dataLength, 0, 0);

  if(buf) free(buf);
  
  return 0;
}

static char* help[] = {
  "",
  "post [ <options> ] URL [ <data> ]",
  "",
  "Post some data to a HTTP(S) server.",
  "",
  "Options include:",
  "",
  "  -a <ca>     ... set PEM CA file",
  "  -c <cert>   ... set PEM certificate file",
  "  -i          ... insecure: allow HTTP and non-certified HTTPS connections",
  "  -v          ... be verbose; can be set multiple times",
  NULL
};

static void usage() {
  fprint_list(stderr, help);
  fprintf(stderr, "\nThis is %s, compiled %s %s.\n\n", options.arg0, __DATE__, __TIME__);

  exit(1);
}
