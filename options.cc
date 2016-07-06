/* liboptions by Lukas Joeressen.
 *
 * The author disclaims copyright to this source code.  In place of
 * a legal notice, here is a blessing:
 *
 * - May you do good and not evil.
 * - May you find forgiveness for yourself and forgive others.
 * - May you share freely, never taking more than you give. */

#include "options.h"
#include <cstdio>
#include <cstdlib>
#include <algorithm>

enum {
  CB_VOID,
  CB_STRING,
  CB_INTEGER,
  CB_FLOAT
};

#define NEED_ARGUMENT_LONG  "Option '--\0' needs an argument."
#define NEED_ARGUMENT_SHORT "Option '-\0' needs an argument."
#define UNRECOGNISED_LONG   "Unrecognised option '--\0'."
#define UNRECOGNISED_SHORT  "Unrecognised option '-\0'."
#define UNEXPECTED_ARGUMENT "Option '--\0' expects no argument."

static int fprintfs(FILE* f, const char *format, const char *str)
{
  while (*format) {putc(*(format++), f);} format++;
  while (*str)    {putc(*(str++), f);}
  while (*format) {putc(*(format++), f);}
  return 0;
}

static int fprintfc(FILE* f, const char *format, char c)
{
  while (*format) {putc(*(format++), f);} format++;
  putc(c, f);
  while (*format) {putc(*(format++), f);}
  return 0;
}

static bool str_equal(const char *a, const char *b)
{
  if (!a || !b) return 0;
  while (*a && *b && *a == *b) {++a; ++b;}
  return *a == *b;
}

static char *find_and_null_eq(char *s)
{
  while (*s) {
    if (*s == '=') {
      *s = 0;
      return s + 1;
    }
    ++s;
  }
  return 0;
}

int Options::parse(int &argc, char **&argv)
{
  auto apply_option = [](struct Options::Option &o, char *arg, int l) {
    if (!arg && o.type != CB_VOID) {
      if (l) {
        fprintfs(stderr, NEED_ARGUMENT_LONG "\n", o.long_opt);
      } else {
        fprintfc(stderr, NEED_ARGUMENT_SHORT "\n", o.short_opt);
      }
      exit(1);
    }
    switch (o.type) {
    case CB_VOID:
      o.callback_void();
      break;
    case CB_STRING:
      o.callback_string(arg);
      break;
    case CB_INTEGER:
      o.callback_integer(strtol(arg, 0, 0));
      break;
    case CB_FLOAT:
      o.callback_double(strtod(arg, 0));
      break;
    default:
      break;
    }
  };

  int c = 1, i = 1, j;
  char x;
  char *a, *v;
  auto o = _options.end();
  int l = 0;

  while (i < argc) {
    if (argv[i][0] == '-' && argv[i][1]) {
      if (o != _options.end()) {
        apply_option(*o, 0, l);
        o = _options.end();
      }
      if (argv[i][1] == '-' && !argv[i][2]) {
        /* End of options */
        ++i;
        while (i < argc) {
          argv[c] = argv[i];
          ++c;
          ++i;
        }
      } else if (argv[i][1] == '-') {
        /* Longopt */
        l = 1;
        a = argv[i] + 2;
        v = find_and_null_eq(a);
        o = std::find_if(_options.begin(), _options.end(), [=](struct Options::Option &opt) -> bool {
          return str_equal(opt.long_opt, a);
        });
        if (o == _options.end()) {
          fprintfs(stderr, UNRECOGNISED_LONG "\n", a);
          exit(1);
        }
        if (v || o->type == CB_VOID) {
          if (v && o->type == CB_VOID) {
            fprintfs(stderr, UNEXPECTED_ARGUMENT "\n", o->long_opt);
            exit(1);
          }
          apply_option(*o, v, l);
          o = _options.end();
        }
      } else {
        /* Shortopt */
        l = 0;
        j = 1;
        while ((x = argv[i][j])) {
          o = std::find_if(_options.begin(), _options.end(), [=](struct Options::Option &opt) -> bool {
            return opt.short_opt == x;
          });//lookup_short(options, x);
          if (o == _options.end()) {
            fprintfc(stderr, UNRECOGNISED_SHORT "\n", x);
            exit(1);
          }
          if (o->type == CB_VOID) {
            apply_option(*o, 0, l);
            o = _options.end();
          } else {
            if (argv[i][j + 1]) {
              apply_option(*o, argv[i] + j + 1, l);
              o = _options.end();
            }
            break;
          }
          ++j;
        }
      }
    } else {
      if (o != _options.end()) {
        apply_option(*o, argv[i], l);
        o = _options.end();
      } else {
        argv[c] = argv[i];
        ++c;
      }
    }
    ++i;
  }
  if (o != _options.end()) apply_option(*o, 0, l);

  /* Clean argument vector. */
  for (i = c; i < argc; ++i) {
    argv[i] = 0;
  }
  argc = c;

  return 0;
}

void Options::flag(char short_opt, const char *long_opt, std::function<void (void)> callback)
{
  _options.push_back({
    short_opt, long_opt, CB_VOID,
    callback,
    [](const char *s){},
    [](long i){},
    [](double f){}
  });
}

void Options::option(char short_opt, const char *long_opt, std::function<void (const char *)> callback)
{
  _options.push_back({
    short_opt, long_opt, CB_STRING,
    [](){},
    callback,
    [](long i){},
    [](double f){}
  });
}

void Options::option_i(char short_opt, const char *long_opt, std::function<void (long)> callback)
{
  _options.push_back({
    short_opt, long_opt, CB_INTEGER,
    [](){},
    [](const char *s){},
    callback,
    [](double f){}
  });
}

void Options::option_d(char short_opt, const char *long_opt, std::function<void (double)> callback)
{
  _options.push_back({
    short_opt, long_opt, CB_FLOAT,
    [](){},
    [](const char *s){},
    [](long i){},
    callback
  });
}
