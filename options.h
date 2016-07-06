/* liboptions by Lukas Joeressen.
 *
 * The author disclaims copyright to this source code.  In place of
 * a legal notice, here is a blessing:
 *
 * - May you do good and not evil.
 * - May you find forgiveness for yourself and forgive others.
 * - May you share freely, never taking more than you give. */

#pragma once

#include <functional>
#include <vector>

/* Option parser class.
 * Create a new instance, define some options and parse the command-line arguments. */
class Options {
public:
  /* Does the actual parsing of `argv`.
   * When this function returns `argc` and `argv` will be modified and contain only the
   * arguments, which were not parsed as options.
   * A `--` in the command line stops the option parsing. */
  int parse(int &argc, char **&argv);

  /* Defines a flag.
   * Whenever the flag is encountered on the command line, the callback will be called. */
  void flag(char short_opt, const char *long_opt, std::function<void (void)> callback);

  /* Defines a string option.
   * Whenever this option is encountered, the callback will be called with the parameter
   * as argument. */
  void option(char short_opt, const char *long_opt, std::function<void (const char *)> callback);

  /* Like `option`, but the parameter will be converted to a long int. */
  void option_i(char short_opt, const char *long_opt, std::function<void (long)> callback);

  /* Like `option`, but the parameter will be converted to double. */
  void option_d(char short_opt, const char *long_opt, std::function<void (double)> callback);

private:
  struct Option {
    char short_opt;
    const char *long_opt;
    int type;
    std::function<void (void)> callback_void;
    std::function<void (const char *)> callback_string;
    std::function<void (long)> callback_integer;
    std::function<void (double)> callback_double;
  };
  std::vector<struct Option> _options;
};
