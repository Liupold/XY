#include <stdio.h>
#include <string.h>

char *get_arg_param(int argc, char **argv, char *option) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], option) == 0) {
      return argv[i + 1];
    };
  }
  return NULL;
}

int get_flag(int argc, char **argv, char *flag) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0) {
      return 1;
    };
  }
  return 0;
}

double parse_arg_d(int argc, char **argv, char *option, double def_val) {
  char *c_val = get_arg_param(argc, argv, option);
  if (c_val != NULL) {
    return strtod(c_val, NULL);
  }
  return def_val;
}

unsigned long parse_arg_ul(int argc, char **argv, char *option,
                           unsigned long def_val) {
  char *c_val = get_arg_param(argc, argv, option);
  if (c_val != NULL) {
    return strtoul(c_val, NULL, 10);
  }
  return def_val;
}

unsigned long long parse_arg_ull(int argc, char **argv, char *option,
                                 unsigned long long def_val) {
  char *c_val = get_arg_param(argc, argv, option);
  if (c_val != NULL) {
    return strtoull(c_val, NULL, 10);
  }
  return def_val;
}

unsigned int parse_arg_ui(int argc, char **argv, char *option,
                          unsigned int def_val) {
  char *c_val = get_arg_param(argc, argv, option);
  if (c_val != NULL) {
    return (unsigned int)strtoul(c_val, NULL, 10);
  }
  return def_val;
}

int parse_arg_i(int argc, char **argv, char *option, int def_val) {
  char *c_val = get_arg_param(argc, argv, option);
  if (c_val != NULL) {
    return atoi(c_val);
  }
  return def_val;
}

int parse_help(int argc, char **argv, char *helpstr) {
  if (get_flag(argc, argv, "-help")) {
    printf("%s", helpstr);
    return 1;
  } else if (get_flag(argc, argv, "-h")) {
    printf("%s", helpstr);
    return 1;
  }
  return 0;
}
