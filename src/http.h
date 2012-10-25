/*
 * This file is part of the sse package, copyright (c) 2011, 2012, @radiospiel.
 * It is copyrighted under the terms of the modified BSD license, see LICENSE.BSD.
 *
 * For more information see https://https://github.com/radiospiel/sse.
 */

#ifndef HTTP_H
#define HTTP_H

#include <string.h>
#include <curl/curl.h>

/*
 * send HTTP request.
 */

#define HTTP_GET  1
#define HTTP_POST 2

extern void http(int  verb,
  const char*   url, 
  const char**  http_headers, 

  const char*   body, 
  unsigned      bodyLenght,

  size_t        (*on_data)(char *ptr, size_t size, size_t nmemb, void *userdata),
  const char*   (*on_verify)(CURL* curl)
);

extern char curl_error_buf[];

extern size_t http_ignore_data(char *ptr, size_t size, size_t nmemb, void *userdata);

#endif