all: post sse

TEST_URL=http://rubpubsub.16b.org

CFLAGS=-Isrc -Os -DNDEBUG -Wall
CFLAGS=-Isrc -g -Wall -Werror

# --- shortcuts -------------------------------------------------------

bin:
	mkdir bin

post: bin bin/post
sse:  bin bin/sse

clean: 
	rm -rf bin/*

# --- binaries --------------------------------------------------------

bin/post: src/post.c src/tools.c src/http.c src/tools.c
	gcc $(CFLAGS) -o $@ $^ -lcurl

bin/sse: src/sse.c src/tools.c src/http.c src/parse-sse.c
	gcc $(CFLAGS) -o $@ $^ -lcurl

src/parse-sse.c: src/parse-sse.fl
	flex -I -o $@ $<
