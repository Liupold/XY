#include <string.h>

char* get_option(int argc, char **argv, char *option) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], option) == 0) {
      return argv[i + 1];
    };
  }
  return NULL;
}

double get_option_d(int argc, char **argv, char *option, double def_val) {
  char *c_val = get_option(argc, argv, option);
  if (c_val != NULL) {
    return strtod(c_val, NULL);
  }
  return def_val;
}

unsigned long get_option_ul(int argc, char **argv, char *option, unsigned long def_val) {
  char *c_val = get_option(argc, argv, option);
  if (c_val != NULL) {
    return strtoul(c_val, NULL, 10);
  }
  return def_val;
}

unsigned long long get_option_ull(int argc, char **argv, char *option, unsigned long long def_val) {
  char *c_val = get_option(argc, argv, option);
  if (c_val != NULL) {
    return strtoull(c_val, NULL, 10);
  }
  return def_val;
}
