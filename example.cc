#include <cstdio>
#include <cstdlib>
#include "options.h"

int main(int argc, char **argv)
{
  /* A new option parser. */
  Options o;
  /* A standard flag. -f or --flag. */
  o.flag('f', "flag", []() {
    printf("flag\n");
  });
  /* A parameter, which will be passed on as-is. */
  o.option('s', "string", [](const char *x) {
    printf("string: %s\n", x);
  });
  /* This parameter will be parsed as an integer. */
  o.option_i('i', "integer", [](long i) {
    printf("integer: %ld\n", i);
  });
  /* This one will become a double. */
  o.option_d('d', "double", [](double f) {
    printf("double: %g\n", f);
  });
  /* This help flag makes use of captures in lambda expressions. */
  o.flag('h', "help", [&]() {
    fprintf(stderr, "Usage: %s ...\n", argv[0]);
    exit(1);
  });
  /* Parse all the arguments. */
  o.parse(argc, argv);

  /* Print the rest of argv. */
  for (int i = 0; i < argc; ++i) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  return 0;
}
