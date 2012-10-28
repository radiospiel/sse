#include "sse.h"

DEFINE_OBJECT(Options, options);

extern int post_main(int argc, char** argv);
extern int sse_main(int argc, char** argv);

/*
 * Multi-binary main function.
 */
int main(int argc, char** argv) {
  options.arg0 = *argv;

  if(streeq(*argv, "post"))
    return post_main(argc, argv);

  return sse_main(argc, argv);
}
