# SSE

This package contains a native client for HTTP server sent events. 

The `sse` client is geared to the specification as per [http://www.w3.org/TR/eventsource](http://www.w3.org/TR/eventsource), but does not follow it completely. See below for known differences.

## sse: listening to an SSE stream

sse connects to an URL, where it expects a stream of server sent events. On each incoming event it runs a command specified on the command line, passing in event data via process environment.

    sse [ <options> ] URL [ <command> ... ]

On each incoming event `sse` runs `command`, with any additional arguments passed in at the command line.

Options include:

      -a <ca>      ... set PEM CA file
      -c <cert>    ... set PEM certificate file
      -i           ... insecure: allow HTTP and non-certified HTTPS connections
      -l <limit>   ... limit number of events
      -v           ... be verbose; can be set multiple times

The event's `data` attribute is written to the command's standard input. All other event attributes are passed via environment variables (`SSE_EVENT`, `SSE_ID`, and so on.)

If a SSE "reply" attribute is set, sse also posts the command's result to the URL specified there.

### `sse` security

By default, `sse` only accepts HTTPS connections. It verifies the complete certificate chain and the host name. To run
against HTTP and insecure HTTPS connections use the "-i" parameter. 

If you need to use CA files different from the system default, use the `-a` and `-c` options. 

### Issues and limitations.

`sse` violates the eventsource specifications in a number of ways. They

- When evaluating an event stream `sse` does not decode anything as UTF-8, and does not do any error checking on this.
- `sse` does not deal with LF characters in its input.
- `sse` does not ignore any field names (There is a compile time limit on possible fields, though.)
- `sse` ignores lines without a colon, instead of setting a event field with no value
- `sse` resets the "id" between events
- `sse` does not evaluate "retry" fields

In addition, if an event has a "reply" field, and that field contains an URL, `sse` sends the result of the command execution via HTTP(S) POST to that URL.

**Remember:** `sse` was extracted from a communication suite intended to run on mobile devices. As such, it implements some things
differently from the specs. It should still be able to listen to any conforming SSE stream, though (with the notable exception of dealing with LF characters).

## post: listening to an SSE stream

For development purposes this package also contains a `post` binary, which one can use to POST some data to a URL.

    post [ <options> ] URL [ <data> ]

Options include:

    -a <ca>     ... set PEM CA file
    -c <cert>   ... set PEM certificate file
    -i          ... insecure: allow HTTP and non-certified HTTPS connections
    -v          ... be verbose; can be set multiple times

Usage examples:

    echo -n foo | post http://some.where/bar
    post http://some.where/bar foo

But really `post` does not offer any advantages over clients such as `curl` or `wget`.

## Building

    make

builds the binaries `./bin/sse` and `./bin/post`. 

