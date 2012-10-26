all: post sse

static:
	STATIC=1 RELEASE=1 make clean all 

release:
	RELEASE=1 make clean all 

debug:
	make clean all 

# set PLATFORM values. NOTE: this is not yet used.
PLATFORM=

# The URL for curl.
CURL_URL=http://curl.haxx.se/download/curl-7.28.0.tar.gz

# --- compile flags ---------------------------------------------------

CFLAGS=-Isrc
RFLAGS=-Os -DNDEBUG -Wall
DFLAGS=-g -Wall -Werror

ifeq ($(RELEASE),1)
	CFLAGS:=$(CFLAGS) $(RFLAGS)
else
	CFLAGS:=$(CFLAGS) $(DFLAGS)
endif

ifeq ($(STATIC),1)
	CFLAGS:=$(CFLAGS) -I./curl/include
	LFLAGS=-L./lib -lcurl -lcrypto -lssl -lz
else
	LFLAGS=-lcurl
endif

# --- shortcuts -------------------------------------------------------

bin:
	mkdir bin

tmp:
	mkdir tmp

post: bin bin/post

sse:  bin bin/sse

clean: 
	rm -rf bin/*

# --- binaries --------------------------------------------------------

bin/post: bin/sse
	$(shell cd bin; ln -sf sse post)

bin/sse: src/main.c src/post.c src/sse.c src/tools.c src/http.c src/parse-sse.c
	gcc $(CFLAGS) -o $@ $^ $(LFLAGS)
ifeq ($(RELEASE),1)
	strip bin/sse
endif

src/parse-sse.c: src/parse-sse.fl
	flex -I -o $@ $<

# --- libcurl  --------------------------------------------------------
#
# This makefile is prepared to build a libcurl.a static library. To use
# it run "make static"

LIBCURL_CONFIG=--disable-debug       \
	 --enable-optimize     \
	 --enable-warnings     \
	 --disable-curldebug   \
	 --enable-symbol-hiding\
	 --disable-largefile   \
	 --disable-shared      \
	 --enable-http         \
	 --disable-ftp         \
	 --disable-file        \
	 --disable-ldap        \
	 --disable-ldaps       \
	 --disable-rtsp        \
	 --enable-proxy        \
	 --disable-dict        \
	 --disable-telnet      \
	 --disable-tftp        \
	 --disable-pop3        \
	 --disable-imap        \
	 --disable-smtp        \
	 --disable-gopher      \
	 --disable-manual      \
	 --enable-libcurl-option \
	 --disable-ipv6        \
	 --enable-verbose      \
	 --enable-crypto-auth  \
	 --enable-cookies

curl: tmp tmp/curl.tar.gz
	tar xvfz tmp/curl.tar.gz
	ln -sf curl-7.*.* curl

tmp/curl.tar.gz:
	curl -o tmp/curl.tar.gz $(CURL_URL) 

lib/libcurl.a: curl
	cd curl && ./configure $(LIBCURL_CONFIG) && make
	mkdir -p lib
	cp curl/lib/.libs/libcurl.a lib
