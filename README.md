# C++ Options

Easy option parsing.

## License

> The author disclaims copyright to this source code.  In place of
> a legal notice, here is a blessing:
>
> * May you do good and not evil.
> * May you find forgiveness for yourself and forgive others.
> * May you share freely, never taking more than you give.

## Example

```cpp
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
```

Compile with `c++ -std=c++11 -o example example.cc options.cc`.

## Usage

  - `class Options`

    Option parser class.
    Create a new instance, define some options and parse the command-line arguments.

  - `int Options::parse(int &argc, char **&argv)`

    Does the actual parsing of `argv`.
    When this function returns `argc` and `argv` will be modified and contain only the arguments, which were not parsed as options.
    A `--` in the command line stops the option parsing.

  - `void Options::flag(char short_opt, const char *long_opt, std::function<void (void)> callback)`

    Defines a flag.
    Whenever the flag is encountered on the command line, the callback will be called.

  - `void Options::option(char short_opt, const char *long_opt, std::function<void (const char *)> callback)`

    Defines a string option.
    Whenever this option is encountered, the callback will be called with the parameter as argument.

  - `void Options::option_i(char short_opt, const char *long_opt, std::function<void (long)> callback)`

    Like `option`, but the parameter will be converted to a long int.

  - `void Options::option_d(char short_opt, const char *long_opt, std::function<void (double)> callback)`

    Like `option`, but the parameter will be converted to double.
